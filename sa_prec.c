/**CFile****************************************************************

  FileName    [sa_prec.c]

  SystemName  [IFJ - PROJECT]

  PackageName [Syntactic analysis]

  Synopsis    []

  Author      [Lukas Piwowarski, Lukas Valek]

  Affiliation []

  Date        [01/11/2018]

  Revision    []

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                          INCLUDES                                ///
////////////////////////////////////////////////////////////////////////
#include "token.h"
#include "scanner.h"
#include "sa_prec.h"
#include "stack_sa_prec.h"
#include <string.h>
#include <stdbool.h>

////////////////////////////////////////////////////////////////////////
///                       GLOBAL VARIABLES                           ///
////////////////////////////////////////////////////////////////////////
#define INVALID_TOKEN -1

char sa_prec_table[PREC_TABLE_ROWS][PREC_TABLE_COLS] = {
  /*  +    *    id   $*/  
    {'>', '<', '<', '>'}, // +
    {'>', '<', '<', '>'}, // *
    {'>', '>', 'X', '>'}, // id
    {'<', '<', '<', 'X'}  // $
};

////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
////////////////////////////////////////////////////////////////////////

/**
 * @brief  If token indicates end of expression (EOL, then, do)
 *
 * @param  token  Token to be analysed
 * @return true   If token is 'EOL', 'then' or 'do'
 *         false  If token is NOT 'EOL', 'then' or 'do'
 */
bool sa_isEndToken(token_t *token)
{
    if(strcmp(token->name, "EOL") == 0)
        return true;
    else if(strcmp(token->name, "then") == 0)
        return true;
    else if(strcmp(token->name, "do") == 0)
        return true;

    return false;
}

char sa_getTokenIndex(token_t *token)
{
    if(strcmp(token->name, "+") == 0)
        return _plus_;
    else if(strcmp(token->name, "*") == 0)
        return _mult_;
    else if(strcmp(token->name, "id") == 0)
        return _id_;

    return INVALID_TOKEN;
}
/* 
 * @brief Operator-precedence parser
 *
 * @params  sc_str    Param for scanner
 *          que       Param for scanner
 *          symtable  Param for scanner
 * @return  true      If analysed expression is correct  
 *          false     If analysed expression is incorrect 
 */   
bool sa_prec(dynamicStr_t *sc_str, queue_t *que, symtable_t *symtable)
{
    stack_sa_t *stack = stc_init();
    stc_push(stack, _empt_);

    token_t *token = NULL;  
    int token_term = 0;
    int stack_top_term = 0;
    char rule = 0;
    int term = 0;
    while((token = scanner_get(sc_str, que, symtable)))
    {
        token_term = sa_getTokenIndex(token);
        stack_top_term = stc_topTerm(stack);
        rule = sa_prec_table[stack_top_term][token_term];

        if(rule == '=')
        {
            stc_push(stack, token_term);
            continue;
        }
        else if(rule == '<')
        {
            stc_pushAfter(stack, stack_top_term, _sml_);
            stc_push(stack, token_term);
            continue;
        }
        else if(rule == '>')
        {
            switch(stack_top_term)
            {
                /* E -> i */
                case _id_:
                    term = stc_popTop(stack);
                    if(term != _id_)
                        return false;

                    term = stc_popTop(stack);
                    if(term != _sml_)
                        return false;

                    stc_push(stack, _E_);

                    break;

                case _plus_:
                    term = stc_popTop(stack);
                    if(term != _E_)
                        return false;

                    term = stc_popTop(stack);
                    if(term != _mult_)
                        return false;

                    term = stc_popTop(stack);
                    if(term != _E_)
                        return false;  

                    term = stc_popTop(stack);
                    if(term != _sml_)
                        return false;

                    break;


            } 
        }

        if(sa_isEndToken(token))
            break;
    }


    stc_destroy(stack);
    return true;
}

////////////////////////////////////////////////////////////////////////