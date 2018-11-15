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
/*         +      *     (     )     i     -     /     rel    str    f     ,    $  */
/*  +  */{ '>' , '<' , '<' , '>' , '<' , '>' , '<' ,  '>' , '<' , '<' , '>' , '>' }, 
/*  *  */{ '>' , '>' , '<' , '>' , '<' , '>' , '>' ,  '>' , 'X' , '<' , '>' , '>' },
/*  (  */{ '<' , '<' , '<' , '=' , '<' , '<' , '<' ,  '<' , 'X' , '<' , '=' , 'X' },
/*  )  */{ '>' , '>' , 'X' , '>' , 'X' , '>' , '>' ,  '>' , 'X' , 'X' , '>' , '>' },
/*  i  */{ '>' , '>' , 'X' , '>' , 'X' , '>' , '>' ,  '>' , 'X' , 'X' , '>' , '>' },
/*  -  */{ '>' , '<' , '<' , '>' , '<' , '>' , '<' ,  '>' , 'X' , 'X' , '>' , '>' },
/*  /  */{ '>' , '<' , '<' , '>' , '<' , '>' , '>' ,  '>' , 'X' , 'X' , '>' , '>' },
/* rel */{ '<' , '<' , '<' , '>' , '<' , '<' , '<' ,  'X' , '<' , 'X' , '>' , '>' },
/* str */{ '>' , 'X' , 'X' , 'X' , 'X' , 'X' , 'X' ,  '>' , 'X' , 'X' , 'X' , '>' },
/*  f  */{ 'X' , 'X' , '=' , 'X' , 'X' , 'X' , 'X' ,  'X' , 'X' , 'X' , 'X' , 'X' },
/*  ,  */{ '<' , '<' , '<' , '=' , '<' , '<' , '<' ,  '<' , '<' , '<' , '=' , 'X' },
/*  $  */{ '<' , '<' , '<' , 'X' , '<' , '<' , '<' ,  '<' , '<' , '<' , 'X' , 'X' }
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
    printf("=> sa_getTokenIndex: %s\n", token->name);
    if(strcmp(token->name, "+") == 0)
        return _plus_;
    else if(strcmp(token->name, "*") == 0)
        return _mult_;
    else if(strcmp(token->name, "ID") == 0)
        return _id_;
    else if(strcmp(token->name, "INT") == 0)
        return _id_;
    else if(strcmp(token->name, "DBL") == 0)
        return _id_;
    else if(strcmp(token->name, "(") == 0)
        return _lbrc_;
    else if(strcmp(token->name, ")") == 0)
        return _rbrc_;
    else if(strcmp(token->name, "-") == 0)
        return _mins_;
    else if(strcmp(token->name, "/") == 0)
        return _div_;
    else if(strcmp(token->name, "STR") == 0)
        return _str_;
    else if(strcmp(token->name, "<=") == 0)
        return _rel_;
    else if(strcmp(token->name, ">=") == 0)
        return _rel_;
    else if(strcmp(token->name, "==") == 0)
        return _rel_;
    else if(strcmp(token->name, "!=") == 0)
        return _rel_;
    else if(strcmp(token->name, "<") == 0)
        return _rel_;
    else if(strcmp(token->name, ">") == 0)
        return _rel_;
    else if(sa_isEndToken(token))
        return _empt_;

    return INVALID_TOKEN;
}

static inline bool sa_isNonTerm(table_elem_t term)
{
    if(term == _E_)
        return true;
    else if(term == _L_)
        return true;
    else if(term == _S_)
        return true;

    return false;
}

static inline bool sa_detectSucEnd(stack_sa_t *stack, table_elem_t token)
{
    return (sa_isNonTerm(stack->top->term) 
            && stack->top->lptr->term == _empt_ 
            && token == _empt_);
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

    token_t *token = scanner_get(sc_str, que);  
    int token_term = 0;
    int stack_top_term = 0;
    char rule = 0;
    int term = 0;
    while(42)
    {
        stack_top_term = stc_topTerm(stack);
    
        token_term = sa_getTokenIndex(token);
#if 0
        if(token_term == _id_)
        {
            elem_t *elem = symtab_find(symtable, token->name);
            if(elem == NULL)
            {
                token_term = _func_;
                symtab_elem_add(symtable, "FUNC", sc_str->str);
            }
            else if()
        }
#endif

        printf("=> sa_prec: Token_term %d\n", token_term);
        printf("=> sa_prec: Stack topTerm %d\n", stack_top_term);
        rule = sa_prec_table[stack_top_term][token_term];
        printf("=> sa_prec: rule %c\n", rule);

        if(rule == 'X')
        {
            //printf("=> sa_prec: X rule\n");
            return false;
        }
        else if(rule == '=')
        {
            stc_push(stack, token_term);
        }
        else if(rule == '<')
        {
            stc_pushAfter(stack, stack_top_term, _sml_);
            stc_push(stack, token_term);
            stc_print(stack);
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

                /* S -> str */
                case _str_:
                    term = stc_popTop(stack);
                    if(term != _str_)
                        return false;

                    term = stc_popTop(stack);
                    if(term != _sml_)
                        return false;

                    stc_push(stack, _S_);
                    break;

                /* E -> E + E && E -> S + S */
                case _plus_:
                    term = stc_popTop(stack);
                    if(term == _E_)
                    {
                        term = stc_popTop(stack);
                        if(term != _plus_)
                            return false;

                        term = stc_popTop(stack);
                        if(term != _E_)
                            return false;  

                        term = stc_popTop(stack);
                        if(term != _sml_)
                            return false;

                        stc_push(stack, _E_);
                    }
                    else if(term == _S_) 
                    {
                        term = stc_popTop(stack);
                        if(term != _plus_)
                            return false;

                        term = stc_popTop(stack);
                        if(term != _S_)
                            return false;

                        term = stc_popTop(stack);
                        if(term != _sml_)
                            return false;

                        stc_push(stack, _S_);
                    }
                    else
                    {
                        return false;
                    }

                    break;

                /* E -> E * E */
                case _mult_:
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

                    stc_push(stack, _E_);
                    break;

                /* E -> (E) */
                case _rbrc_:
                    term = stc_popTop(stack);
                    if(term != _rbrc_)
                        return false;

                    term = stc_popTop(stack);
                    if(term != _E_)
                        return false;

                    term = stc_popTop(stack);
                    if(term != _lbrc_)
                        return false;

                    term = stc_popTop(stack);
                    if(term != _sml_)
                        return false;

                    stc_push(stack, _E_);
                    break;

                /* E -> E - E */
                case _mins_:
                    term = stc_popTop(stack);
                    if(term != _E_)
                        return false;

                    term = stc_popTop(stack);
                    if(term != _mins_)
                        return false;

                    term = stc_popTop(stack);
                    if(term != _E_)
                        return false;

                    term = stc_popTop(stack);
                    if(term != _sml_)
                        return false;

                    stc_push(stack, _E_);
                    break;

                /* E -> E / E */
                case _div_:
                    term = stc_popTop(stack);
                    if(term != _E_)
                        return false;

                    term = stc_popTop(stack);
                    if(term != _div_)
                        return false;

                    term = stc_popTop(stack);
                    if(term != _E_)
                        return false;

                    term = stc_popTop(stack);
                    if(term != _sml_)
                        return false;

                    stc_push(stack, _E_);
                    break;

                /* L -> E rel E && L -> S rel S */
                case _rel_:
                    term = stc_popTop(stack);

                    if(term == _E_)
                    {
                        term = stc_popTop(stack);
                        if(term != _rel_)
                            return false;

                        term = stc_popTop(stack);
                        if(term != _E_)
                            return false;

                        term = stc_popTop(stack);  
                        if(term != _sml_)
                            return false;
                    }
                    else if(term == _S_)
                    {

                        term = stc_popTop(stack);
                        if(term != _rel_)
                            return false;

                        term = stc_popTop(stack);
                        if(term != _S_)
                            return false;

                        term = stc_popTop(stack);  
                        if(term != _sml_)
                            return false;
                    }
                    else
                    {
                        return false;
                    }

                    stc_push(stack, _L_);
                    break;

            }
            stc_print(stack);
            
            if(sa_detectSucEnd(stack, token_term))
                return true;
            else
                continue;
        }
        stc_print(stack);

        if(sa_detectSucEnd(stack, token_term))
            return true;

        token = scanner_get(sc_str, que);
    }

    stc_destroy(stack);
    return true;
}

////////////////////////////////////////////////////////////////////////