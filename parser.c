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

#include "stack.h"
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
    {  NULL, }, // empty rule   
    { "[EOL-EOF]", "[stat]", },
    { "[st-list]", "EOL", },
    {  NULL, }, 
    { "[st-list]", "EOL", },
    {  NULL, }, 
    { "[end-list]", "[params-gen]", "[id-func]", "def", },
    { "[command]", },
    { "[end-list]", "EOL", "do", "**expr**", "while", },
    { "[if-list]", "EOL", "then", "**expr**", "if", },
    { "[func-assign-expr]", "ID", },
    { "**expr**", "=", },
    { "[end-list]", "EOL", "[command]", },
    { "[end-list]", "EOL", },
    { "end", },
    { "[if-list]", "EOL", "[command]", },
    { "[if-list]", "EOL", },
    { "[if-list]", "EOL", "then", "**expr**", "elif", },
    { "[end-list]", "EOL", "else", },
    { "end", },
    { "ID", },
    { "FUNC", },
    { "[p-brackets]", "(", },
    { "[p-without]", "ID", },
    { "EOL", },
    { "[p-brackets-cont]", "ID", },
    { "EOL", ")", },
    { "[p-brackets-cont]", "ID", ",", },
    { "EOL", ")", },
    { "[p-without]", "ID", ",", },
    { "EOL", },
};


dynamicArrInt_t left_pars; // left analysis of program

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
int ll_tableFind(char *nonterm, char *term)
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


    printf("nonterm_idx: %d\tterm_idx: %d\n", nonterm_idx, term_idx);

    // return value from corresponding bucket in ll_table
    return ll_table[nonterm_idx][term_idx];
}


/**
 * @brief Parser - Syntactic analysis
 * 
 * @return int return value
 */
int parser(dynamicStr_t *sc_str, queue_t *que, symtable_t *symtable)
{
    printf("Parser started\n"); 
    /// INIT STRUCTURES
    if ( ! dynamicArrInt_init(&left_pars))
        return ERR_INTERNAL;
    stack_t *stack = stc_create();
    if (stack == NULL)
        goto err_internal_1;


    token_t *token;
    token_info_t info = { .ptr = NULL };

    printf("Initial push\n");
    /// INITIAL PUSH of EOF and starting nonterminal
    token = createToken("EOF", info);
    if (token == NULL)
        goto err_internal_2;
    stc_push(stack, token); // error cannot occur
     
    token = createToken("[st-list]", info);
    if (token == NULL)
        goto err_internal_2;
    stc_push(stack, token); // error cannot occur

    /////////////////////////////////////
    ///            MAIN LOOP          ///
    /////////////////////////////////////
    token_t *top; 
    token_t *act;

    token_t *id_tmp;

    bool succ = false;
    bool fail = false;
    bool get = true;
    int rule;
   
    do {
        //////////////////////
        //usleep(1 * 100000);
        //////////////////////

        top = stc_top(stack);
        if (get)
        {   
            act = scanner_get(sc_str, que, symtable);
            printf("scanner_get: %s\n", act->name);
            
            if (strcmp(act->name, "ERR_LEX") == 0)
                goto err_lexical;
            else if (strcmp(act->name, "ERR_INTERNAL") == 0)
                goto err_internal_2;
            
            get = false;
        }


        if (strcmp(top->name, "EOF") == 0)
        {
            if (strcmp(act->name, "EOF") == 0)
            {
                printf("EOF reached on both stack and scanner\n");
                succ = true;
            }
            else
            {
                printf("EOF reached on stack but not from scanner\n");
                fail = true;
            }
        }
        else if (isTerminal(top->name))
        {
            if (strcmp(top->name, "**expr**") == 0)
            {
                printf("********** EXPR **********\n");
                // destroy token: "**expr**"
                token = stc_pop(stack);
                destroyToken(token);

                scanner_unget(que, act);
                // SPUSTENIE PRECEDENCNEJ ANALYZY 
                //////////////////////////////////////////////////
                do {
                    act = scanner_get(sc_str, que, symtable);
                    printf("expr handling: %s\n", act->name);
                } while(strcmp(act->name, "then") != 0 && strcmp(act->name, "do") != 0 && strcmp(act->name, "EOL") != 0 && strcmp(act->name, "EOF") != 0);

                scanner_unget(que, act);
                /////////////////////////////////////////////////
                get = true;

                printf("********** END ***********\n");
            }
            else if (strcmp(top->name, act->name) == 0)
            {
                printf("top == act: %s\n", act->name);
                
                token = stc_pop(stack);
                destroyToken(token);
                
                get = true;
            }
            else
            {
                printf("top != name ... top: %s\tact: %s\n", top->name, act->name);
                fail = true;
            }
                
        }
        else
        {
            rule = ll_tableFind(top->name, act->name);
            if (rule == 10) // ????????????
                id_tmp = createToken("ID", act->info); 
 
            if (rule == EXPR_INCLUDE)
            {
                printf("********** EXPR **********\n");
                // destroy token: "[command]"
                token = stc_pop(stack);
                destroyToken(token);

                scanner_unget(que, act);
                // SPUSTENIE PRECEDENCNEJ ANALYZY 
                //////////////////////////////////////////
                do {
                    act = scanner_get(sc_str, que, symtable);
                    printf("expr handling: %s\n", act->name);
                } while(strcmp(act->name, "EOL") != 0 && strcmp(act->name, "EOF") != 0);

                scanner_unget(que, act);
                ////////////////////////////////////////

                printf("********** END ***********\n");
                get = true;
            }
            else if (rule == EXPR_INCLUDE_TWO)
            {
                printf("********** EXPR **********\n");
                // destroy token: "[func-assign-expr]"
                token = stc_pop(stack);
                destroyToken(token);
                
                scanner_unget(que, id_tmp);
                scanner_unget(que, act);
                // SPUSTENIE PRECEDENCNEJ ANALYZY
                //////////////////////////////////////////
                do {
                    act = scanner_get(sc_str, que, symtable);
                    printf("expr handling: %s\n", act->name);
                } while(strcmp(act->name, "EOL") != 0 && strcmp(act->name, "EOF") != 0);

                scanner_unget(que, act);
                /////////////////////////////////////////

                printf("********** END ***********\n");
                get = true;
            }
            else if (rule) // in case rule == 0 -- fail
            {
                printf("top: %s\tact: %s\n", top->name, act->name);
                token = stc_pop(stack);
                destroyToken(token);

                int i = 0;
                while (reverted_rules[rule][i] != NULL)
                {
                    token = createToken(reverted_rules[rule][i], info);
                    if (token == NULL)
                        goto err_internal_2;
                    if ( ! stc_push(stack, token))
                        goto err_internal_3;

                    i++;
                }
                
                if ( ! dynamicArrInt_add(&left_pars, rule))
                    goto err_internal_2;                
            }
            else
            {
                printf("no corresponding rule ... top: %s\tact: %s\n", top->name, act->name);
                fail = true;
            } 
        }

    } while (succ == false && fail == false);

    if (fail)     
        goto err_syntactic;
    

    // NIECO SPRAV S left_pars
    

    stc_destroy(stack);
    dynamicArrInt_free(&left_pars);
    return SUCCESS;

///////////////////////////////////////
///         ERROR HANDLING          ///
///////////////////////////////////////
err_internal_1:
    dynamicArrInt_free(&left_pars);
    error_msg("internal\n");
    return ERR_INTERNAL;

err_internal_2:
    dynamicArrInt_free(&left_pars);
    stc_destroy(stack);
    error_msg("internal\n");
    return ERR_INTERNAL;

err_internal_3:
    destroyToken(token);
    dynamicArrInt_free(&left_pars);
    stc_destroy(stack);
    error_msg("internal\n");
    return ERR_INTERNAL;

err_lexical:
    dynamicArrInt_free(&left_pars);
    stc_destroy(stack);
    error_msg("lexical\n");
    return ERR_LEX;

err_syntactic:
    dynamicArrInt_free(&left_pars);
    stc_destroy(stack);
    error_msg("syntactic\n");
    return ERR_SYN;
}



////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////

