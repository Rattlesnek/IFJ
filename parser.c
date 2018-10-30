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

#include "scanner.h"
#include "stack.h"
#include "token.h"
#include "symtable.h"
#include "dynamicArrInt.h"
#include "error.h"

#include "parser.h"

////////////////////////////////////////////////////////////////////////
///                       GLOBAL VARIABLES                           ///
////////////////////////////////////////////////////////////////////////

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
    else if (strcmp(nonterm, "[stat]") == 0)
        nonterm_idx = STAT_nonterm;
    else if (strcmp(nonterm, "[command]") == 0)
    {
        // special case
        if (strcmp(term, "+") == 0 ||
            strcmp(term, "-") == 0 ||
            strcmp(term, "(") == 0 ||
            strcmp(term, "FUNC") == 0 ||
            strcmp(term, "STR") == 0 ||
            strcmp(term, "INT") == 0 ||
            strcmp(term, "DBL") == 0 ||
            strcmp(term, "nil") == 0)
            return EXPR_INCLUDE; 
        
        nonterm_idx = COMMAND_nonterm;
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
    else if (strcmp(nonterm, "[func-assign-expr]") == 0)
    {
         // special case
        if (strcmp(term, "=") != 0)
            return EXPR_INCLUDE_TWO;

        nonterm_idx = FUNC_ASSIGN_EXPR_nonterm;
    }

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

    // return value from corresponding bucket in ll_table
    return ll_table[nonterm_idx][term_idx];
}


/**
 * @brief Parser - Syntactic analysis
 * 
 * @return int return value
 */
int pr_parser()
{
    /// INIT STRUCTURES
    if ( ! dynamicArrInt_init(&left_pars))
        return ERR_INTERNAL;
    stack_t *stack = stc_create();
    if (stack == NULL)
        goto err_internal_1;


    token_t *token;
    token_info_t info = { .ptr = NULL };

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
    int rule;
   
    do {
        top = stc_top(stack);
        act = sc_scanner();
        if (strcmp(act->name, "ERR_LEX") == 0)
            goto err_lexical;
        else if (strcmp(act->name, "ERR_INTERNAL") == 0)
            goto err_internal_2;


        if (strcmp(top->name, "EOF") == 0)
        {
            if (strcmp(act->name, "EOF") == 0)
                succ = true;
            else
                fail = true;
        }
        else if (isTerminal(top->name))
        {
            if (strcmp(top->name, "**expr**") == 0)
            {
                // destroy token: "**expr**"
                token = stc_pop(stack);
                destroyToken(token);
                
                // SPUSTENIE PRECEDENCNEJ ANALYZY 
                // definicia: int preced(token_t *token1, token_t *token2)

                // preced(NULL, NULL);
            }
            else if (strcmp(top->name, act->name) == 0)
            {
                token = stc_pop(stack);
                destroyToken(token);
            }
            else
                fail = true;
        }
        else
        {
            rule = ll_tableFind(top->name, act->name);
            // { "EOL", "[func-assign-expr]" "ID", }
            if (rule == 8)
                id_tmp = createToken("ID", act->info);

            if (rule == EXPR_INCLUDE)
            {
                // destroy token: "[command]"
                token = stc_pop(stack);
                destroyToken(token);

                // SPUSTENIE PRECEDENCNEJ ANALYZY 
                // preced(act, NULL);

            }
            else if (rule == EXPR_INCLUDE_TWO)
            {
                // destroy token: "[func-assign-expr]"
                token = stc_pop(stack);
                destroyToken(token);
                
                // SPUSTENIE PRECEDENCNEJ ANALYZY
                // preced(id_tmp, act);
            }
            else if (rule) // in case rule == 0 -- fail
            {
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
                
                if ( ! dynamicArrInt_add(&left_pars, rule)); 
                    goto err_internal_2;                
            }
            else 
                fail = true;
        }


    } while (succ || fail);

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
    return ERR_INTERNAL;

err_internal_2:
    dynamicArrInt_free(&left_pars);
    stc_destroy(stack);
    return ERR_INTERNAL;

err_internal_3:
    destroyToken(token);
    dynamicArrInt_free(&left_pars);
    stc_destroy(stack);
    return ERR_INTERNAL;

err_lexical:
    dynamicArrInt_free(&left_pars);
    stc_destroy(stack);
    return ERR_LEX;

err_syntactic:
    dynamicArrInt_free(&left_pars);
    stc_destroy(stack);
    return ERR_SYN;
}



////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////

