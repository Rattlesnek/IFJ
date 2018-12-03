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

#include <stdbool.h>

#include "parser.h"

#include "scanner.h"
#include "sa_prec.h"

#include "dynamic_arr_param.h"
#include "dynamic_str.h"
#include "stack_str.h"
#include "stack_tkn.h"
#include "queue.h"
#include "list.h"

#include "token.h"
#include "symtable.h"

#include "parser_gen.h"

#include "error.h"

#ifdef PARSER_PRINT
#define PARSER_DBG_PRINT(...) do{ printf( __VA_ARGS__ ); } while(0)
#else
#define PARSER_DBG_PRINT(...) do{ } while(0)
#endif

////////////////////////////////////////////////////////////////////////
///                       GLOBAL VARIABLES                           ///
////////////////////////////////////////////////////////////////////////

// LL-Grammar table
int ll_table[LL_ROWS][LL_COLS] = {
    // if/els/elif/end/whi/def/EOL/EOF/ =/ (/  )/  ,/  ID/ FUNC/expr
    {   1,  0,  0,  0,  1,  1,  2,  3,  0,  1,  0,  0,  1,  1,  1,  },  // [st-list]
    {   0,  0,  0,  0,  0,  0,  4,  5,  0,  0,  0,  0,  0,  0,  0,  },  // [EOF-EOL]
    {   7,  0,  0,  0,  7,  6,  0,  0,  0,  7,  0,  0,  7,  7,  7,  },  // [stat]
    {   9,  0,  0,  0,  8,  0,  0,  0,  0,  0,  0,  0,  10, 0,  0,  },  // [command]
    {   0,  0,  0,  0,  0,  0,  0,  0,  11, 0,  0,  0,  0,  0,  0,  },  // [func-assign-expr]
    {   12, 0,  0,  14, 12, 0,  13, 0,  0,  12, 0,  0,  12, 12, 12, },  // [end-list]
    {   15, 18, 17, 19, 15, 0,  16, 0,  0,  15, 0,  0,  15, 15, 15, },  // [if-list]
    {   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  20, 21, 0,  },  // [id-func]
    {   0,  0,  0,  0,  0,  0,  23, 0,  0,  22, 0,  0,  0,  0,  0,  },  // [params-gen]
    {   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  25, 0,  24, 0,  0,  },  // [p-brackets]
    {   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  27, 26, 0,  0,  0,  }   // [p-brackets-cont]
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
    /*COMMAND_8*/           { "[end-list]", "EOL", "do", "while", },
    /*COMMAND_9*/           { "[if-list]", "EOL", "then", "if", },
    /*COMMAND_10*/          { "[func-assign-expr]", "ID", },
    /*FUNC_ASSIGN_EXPR_11*/ { "=", },
    /*END_LIST_12*/         { "[end-list]", "EOL", "[command]", },
    /*END_LIST_13*/         { "[end-list]", "EOL", },
    /*END_LIST_14*/         { "end", },
    /*IF_LIST_15*/          { "[if-list]", "EOL", "[command]", },
    /*IF_LIST_16*/          { "[if-list]", "EOL", },
    /*IF_LIST_17*/          { "[if-list]", "EOL", "then", "elsif", },
    /*IF_LIST_18*/          { "[end-list]", "EOL", "else", },
    /*IF_LIST_19*/          { "end", },
    /*ID_FUNC_20*/          { "ID", },
    /*ID_FUNC_21*/          { "FUNC", },
    /*PARAMS_GEN_22*/       { "[p-brackets]", "(", },
    /*PARAMS_GEN_23*/       { "EOL", },
    /*P_BRACKETS_24*/       { "[p-brackets-cont]", "ID", },
    /*P_BRACKETS_25*/       { "EOL", ")", },
    /*P_BRACKETS_CONT_26*/  { "[p-brackets-cont]", "ID", ",", },
    /*P_BRACKETS_CONT_27*/  { "EOL", ")", }
};

////////////////////////////////////////////////////////////////////////
///                      MACRO DEFINITIONS                           ///
////////////////////////////////////////////////////////////////////////

#define HANDLE_ERROR(ret_val) do {                                      \
        switch (ret_val)                                                \
        {                                                               \
            case SUCCESS:       break;                                  \
            case ERR_LEX:       goto err_lexical;                       \
            case ERR_SYN:       goto err_syntactic_precedenc;           \
            case ERR_SEM_UNDEF: goto err_sem_undef;                     \
            case ERR_SEM_TYPE:  goto err_sem_type;                      \
            case ERR_SEM_FUNC:  goto err_sem_func;                      \
            case ERR_SEM_OTHER: goto err_sem_other;                     \
            case ERR_ZERO_DIV:  goto err_zero_div;                      \
            case ERR_INTERNAL:  goto err_internal_main;                 \
            default:            break;                                  \
        }                                                               \
    } while(0)


////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
////////////////////////////////////////////////////////////////////////

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
    else if (strcmp(term, "elsif") == 0)
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
    else if (isExpr(term)) // has to be behind strcmp(term, "FUNC") and behind strcmp(term, "(") otherwise wont work
        term_idx = EXPR_term;

    //PARSER_DBG_PRINT("nonterm_idx: %d\tterm_idx: %d\n", nonterm_idx, term_idx);

    // return value from corresponding bucket in ll_table
    return ll_table[nonterm_idx][term_idx];
}


bool tempKey_create(char **key_tmp, char *name)
{
    *key_tmp = malloc( (strlen(name) + 1) * sizeof(char) );
    if (*key_tmp == NULL)
        return false;
    strcpy(*key_tmp, name);

    return true;
}


void tempKey_destroy(char **key_tmp)
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


void freeAll(symtable_t *gl_var_tab, symtable_t *fun_tab, symtable_t *lc_var_tab,
             dynamicArrParam_t *param_arr,
             char **id_key_tmp, char **func_key_tmp,
             token_t *act, token_t *token, token_t *id_token_tmp, token_t *sa_prec_ret)
{
    symtab_free(gl_var_tab);
    symtab_free(fun_tab);
    if (lc_var_tab != NULL)
        symtab_free(lc_var_tab);

    dynamicArrParams_free(param_arr);

    tempKey_destroy(id_key_tmp);
    tempKey_destroy(func_key_tmp);

    destroyToken(act);
    destroyToken(token);
    destroyToken(id_token_tmp);
    destroyToken(sa_prec_ret);
}



#ifdef PARSER_PRINT
bool print_fun(elem_t *element)
{
    PARSER_DBG_PRINT("%s, ", element->func.key);
    return true;
}

bool print_fun_info(elem_t *element)
{
    PARSER_DBG_PRINT("key %s\n", element->func.key);
    PARSER_DBG_PRINT("defined %d\n", element->func.is_defined);
    PARSER_DBG_PRINT("n_params %d\n", element->func.n_params);

    return true;
}

bool print_var(elem_t *element)
{
    PARSER_DBG_PRINT("%s, ", element->var.key);
    return true;
}
#endif


#ifdef DEBUG_PARSER
int prec_tmp(dynamicStr_t *sc_str, queue_t *que)
{
    token_t *act = scanner_get(sc_str, que);
    PARSER_DBG_PRINT("expr handling: %s\n", act->name);
    if (strcmp(act->name, "then") != 0 && strcmp(act->name, "do") != 0 && strcmp(act->name, "EOL") != 0 && strcmp(act->name, "EOF") != 0)
    {
        do {
            destroyToken(act);
            act = scanner_get(sc_str, que);
            PARSER_DBG_PRINT("expr handling: %s\n", act->name);
        } while (strcmp(act->name, "then") != 0 && strcmp(act->name, "do") != 0 && strcmp(act->name, "EOL") != 0 && strcmp(act->name, "EOF") != 0);
    }
    scanner_unget(que, act, sc_str->str);

    return 0;
}
#endif


////////////////////////////////////////////////////////////////////////
///                             PARSER                               ///
////////////////////////////////////////////////////////////////////////
int parser(stack_tkn_t *stack_tkn, stack_str_t *stack_str, list_t *code_buffer, list_t *defvar_buffer, dynamicStr_t *sc_str, queue_t *que)
{
    PARSER_DBG_PRINT("Parser started\n");

    // PRINT PROLOG OF GENERATED CODE
    print_prolog();

    // DEFINE LOCAL VARIABLES
    token_t *top = NULL;    // token form the top of the stack
    token_t *act = NULL;    // token from scanner

    bool succ = false;      // parser success

    bool get_new_token = true;      // get new token from scanner
    bool get_func_params = false;   // load funciton parameters

    int rule;       // ll-grammar rule which is going to be used
    int ret_val;    // return value of precedenc analysis

    token_t *sa_prec_ret = NULL; // TODO

    char *id_key_tmp = NULL;        // temporary key of an identifier
    token_t *id_token_tmp = NULL;   // temporary token of an identifier

    char *func_key_tmp = NULL;      // temporary key of a function
    int param_cnt;                  // number of parameters of a function
    dynamicArrParam_t *param_arr = NULL;  // array where parameters of a function are stored

    char label_stat[40] = "\0";
    bool in_stat = false;

    // INIT SYMTABLES
    symtable_t *fun_tab;            // function symtable
    symtable_t *var_tab;            // general variables symtable (either global or local)
    symtable_t *gl_var_tab;         // global variables symtable
    symtable_t *lc_var_tab = NULL;  // local variables symtable

    gl_var_tab = symtab_init(NULL, VARIABLES);
    if (gl_var_tab == NULL)
        return ERR_INTERNAL;
    ////////////////////////////////////
    gl_var_tab->name = malloc( (strlen("$GT") + 1) * sizeof(char));
    strcpy(gl_var_tab->name, "$GT");
    ////////////////////////////////////
    var_tab = gl_var_tab;
    fun_tab = symtab_init(NULL, FUNCTIONS);
    if (fun_tab == NULL)
        goto err_internal_1;

    /// INITIAL PUSH of EOF and starting nonterminal
    token_t *token;
    token_info_t info = { .ptr = NULL };

    token = createToken("EOF", info);
    if (token == NULL)
        goto err_internal_2;
    stcTkn_push(stack_tkn, token); // error cannot occur

    token = createToken("[st-list]", info);
    if (token == NULL)
        goto err_internal_2;
    stcTkn_push(stack_tkn, token); // error cannot occur

    token = NULL;

    ////////////////////////////////////////////
    ///               MAIN LOOP              ///
    ////////////////////////////////////////////
    do {
        // get token from the top of the stack
        top = stcTkn_top(stack_tkn);
        if (get_new_token)
        {
            // if needed get new token from scanner
            act = scanner_get(sc_str, que);
            PARSER_DBG_PRINT("scanner_get: %s\n", act->name);
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
                PARSER_DBG_PRINT("EOF reached on both stack and scanner\n");
                succ = true;
            }
            else
            {
                PARSER_DBG_PRINT("EOF reached on stack but not from scanner\n");
                goto err_syntactic_predictive;
            }
        }
        else if (isTerminal(top->name))
        {
            if (strcmp(top->name, act->name) == 0)
            {
                PARSER_DBG_PRINT("top == act: %s\n", act->name);
                if (strcmp(top->name, "if") == 0)
                {
                    PARSER_DBG_PRINT("*********** IF ***********\n");
                    ///////////////////
                    //  GENERATE IF  //
                    ///////////////////
                    expected_LABEL_if(in_stat, label_stat);

#ifdef DEBUG_PARSER
                    ret_val = prec_tmp(sc_str, que);
#else
                    ret_val = sa_prec(sc_str, que, var_tab, fun_tab, &sa_prec_ret, code_buffer, in_stat);
                    HANDLE_ERROR(ret_val);

                    PARSER_DBG_PRINT("Returned Token: %s\n", sa_prec_ret->name);
#endif
                    in_stat = true;
                    if (! generate_if_condition(code_buffer, in_stat, stack_str, sa_prec_ret))
                        goto err_internal_main;

                    destroyToken(sa_prec_ret);
                    sa_prec_ret = NULL;

                    PARSER_DBG_PRINT("********** END ***********\n");
                }
                else if (strcmp(top->name, "elsif") == 0)
                {
                    PARSER_DBG_PRINT("*********** ELIF ***********\n");

                    if (! generate_LABEL_elsif(code_buffer, in_stat, stack_str))
                        goto err_internal_main;

#ifdef DEBUG_PARSER
                    ret_val = prec_tmp(sc_str, que);
#else
                    ret_val = sa_prec(sc_str, que, var_tab, fun_tab, &sa_prec_ret, code_buffer, in_stat);
                    HANDLE_ERROR(ret_val);

                    PARSER_DBG_PRINT("Returned Token: %s\n", sa_prec_ret->name);
#endif
                    if (! generate_elsif_condition(code_buffer, in_stat, sa_prec_ret))
                        goto err_internal_main;

                    destroyToken(sa_prec_ret);
                    sa_prec_ret = NULL;

                    get_new_token = true;
                    PARSER_DBG_PRINT("********** END ***********\n");
                }
                else if (strcmp(top->name, "else") == 0)
                {
                    PARSER_DBG_PRINT("*********** ELSE ***********\n");

                    if (! generate_else(code_buffer, in_stat, stack_str))
                        goto err_internal_main;

                    PARSER_DBG_PRINT("********** END ***********\n");
                }
                else if (strcmp(top->name, "while") == 0)
                {
                    PARSER_DBG_PRINT("********* WHILE **********\n");
                    //////////////////////
                    //  GENERATE WHILE  //
                    //////////////////////
                    expected_LABEL_while(in_stat, label_stat);
                    in_stat = true;
                    if (! generate_LABEL_while(code_buffer, in_stat))
                        goto err_internal_main;

#ifdef DEBUG_PARSER
                    ret_val = prec_tmp(sc_str, que);
#else
                    ret_val = sa_prec(sc_str, que, var_tab, fun_tab, &sa_prec_ret, code_buffer, in_stat);
                    HANDLE_ERROR(ret_val);

                    PARSER_DBG_PRINT("Returned Token: %s\n", sa_prec_ret->name);
#endif

                    if (! generate_while_condition(code_buffer, in_stat, sa_prec_ret))
                        goto err_internal_main;

                    // push to stack_tkn "epilog of while"
                    if (! generate_while_ending(stack_str))
                        goto err_internal_main;

                    destroyToken(sa_prec_ret);
                    sa_prec_ret = NULL;

                    //printf("in_stat: %d\n", in_stat);
                    PARSER_DBG_PRINT("********** END ***********\n");
                }
                else if (strcmp(top->name, "=") == 0)
                {
                    PARSER_DBG_PRINT("********** EXPR **********\n");
                    /////////////////////////
                    //  GENERATE VARIABLE  //
                    /////////////////////////
#ifdef DEBUG_PARSER
                    ret_val = prec_tmp(sc_str, que);
#else
                    ret_val = sa_prec(sc_str, que, var_tab, fun_tab, &sa_prec_ret, code_buffer, in_stat);
                    HANDLE_ERROR(ret_val);

                    if (sa_prec_ret == NULL)
                        PARSER_DBG_PRINT("token is NULL\n");
                    else if (sa_prec_ret->name == NULL)
                        PARSER_DBG_PRINT("token->name is NULL\n");
                    PARSER_DBG_PRINT("Returned Token: %s\n", sa_prec_ret->name);
                    //PARSER_DBG_PRINT("Returned Token val: %s\n", sa_prec_ret->info.string);
#endif

                    ret_val = generate_var(code_buffer, defvar_buffer, in_stat, var_tab, id_key_tmp, sa_prec_ret); // TODO
                    HANDLE_ERROR(ret_val);

                    // check if same name isnt used for function
                    if (symtab_find(fun_tab, id_key_tmp) != NULL)
                        goto err_sem_undef;
                    if (symtab_elem_add(var_tab, id_key_tmp) == NULL)
                        goto err_internal_main;

                    tempKey_destroy(&id_key_tmp);
                    destroyToken(sa_prec_ret);
                    sa_prec_ret = NULL;

                    PARSER_DBG_PRINT("********** END ***********\n");
                }
                else if (strcmp(act->name, "end") == 0)
                {
                    ////////////////////////////////
                    //  STACK POP GENERATED CODE  //
                    ////////////////////////////////

                    char *generated_code = stcStr_top(stack_str);
                    //printf("=== generated_code ===\n%s================\n", generated_code);
                    //printf("=== label_stat ===\n%s================\n", label_stat);
                    if (isFunctionEnd(generated_code))
                    {
#ifdef PARSER_PRINT
                        PARSER_DBG_PRINT("LOCAL TABLE: ");
                        symtab_foreach(lc_var_tab, print_var);
                        PARSER_DBG_PRINT("\n\n");
#endif
                        // return value TODO
                        //printf("MOVE LF@%%retval LF@%s\n", "something");

                        symtab_free(lc_var_tab);
                        lc_var_tab = NULL;
                        var_tab = gl_var_tab;
                    }
                    else if (strncmp(generated_code, "\nJUMP $endif$", strlen("\nJUMP $endif$")) == 0)
                    {
                        //printf("%s", generated_code);
                        print_or_append(code_buffer, in_stat, "%s", generated_code);
                        stcStr_pop(stack_str);
                        print_or_append(code_buffer, in_stat, "%s", stcStr_top(stack_str));
                        stcStr_pop(stack_str);
                        generated_code = stcStr_top(stack_str);
                    }

                    //if (strncmp(generated_code, "LABEL $endif$", strlen("LABEL $endif$")) == 0)
                    //    inc_endif();
                    //printf("%s", generated_code);
                    //printf("%s", label_stat);

                    if (strlen(label_stat) != 0 && strncmp(generated_code, label_stat, strlen(label_stat)) == 0)
                    {
                        list_print_clean(defvar_buffer);
                        list_print_clean(code_buffer);
                        in_stat = false;
                    }
                    print_or_append(code_buffer, in_stat, "%s", generated_code);

                    stcStr_pop(stack_str);
                }


                if (get_func_params)
                {
                    if (strcmp(act->name, "ID") == 0)
                    {
                        if (param_cnt >= 0)
                        {
                            // check if same name is not used for function or other parameter
                            if (symtab_find(fun_tab, sc_str->str) != NULL || symtab_find(lc_var_tab, sc_str->str) != NULL)
                                goto err_sem_undef;
                            elem_t *param = symtab_elem_add(lc_var_tab, sc_str->str);
                            if (param == NULL)
                                goto err_internal_main;
                            if (! dynamicArrParams_add(param_arr, param))
                                goto err_internal_main;
                        }
                        param_cnt++;
                    }
                    else if (strcmp(act->name, "EOL") == 0)
                    {
                        /////////////////////////
                        //  GENERATE FUNCTION  //
                        /////////////////////////
                        elem_t *fun = symtab_elem_add(fun_tab, func_key_tmp);
                        if (fun == NULL)
                            goto err_internal_main;

                        // check if same name was not used for other function
                        if (fun->func.is_defined == false)
                        {
                            // check if number of parameters is set to default value
                            if (fun->func.n_params == UNDEF_NO_PARAMS)
                            {
                                fun->func.is_defined = true;
                                fun->func.n_params = param_cnt;
                            }
                            else
                            {
                                // in this else branch we know that function was called before in the body of another function
                                // if number of parameters is not equal, function call was wrong
                                if (fun->func.n_params == param_cnt)
                                    fun->func.is_defined = true;
                                else
                                    goto err_sem_func;
                            }
                        }
                        else
                            goto err_sem_undef;


                        tempKey_destroy(&func_key_tmp);

                        // GENERATE FUNCTION PROLOG
                        if (! generate_function(stack_str, fun, param_arr))
                            goto err_internal_main;

                        dynamicArrParams_free(param_arr);
                        param_arr = NULL;

                        get_func_params = false;
                    }
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
                PARSER_DBG_PRINT("top != name ... top: %s\tact: %s\n", top->name, act->name);
                goto err_syntactic_predictive;
            }

        }
        else
        {
            rule = LLtableFind(top->name, act->name);
            if (rule == COMMAND_10) // [command] -> ID [func-assign-expr] ????????????
            {
                if ( ! tempKey_create(&id_key_tmp, sc_str->str))
                    goto err_internal_main;

                if (strcmp(act->name, "BUILTIN") == 0)
                {
                    info.string = malloc( (strlen(act->info.string) + 1) * sizeof(char) );
                    strcpy(info.string, act->info.string);
                }
                else
                    info.ptr = NULL;

                id_token_tmp = createToken(act->name, info);
                PARSER_DBG_PRINT("id_tmp load %s\n", id_key_tmp);
            }
            else if (rule == ID_FUNC_20 || rule == ID_FUNC_21)
            {
                // check if same name isnt used global variable
                if (symtab_find(gl_var_tab, sc_str->str) != NULL)
                    goto err_sem_undef;

                if ( ! tempKey_create(&func_key_tmp, sc_str->str))
                    goto err_internal_main;

                // HACK :D
                param_cnt = (strcmp(act->name, "ID") == 0) ? -1 : 0;
                param_arr = dynamicArrParams_init();
                if (param_arr == NULL)
                    goto err_internal_main;

                PARSER_DBG_PRINT("create local hash table for function\n");
                if (lc_var_tab == NULL) // TODO
                {
                    lc_var_tab = symtab_init(func_key_tmp, VARIABLES);
                    var_tab = lc_var_tab;
                }

                get_func_params = true;
            }

            // need to be if (not else if) !!!
            if (rule == EXPR_INCLUDE)
            {
                PARSER_DBG_PRINT("********** EXPR **********\n");
                token = stcTkn_pop(stack_tkn);
                destroyToken(token);
                token = NULL;

                scanner_unget(que, act, sc_str->str);
                PARSER_DBG_PRINT("Unget Token: %s\n", act->name);
                PARSER_DBG_PRINT("Unget sc_str: %s\n", sc_str->str);
                act = NULL;
#ifdef DEBUG_PARSER
                ret_val = prec_tmp(sc_str, que);
#else
                ret_val = sa_prec(sc_str, que, var_tab, fun_tab, &sa_prec_ret, code_buffer, in_stat);
                HANDLE_ERROR(ret_val);

                PARSER_DBG_PRINT("Returned Token: %s\n", sa_prec_ret->name);
#endif

                // expression TODO
                destroyToken(sa_prec_ret);
                sa_prec_ret = NULL;

                get_new_token = true;
                PARSER_DBG_PRINT("********** END ***********\n");
            }
            else if (rule == EXPR_INCLUDE_TWO)
            {

                PARSER_DBG_PRINT("********** EXPR **********\n");

                // destroy token: "[func-assign-expr]"
                token = stcTkn_pop(stack_tkn);
                destroyToken(token);
                token = NULL;

                scanner_unget(que, id_token_tmp, id_key_tmp);
                id_token_tmp = NULL;
                tempKey_destroy(&id_key_tmp);
                scanner_unget(que, act, sc_str->str);
                act = NULL;

#ifdef DEBUG_PARSER
                ret_val = prec_tmp(sc_str, que);
#else
                ret_val = sa_prec(sc_str, que, var_tab, fun_tab, &sa_prec_ret, code_buffer, in_stat);
                HANDLE_ERROR(ret_val);

                PARSER_DBG_PRINT("Returned Token: %s\n", sa_prec_ret->name);
#endif

                // expression TODO
                destroyToken(sa_prec_ret);
                sa_prec_ret = NULL;

                get_new_token = true;
                PARSER_DBG_PRINT("********** END ***********\n");
            }
            else if (rule) // in case rule == 0 -- fail
            {
                PARSER_DBG_PRINT("top: %s\tact: %s\n", top->name, act->name);
                token = stcTkn_pop(stack_tkn);
                destroyToken(token);
                token = NULL;

                if ( ! pushRevertedRules(stack_tkn, rule))
                    goto err_internal_main;
            }
            else
            {
                PARSER_DBG_PRINT("no corresponding rule ... top: %s\tact: %s\n", top->name, act->name);
                goto err_syntactic_predictive;
            }
        }

    } while (succ == false);
    /////////////////////////////////////////
    ///         END OF MAIN LOOP          ///
    /////////////////////////////////////////

    //symtab_foreach(fun_tab, print_fun_info);
    if (! symtab_foreach(fun_tab, checkFun))
        goto err_sem_undef;

#ifdef PARSER_PRINT
    PARSER_DBG_PRINT("\nGLOBAL TABLE:\n");
    symtab_foreach(gl_var_tab, print_var);
    PARSER_DBG_PRINT("\n\nFUNC TABLE:\n");
    symtab_foreach(fun_tab, print_fun);
    PARSER_DBG_PRINT("\n\n");
#endif

    // free all alocated elements
    freeAll(gl_var_tab, fun_tab, lc_var_tab, param_arr, &id_key_tmp, &func_key_tmp, act, token, id_token_tmp, sa_prec_ret);
    return SUCCESS;

////////////////////////////////////////////////////////////////////////
///                       ERROR HANDLING                             ///
////////////////////////////////////////////////////////////////////////
err_internal_1:
    symtab_free(gl_var_tab);
    error_msg("internal\n");
    return ERR_INTERNAL;

err_internal_2:
    symtab_free(gl_var_tab);
    symtab_free(fun_tab);
    error_msg("internal\n");
    return ERR_INTERNAL;

err_internal_main:
    freeAll(gl_var_tab, fun_tab, lc_var_tab, param_arr, &id_key_tmp, &func_key_tmp, act, token, id_token_tmp, sa_prec_ret);
    error_msg("internal\n");
    return ERR_INTERNAL;

err_lexical:
    freeAll(gl_var_tab, fun_tab, lc_var_tab, param_arr, &id_key_tmp, &func_key_tmp, act, token, id_token_tmp, sa_prec_ret);
    error_msg("lexical\n");
    return ERR_LEX;

err_syntactic_predictive:
    freeAll(gl_var_tab, fun_tab, lc_var_tab, param_arr, &id_key_tmp, &func_key_tmp, act, token, id_token_tmp, sa_prec_ret);
    error_msg("syntactic - predictive analysis\n");
    return ERR_SYN;

err_syntactic_precedenc:
    freeAll(gl_var_tab, fun_tab, lc_var_tab, param_arr, &id_key_tmp, &func_key_tmp, act, token, id_token_tmp, sa_prec_ret);
    error_msg("syntactic - precendec analysis\n");
    return ERR_SYN;

err_sem_undef:
    freeAll(gl_var_tab, fun_tab, lc_var_tab, param_arr, &id_key_tmp, &func_key_tmp, act, token, id_token_tmp, sa_prec_ret);
    error_msg("semantic - undefined/redefined variable or function\n");
    return ERR_SEM_UNDEF;

err_sem_type:
    freeAll(gl_var_tab, fun_tab, lc_var_tab, param_arr, &id_key_tmp, &func_key_tmp, act, token, id_token_tmp, sa_prec_ret);
    error_msg("semantic - type\n");
    return ERR_SEM_TYPE;

err_sem_func:
    freeAll(gl_var_tab, fun_tab, lc_var_tab, param_arr, &id_key_tmp, &func_key_tmp, act, token, id_token_tmp, sa_prec_ret);
    error_msg("semantic - wrong number of function parameters\n");
    return ERR_SEM_FUNC;

err_sem_other:
    freeAll(gl_var_tab, fun_tab, lc_var_tab, param_arr, &id_key_tmp, &func_key_tmp, act, token, id_token_tmp, sa_prec_ret);
    error_msg("semantic - other\n");
    return ERR_SEM_OTHER;

err_zero_div:
    freeAll(gl_var_tab, fun_tab, lc_var_tab, param_arr, &id_key_tmp, &func_key_tmp, act, token, id_token_tmp, sa_prec_ret);
    error_msg("zero division\n");
    return ERR_ZERO_DIV;
}


////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////

