/**CFile****************************************************************

  FileName    [parser.c]

  SystemName  [IFJ - PROJECT]

  PackageName [Syntactic analysis]

  Synopsis    []

  Author      [Adam Pankuch]

  Affiliation []

  Date        [26/10/2018]

  Revision    []

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                          INCLUDES                                ///
////////////////////////////////////////////////////////////////////////

#include "parser.h"

#include <stdbool.h>
//////////////////////
#include <unistd.h> 

#include "scanner.h"

#include "stackStr.h"
#include "stackTkn.h"
#include "queue.h"

#include "token.h"
#include "symtable.h"

#include "dynamicArrInt.h"
#include "dynamicStr.h"

#include "error.h"


////////////////////////////////////////////////////////////////////////
///                       GLOBAL VARIABLES                           ///
////////////////////////////////////////////////////////////////////////

// LL-Grammar table
int ll_table[LL_ROWS][LL_COLS] = {
    // if/els/elif/end/whi/def/EOL/EOF/ =/ (/  )/  ,/  ID/ FUNC/expr
    {   1,	0,	0,	0,	1,	1,	2,	3,	0,	0,	0,	0,	1,	1,	1,  },  // [st-list]
    {   0,	0,	0,	0,	0,	0,	4,	5,	0,	0,	0,	0,	0,	0,	0,	},  // [EOF-EOL]
    {   7,	0,	0,	0,	7,	6,	0,	0,	0,	0,	0,	0,	7,	7,	7,	},  // [stat]
    {   9,	0,	0,	0,	8,	0,	0,	0,	0,	0,	0,	0,	10,	0,	0,	},  // [command]
    {   0,	0,	0,	0,	0,	0,	0,	0,	11,	0,	0,	0,	0,	0,	0,	},  // [func-assign-expr]
    {   12,	0,	0,	14,	12,	0,	13,	0,	0,	0,	0,	0,  12,	12,	12,	},  // [end-list]
    {   15,	18,	17,	19,	15,	0,	16,	0,	0,	0,	0,	0,	15,	15,	15,	},  // [if-list]
    {   0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,  0,	20,	21,	0,	},  // [id-func]
    {   0,	0,	0,	0,	0,	0,	24,	0,	0,	22,	0,	0,	23,	0,	0,	},  // [params-gen]
    {   0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	26,	0,	25,	0,	0,	},  // [p-brackets]
    {   0,	0,  0,	0,	0,	0,	0,	0,	0,	0,	28,	27,	0,	0,	0,	},  // [p-brackets-cont]
    {   0,	0,	0,	0,	0,	0,	30,	0,	0,	0,	0,	29,	0,	0,	0,	}   // [p-without]
};


char *reverted_rules[RULES_ROWS][RULES_COLS] = {
    /*RULE_0*/              {  NULL, }, // empty rule   
    /*ST_LIST_1*/           { "[EOL-EOF]", "[stat]", },
    /*ST_LIST_2*/           { "[st-list]", "EOL", },
    /*ST_LIST_3*/           {  NULL, }, 
    /*EOL_EOF_4*/           { "[st-list]", "EOL", },
    /*EOL_EOF_5*/           {  NULL, }, 
    /*STAT_6*/              { "[end-list]", "[params-gen]", "[id-func]", "def", },
    /*STAT_7*/              { "[command]", },
    /*COMMAND_8*/           { "[end-list]", "EOL", "do", "**while**", "while", },
    /*COMMAND_9*/           { "[if-list]", "EOL", "then", "**if**", "if", },
    /*COMMAND_10*/          { "[func-assign-expr]", "ID", },
    /*FUNC_ASSIGN_EXPR_11*/ { "**expr**", "=", },
    /*END_LIST_12*/         { "[end-list]", "EOL", "[command]", },
    /*END_LIST_13*/         { "[end-list]", "EOL", },
    /*END_LIST_14*/         { "end", },
    /*IF_LIST_15*/          { "[if-list]", "EOL", "[command]", },
    /*IF_LIST_16*/          { "[if-list]", "EOL", },
    /*IF_LIST_17*/          { "[if-list]", "EOL", "then", "**elif**", "elif", },
    /*IF_LIST_18*/          { "[end-list]", "EOL", "else", },
    /*IF_LIST_19*/          { "end", },
    /*ID_FUNC_20*/          { "ID", },
    /*ID_FUNC_21*/          { "FUNC", },
    /*PARAMS_GEN_22*/       { "[p-brackets]", "(", },
    /*PARAMS_GEN_23*/       { "[p-without]", "ID", },
    /*PARAMS_GEN_24*/       { "EOL", },
    /*P_BRACKETS_25*/       { "[p-brackets-cont]", "ID", },
    /*P_BRACKETS_26*/       { "EOL", ")", },
    /*P_BRACKETS_CONT_27*/  { "[p-brackets-cont]", "ID", ",", },
    /*P_BRACKETS_CONT_28*/  { "EOL", ")", },
    /*P_WITHOUT_29*/        { "[p-without]", "ID", ",", },
    /*P_WITHOUT_30*/        { "EOL", },
};

////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
////////////////////////////////////////////////////////////////////////

/**
 * @brief Find index of corresponding rule in ll_table
 * 
 * @param nonterm name of nonterminal
 * @param term name of terminal
 * @return int index of rule
 */
int LLtableFind(char *nonterm, char *term)
{
    int nonterm_idx = 0;
    int term_idx = 0;

    if (strcmp(nonterm, "[st-list]") == 0)
        nonterm_idx = ST_LIST_nonterm;
    else if (strcmp(nonterm, "[EOF-EOL]") == 0)
        nonterm_idx = EOF_EOL_nonterm;
    else if (strcmp(nonterm, "[stat]") == 0)
        nonterm_idx = STAT_nonterm;
    else if (strcmp(nonterm, "[command]") == 0)
    {
        // special case
        if (isExpr(term))
            return EXPR_INCLUDE; 
        
        nonterm_idx = COMMAND_nonterm;
    }
    else if (strcmp(nonterm, "[func-assign-expr]") == 0)
    {
         // special case
        if (strcmp(term, "=") != 0)
            return EXPR_INCLUDE_TWO;

        nonterm_idx = FUNC_ASSIGN_EXPR_nonterm;
    }
    else if (strcmp(nonterm, "[end-list]") == 0)
        nonterm_idx = END_LIST_nonterm;
    else if (strcmp(nonterm, "[if-list]") == 0)
        nonterm_idx = IF_LIST_nonterm;
    else if (strcmp(nonterm, "[id-func]") == 0)
        nonterm_idx = ID_FUNC_nonterm;
    else if (strcmp(nonterm, "[params-gen]") == 0)
        nonterm_idx = PARAMS_GEN_nonterm;
    else if (strcmp(nonterm, "[p-brackets]") == 0)
        nonterm_idx = P_BRACKETS_nonterm;
    else if (strcmp(nonterm, "[p-brackets-cont]") == 0)
        nonterm_idx = P_BRACKETS_CONT_nonterm;
    else if (strcmp(nonterm, "[p-without]") == 0)
        nonterm_idx = P_WITHOUT_nonterm;
    
    
    if (strcmp(term, "if") == 0)
        term_idx = IF_term;
    else if (strcmp(term, "else") == 0)
        term_idx = ELSE_term;
    else if (strcmp(term, "elif") == 0)
        term_idx = ELIF_term;
    else if (strcmp(term, "end") == 0)
        term_idx = END_term;    
    else if (strcmp(term, "while") == 0)
        term_idx = WHILE_term;
    else if (strcmp(term, "def") == 0)
        term_idx = DEF_term;
    else if (strcmp(term, "EOL") == 0)
        term_idx = EOL_term;
    else if (strcmp(term, "EOF") == 0)
        term_idx = EOF_term;
    else if (strcmp(term, "=") == 0)
        term_idx = EQUALS_term;
    else if (strcmp(term, "(") == 0)
        term_idx = L_BRACKET_term;
    else if (strcmp(term, ")") == 0)
        term_idx = R_BRACKET_term;
    else if (strcmp(term, ",") == 0)
        term_idx = COMMA_term;
    else if (strcmp(term, "ID") == 0)
        term_idx = ID_term;
    else if (strcmp(term, "FUNC") == 0)
        term_idx = FUNC_term;
    else if (isExpr(term)) // has to be behind strcmp(term, "FUNC") otherwise wont work
        term_idx = EXPR_term;


    //printf("nonterm_idx: %d\tterm_idx: %d\n", nonterm_idx, term_idx);

    // return value from corresponding bucket in ll_table
    return ll_table[nonterm_idx][term_idx];
}


void print_fun(elem_t *element)
{
    printf("FUN %s\n", element->func.key);
}


void print_var(elem_t *element)
{
    printf("VAR %s\n", element->var.key);
}


bool createTempKey(char **key_tmp, char *name)
{
    *key_tmp = malloc( (strlen(name) + 1) * sizeof(char) );
    if (*key_tmp == NULL)
        return false;
    strcpy(*key_tmp, name);

    return true;
}


void destroyTempKey(char **key_tmp)
{
    if (*key_tmp != NULL)
    {
        free(*key_tmp);
        *key_tmp = NULL;
    }
}


bool pushRevertedRules(stack_tkn_t *stack_tkn, int rule)
{
    int i = 0;
    token_t *token;
    token_info_t info = { .ptr = NULL };

    while (reverted_rules[rule][i] != NULL)
    {
        token = createToken(reverted_rules[rule][i], info);
        if (token == NULL)
            return false;
        if ( ! stcTkn_push(stack_tkn, token))
        {
            destroyToken(token);
            return false;
        }
        i++;
    }       

    return true;     
}



void freeAll(stack_tkn_t *stack_tkn, stack_str_t *stack_str, symtable_t *gl_var_tab, symtable_t *fun_tab, symtable_t *lc_var_tab, char **key_tmp, token_t *act, token_t *token)
{
    stcTkn_destroy(stack_tkn);
    stcStr_destroy(stack_str);
    symtab_free(gl_var_tab);
    symtab_free(fun_tab);
    if (lc_var_tab != NULL)
        symtab_free(lc_var_tab);

    destroyTempKey(key_tmp);
    destroyToken(act);
    destroyToken(token);
}


///////////////////////////////////////////////////////////////////////////////
int precedenc_analysis_temp(dynamicStr_t *sc_str, queue_t *que)
{
    token_t *act = scanner_get(sc_str, que);
    //printf("expr handling: %s\n", act->name);
    if (strcmp(act->name, "then") != 0 && strcmp(act->name, "do") != 0 && strcmp(act->name, "EOL") != 0 && strcmp(act->name, "EOF") != 0)
    {
        do {
            destroyToken(act);
            act = scanner_get(sc_str, que);
            //printf("expr handling: %s\n", act->name);
        } while(strcmp(act->name, "then") != 0 && strcmp(act->name, "do") != 0 && strcmp(act->name, "EOL") != 0 && strcmp(act->name, "EOF") != 0);
    }
    scanner_unget(que, act, sc_str->str);
    
    return 0;
}
///////////////////////////////////////////////////////////////////////////////


int expresionsHandler(dynamicStr_t *sc_str, queue_t *que, stack_tkn_t *stack_tkn, token_t *act)
{
    // destroy token: "**expr**"
    token_t *token;

    token = stcTkn_pop(stack_tkn);
    destroyToken(token);

    scanner_unget(que, act, sc_str->str);
    
    // SPUSTENIE PRECEDENCNEJ ANALYZY 
    //////////////////////////////////////////////////
    return precedenc_analysis_temp(sc_str, que);
    /////////////////////////////////////////////////
}


/**
 * @brief Parser - Syntactic analysis
 * 
 * @return int return value
 */
int parser(dynamicStr_t *sc_str, queue_t *que)
{
    //printf("Parser started\n"); 
    
    // DEFINE VARIABLES
    token_t *top = NULL; 
    token_t *act = NULL;

    bool succ = false;
    bool fail = false;

    bool get_new_token = true;
    bool get_func_params = false;

    int rule;
    int ret_val;

    char *id_key_tmp = NULL;
    char *func_key_tmp = NULL;
    int param_cnt;

    stack_tkn_t *stack_tkn;
    stack_str_t *stack_str;

    symtable_t *fun_tab;
    symtable_t *gl_var_tab;
    symtable_t *lc_var_tab = NULL;


    /// INIT STRUCTURES
    stack_tkn = stcTkn_create();
    if (stack_tkn == NULL)
        return ERR_INTERNAL;
    stack_str = stcStr_create();
    if (stack_str == NULL)
        goto err_internal_1;
    gl_var_tab = symtab_init(10, VARIABLES);
    if (gl_var_tab == NULL)
        goto err_internal_2;
    fun_tab = symtab_init(10, FUNCTIONS);
    if (fun_tab == NULL)
        goto err_internal_3;


    /// INITIAL PUSH of EOF and starting nonterminal
    token_t *token;
    token_info_t info = { .ptr = NULL };

    token = createToken("EOF", info);
    if (token == NULL)
        goto err_internal_3;
    stcTkn_push(stack_tkn, token); // error cannot occur
     
    token = createToken("[st-list]", info);
    if (token == NULL)
        goto err_internal_3;
    stcTkn_push(stack_tkn, token); // error cannot occur

    token = NULL;

    /////////////////////////////////////////
    ///             MAIN LOOP             ///
    /////////////////////////////////////////
    do {

        top = stcTkn_top(stack_tkn);
        if (get_new_token)
        {   
            act = scanner_get(sc_str, que);
            //printf("scanner_get: %s\n", act->name);
            
            if (strcmp(act->name, "ERR_LEX") == 0)
                goto err_lexical;
            else if (strcmp(act->name, "ERR_INTERNAL") == 0)
                goto err_internal_main;
            
            get_new_token = false;
        }


        if (strcmp(top->name, "EOF") == 0)
        {
            if (strcmp(act->name, "EOF") == 0)
            {
                printf("EOF reached on both stack_tkn and scanner\n");
                succ = true;
            }
            else
            {
                printf("EOF reached on stack_tkn but not from scanner\n");
                fail = true;
            }
        }
        else if (isTerminal(top->name))
        {
            if (strcmp(top->name, "**if**") == 0)
            {
                //printf("*********** IF ***********\n");                
                ret_val = expresionsHandler(sc_str, que, stack_tkn, act);
                
                //////////////////////
                // GENERATE IF 
                //////////////////////
                printf("if\n");
                // push to stack_tkn "epilog of if" and "else branch" (in this order)
                stcStr_push(stack_str, "endif\n");
                stcStr_push(stack_str, "else\n");

                get_new_token = true;
                //printf("********** END ***********\n");
            }
            else if (strcmp(top->name, "**while**") == 0)
            {
                //printf("********* WHILE **********\n");
                ret_val = expresionsHandler(sc_str, que, stack_tkn, act);
                
                //////////////////////
                // GENERATE WHILE
                /////////////////////
                printf("while\n");
                // push to stack_tkn "epilog of while"
                stcStr_push(stack_str, "endwhile\n");

                get_new_token = true;
                //printf("********** END ***********\n");
            }
            else if (strcmp(top->name, "**expr**") == 0)
            {
                //printf("********** EXPR **********\n");
                ret_val = expresionsHandler(sc_str, que, stack_tkn, act);
                
                ////////////////////////////////
                // GENERATE VARIABLE definition
                ///////////////////////////////
                printf("var %s\n", id_key_tmp);
                
                symtab_elem_add(gl_var_tab, id_key_tmp); // TODO
                destroyTempKey(&id_key_tmp);

                get_new_token = true;
                //printf("********** END ***********\n");
            }
        /*  else if (strcmp(top->name, "**elif**") == 0)
            {

            } */
            else if (strcmp(top->name, act->name) == 0)
            {
                //printf("top == act: %s\n", act->name);
                if (get_func_params)
                {
                    if (strcmp(act->name, "ID") == 0)
                    {
                        if (param_cnt >= 0)
                        {
                            symtab_elem_add(lc_var_tab, sc_str->str);
                        }
                        param_cnt++;
                    }
                    else if (strcmp(act->name, "EOL") == 0)
                    {                   
                        //////////////////////////////
                        // GENERATE FUNCTION prolog
                        /////////////////////////////
                        printf("def %s params %d\n", func_key_tmp, param_cnt);
                        // push to stack_tkn function epilog
                        stcStr_push(stack_str, "enddef\n");

                        symtab_elem_add(fun_tab, func_key_tmp);
                        destroyTempKey(&func_key_tmp);

                        get_func_params = false;
                    }
                }
                if (strcmp(act->name, "end") == 0 || strcmp(act->name, "else") == 0)
                {
                    // stack_tkn pop and add to output
                    printf("%s", stcStr_top(stack_str));
                    stcStr_pop(stack_str);
                }

                token = stcTkn_pop(stack_tkn);
                destroyToken(token);
                token = NULL;
                destroyToken(act);
                act = NULL;
                
                get_new_token = true;
            }
            else
            {
                printf("top != name ... top: %s\tact: %s\n", top->name, act->name);
                fail = true;
            }
                
        }
        else
        {
            rule = LLtableFind(top->name, act->name);
            if (rule == COMMAND_10) // [command] -> ID [func-assign-expr] ????????????
            {
                if ( ! createTempKey(&id_key_tmp, sc_str->str))
                    goto err_internal_main;
                //printf("id_tmp load %s\n", id_key_tmp);
            }
            else if (rule == ID_FUNC_20 || rule == ID_FUNC_21)
            {   
                if ( ! createTempKey(&func_key_tmp, sc_str->str))
                    goto err_internal_main;

                // HACK :D
                param_cnt = (strcmp(act->name, "ID") == 0) ? -1 : 0;
                
                //printf("create local hash table for function\n");
                if (lc_var_tab == NULL) // TODO
                    lc_var_tab = symtab_init(10, VARIABLES);

                get_func_params = true;
            }

            // need to be if (not else if) !!!
            if (rule == EXPR_INCLUDE)
            {
                //printf("********** EXPR **********\n");
                ret_val = expresionsHandler(sc_str, que, stack_tkn, act);
                get_new_token = true;
                //printf("********** END ***********\n");
            }
            else if (rule == EXPR_INCLUDE_TWO)
            {
                //printf("********** EXPR **********\n");
                // destroy token: "[func-assign-expr]"
                token = stcTkn_pop(stack_tkn);
                destroyToken(token);
                token = NULL;
                
                scanner_unget(que, createToken("ID", info), id_key_tmp);
                destroyTempKey(&id_key_tmp);
                scanner_unget(que, act, sc_str->str);
                
                // SPUSTENIE PRECEDENCNEJ ANALYZY
                //////////////////////////////////////////
                ret_val = precedenc_analysis_temp(sc_str, que);
                /////////////////////////////////////////
                get_new_token = true;
                //printf("********** END ***********\n");
            }
            else if (rule) // in case rule == 0 -- fail
            {
                //printf("top: %s\tact: %s\n", top->name, act->name);
                token = stcTkn_pop(stack_tkn);
                destroyToken(token);
                token = NULL;

                if ( ! pushRevertedRules(stack_tkn, rule))
                    goto err_internal_main;          
            }
            else
            {
                printf("no corresponding rule ... top: %s\tact: %s\n", top->name, act->name);
                fail = true;
            } 
        }

        /*
        switch (ret_val)
        {
            case ERR_INTERNAL:  goto err_internal_main; break;
            case ERR_SYN:       goto err_syntactic; break;
            case ERR_SEM_UNDEF: goto err_sem_undef; break;
            case ERR_SEM_TYPE:  goto err_sem_type; break;
            case ERR_SEM_FUNC:  goto err_sem_func; break;
            case ERR_SEM_OTHER: goto err_sem_other; break;
            case ERR_ZERO_DIV:  goto err_sem_div; break;
            default: break;
        }
        */
    } while (succ == false && fail == false);
    /////////////////////////////////////////
    ///         END OF MAIN LOOP          ///
    /////////////////////////////////////////

    if (fail)     
        goto err_syntactic;

    printf("\nglobal\n");
    symtab_foreach(gl_var_tab, print_var);
    printf("\nfunctions\n");
    symtab_foreach(fun_tab, print_fun);
    printf("\nlocal\n");
    symtab_foreach(lc_var_tab, print_var);
    printf("\n");

    // free all alocated elements
    freeAll(stack_tkn, stack_str, gl_var_tab, fun_tab, lc_var_tab, &id_key_tmp, act, token);
    return SUCCESS;

///////////////////////////////////////
///         ERROR HANDLING          ///
///////////////////////////////////////
err_internal_1:
    stcTkn_destroy(stack_tkn);
    error_msg("internal\n");
    return ERR_INTERNAL;

err_internal_2:
    stcTkn_destroy(stack_tkn);
    stcStr_destroy(stack_str);
    symtab_free(gl_var_tab);
    error_msg("internal\n");
    return ERR_INTERNAL;

err_internal_3:
    stcTkn_destroy(stack_tkn);
    stcStr_destroy(stack_str);
    symtab_free(gl_var_tab);
    symtab_free(fun_tab);
    error_msg("internal\n");
    return ERR_INTERNAL;

err_internal_main:
    freeAll(stack_tkn, stack_str, gl_var_tab, fun_tab, lc_var_tab, &id_key_tmp, act, token);
    error_msg("internal\n");
    return ERR_INTERNAL;

err_lexical:
    freeAll(stack_tkn, stack_str, gl_var_tab, fun_tab, lc_var_tab, &id_key_tmp, act, token);
    error_msg("lexical\n");
    return ERR_LEX;

err_syntactic:
    freeAll(stack_tkn, stack_str, gl_var_tab, fun_tab, lc_var_tab, &id_key_tmp, act, token);
    error_msg("syntactic\n");
    return ERR_SYN;
/*
err_sem_undef:
    freeAll(stack_tkn, gl_var_tab, fun_tab, lc_var_tab, &id_key_tmp, act, token);
    error_msg("semantic - undefinde variable / function\n");
    return ERR_SEM_UNDEF;

err_sem_type:

err_sem_func:

err_sem_other:

err_zero_div:
*/
}



////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////

