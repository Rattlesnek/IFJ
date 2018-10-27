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

#include "parser.h"

////////////////////////////////////////////////////////////////////////
///                       GLOBAL VARIABLES                           ///
////////////////////////////////////////////////////////////////////////

dynamicArrInt_t left_pars; // left analysis of program

////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
////////////////////////////////////////////////////////////////////////

int ll_tableFind(char *nonterm, char *term)
{
    int nonterm_idx;
    int term_idx;

    if (strcmp(nonterm, "[st-list]") == 0)
        nonterm_idx = ST_LIST_nonterm;
    else if (strcmp(nonterm, "[stat]") == 0)
        nonterm_idx = STAT_nonterm;
    else if (strcmp(nonterm, "[command]") == 0)
        nonterm_idx = COMMAND_nonterm;
    else if (strcmp(nonterm, "[func-assign-expr]") == 0)
        nonterm_idx = FUNC_ASSIGN_EXPR_nonterm;
    else if (strcmp(nonterm, "[assign-expr]") == 0)
        nonterm_idx = ASSIGN_EXPR_nonterm;
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
    else if (strcmp(nonterm, "[id]") == 0)
        nonterm_idx = ID_nonterm;
    else 
    {
        printf("ERROR: nonterm_idx was not set");
        return 0;
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
    else if (strcmp(term, "nil") == 0)
        term_idx = NIL_term;
    else if (strcmp(term, "STR") == 0)
        term_idx = STR_term;
    else if (strcmp(term, "INT") == 0)
        term_idx = INT_term;
    else if (strcmp(term, "DBL") == 0)
        term_idx = DBL_term;
    else if (strcmp(term, "**expr**") == 0)
        term_idx = EXPR_term;
    else 
    {
        printf("ERROR: term_idx was not set");
        return 0;
    }

    // return value from corresponding bucket in ll_table
    return ll_table[nonterm_idx][term_idx];
}


int pr_parser()
{
    /// INIT STRUCTURES
    if ( ! dynamicArrInt_init(&left_pars))
        return 0; // TODO ERROR
    stack_t *stack = stc_create();
    if (stack == NULL)
        return 0; // TODO ERROR


    token_t *token;
    token_info_t info = { .ptr = NULL };

    /// INITIAL PUSH
    token = createToken("EOF", info);
    if (token == NULL)
        return 0; // TODO ERROR
    stc_push(stack, token);
    // TODO ERROR
    token = createToken("[st-list]", info);
    if (token == NULL)
        return 0; // TODO ERROR
    stc_push(stack, token);
    // TODO ERROR

    ///////////////////
    ///  MAIN LOPP  ///
    ///////////////////
    token_t *top; 
    token_t *act;

    bool succ = false;
    bool fail = false;
    int rule;
   
    do {
        top = stc_top(stack);
        act = sc_scanner();

        if (strcmp(top->name, "EOF") == 0)
        {
            if (strcmp(act->name, "EOF") == 0)
                succ = true;
            else
                fail = true;
        }
        else if (isTerminal(top->name))
        {
            if (strcmp(top->name, act->name) == 0)
            {
                token = stc_pop(stack);
                destroyToken(token);
                continue;
            }
            else
                fail = true;
        }
        else
        {
            rule = ll_tableFind(top->name, act->name);
            if (rule) // in case rule == 0 -- fail
            {
                token = stc_pop(stack);
                destroyToken(token);

                int i = 0;
                while (reverted_rules[rule][i] != NULL)
                {
                    token = createToken(reverted_rules[rule][i], info);
                    stc_push(stack, token);
                    i++;
                }
                
                dynamicArrInt_add(&left_pars, rule);                 
            }
            else 
                fail = true;
        }


    } while (succ || fail);
    

    stc_destroy(stack);
    return 0;
}



////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////

