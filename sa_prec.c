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
#include "error.h"
#include <string.h>
#include <stdbool.h>

////////////////////////////////////////////////////////////////////////
///                       GLOBAL VARIABLES                           ///
////////////////////////////////////////////////////////////////////////
#define INVALID_TOKEN -1
#define TEST_FUNC 1

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
/*  f  */{ 'X' , 'X' , '=' , 'X' , '<' , 'X' , 'X' ,  'X' , 'X' , 'X' , '<' , '>' },
/*  ,  */{ '<' , '<' , '<' , '=' , '<' , '<' , '<' ,  '<' , '<' , '<' , '=' , '>' },
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
    else if(strcmp(token->name, "EOF") == 0)
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
        return _id_;
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
    else if(strcmp(token->name, ",") == 0)
        return _coma_;
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
    else if(term == _F_)
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
int sa_prec(dynamicStr_t *sc_str, queue_t *que, symtable_t *loc_symtab, symtable_t *func_symtab)
{
    stack_sa_t *stack = stc_init();
    stc_push(stack, _empt_, NULL);

    token_t *token = scanner_get(sc_str, que);  
    int token_term = 0;
    int stack_top_term = 0;
    char rule = 0;
    int term = 0;
    char detect_func = 0;
    int num_params = 0;
    elem_t *loc_elem = NULL;
    elem_t *func_elem = NULL;
    int count = 0;
    char func_read = 0;
    while(42)
    {
        stack_top_term = stc_topTerm(stack);
    
        token_term = sa_getTokenIndex(token);


#if TEST_FUNC
        if(token_term == _id_ && (strcmp(token->name, "ID") == 0))
        {
            loc_elem = symtab_find(loc_symtab, sc_str->str);
            func_elem = symtab_find(func_symtab, sc_str->str);

            if(loc_elem != NULL)
            {
                token_term = _id_;
            }
            else if(func_elem != NULL)
            {
                if(detect_func == 1 || count != 0)
                    goto fail_end;

                token_term = _func_;
                detect_func = 1;
            }
            else
            {
                if(loc_symtab->name == NULL && func_symtab->name == NULL)
                    goto sem_fail_defined;

                if(detect_func == 1 || count != 0)
                    goto fail_end;

                func_elem = symtab_elem_add(func_symtab, sc_str->str);
                func_elem->func.is_defined = false;
                token_term = _func_;
                detect_func = 1;
            }

        }
        count++;
#endif

        printf("=> sa_prec: Token_term %d\n", token_term);
        printf("=> sa_prec: Stack topTerm %d\n", stack_top_term);
        rule = sa_prec_table[stack_top_term][token_term];
        printf("=> sa_prec: rule %c\n", rule);

        if(rule == 'X')
        {
            goto fail_end;
        }
        else if(rule == '=')
        {
            if(func_read)
                goto fail_end;

            stc_push(stack, token_term, token);

        }
        else if(rule == '<')
        {
            if(func_read)
                goto fail_end;

            stc_pushAfter(stack, stack_top_term, _sml_);
            stc_push(stack, token_term, token);
            stc_print(stack);
        }
        else if(rule == '>')
        {
            switch(stack_top_term)
            {
                /* E -> i */
                case _id_:
                    term = stc_Top(stack);
                    if(term != _id_)
                        goto fail_end;

                    token_t *token = stc_tokPopTop(stack);

                    term = stc_popTop(stack);
                    if(term != _sml_)
                        goto fail_end;

                    stc_push(stack, _E_, token);

                    if(detect_func)
                        num_params++;

                    break;

                /* E -> E + E */
                case _plus_:
                    term = stc_popTop(stack);
                    if(term != _E_)
                        goto fail_end;

                    term = stc_popTop(stack);
                    if(term != _plus_)
                        goto fail_end;

                    term = stc_popTop(stack);
                    if(term != _E_)
                        goto fail_end;

                    term = stc_popTop(stack);
                    if(term != _sml_)
                        goto fail_end;

                    stc_push(stack, _E_, NULL);
                    break;

                /* E -> E * E */
                case _mult_:
                    term = stc_popTop(stack);
                    if(term != _E_)
                        goto fail_end;

                    term = stc_popTop(stack);
                    if(term != _mult_)
                        goto fail_end;

                    term = stc_popTop(stack);
                    if(term != _E_)
                        goto fail_end;

                    term = stc_popTop(stack);
                    if(term != _sml_)
                        goto fail_end;

                    stc_push(stack, _E_, NULL);
                    break;

                /* 
                 * E -> (E)
                 * F -> f()
                 * F -> f(E)
                 * F -> f(E, ... , E)
                 */
                case _rbrc_:
                    term = stc_popTop(stack);
                    if(term != _rbrc_)
                        goto fail_end;

                    term = stc_popTop(stack);
                    if(term == _lbrc_)
                    {
                        term = stc_popTop(stack);
                        if(term != _func_)
                            goto fail_end;

                        term = stc_popTop(stack);
                        if(term != _sml_)
                            goto fail_end;

                        stc_push(stack, _F_, NULL);

                        if(!func_elem->func.is_defined)
                            func_elem->func.n_params = num_params;
                        else if(func_elem->func.n_params != num_params)
                            goto sem_fail;

                        func_read = 1;
                        break;
                    }
                    else if(term == _E_)
                    {
                        term = stc_popTop(stack);
                        if(term == _lbrc_)
                        {
                            term = stc_popTop(stack);
                            if(term == _func_)
                            {
                                term = stc_popTop(stack);
                                if(term != _sml_)
                                    goto fail_end;

                                stc_push(stack, _F_, NULL);

                                if(!func_elem->func.is_defined)
                                    func_elem->func.n_params = num_params;
                                else if(func_elem->func.n_params != num_params)
                                    goto sem_fail;
                                func_read = 1;
                                break;
                            }
                            else if(term == _sml_)
                            {
                                stc_push(stack, _E_, NULL);
                                break;
                            }
                        }
                        else if(term == _coma_)
                        {
                            while(42)
                            {
                                term = stc_popTop(stack);
                                if(term != _E_)
                                    goto fail_end;

                                term = stc_popTop(stack);
                                if(term == _lbrc_)
                                    break;
                                else if(term != _coma_)
                                    goto fail_end;
                            }

                            term = stc_popTop(stack);
                            if(term != _func_)
                                goto fail_end;

                            term = stc_popTop(stack);
                            if(term != _sml_)
                                goto fail_end;

                            stc_push(stack, _F_, NULL);

                            if(!func_elem->func.is_defined)
                                func_elem->func.n_params = num_params;
                            else if(func_elem->func.n_params != num_params)
                                goto sem_fail;

                            func_read = 1;
                            break;
                        }

                    }
                  
                    break;

                /* E -> E - E */
                case _mins_:
                    term = stc_popTop(stack);
                    if(term != _E_)
                        goto fail_end;

                    term = stc_popTop(stack);
                    if(term != _mins_)
                        goto fail_end;

                    term = stc_popTop(stack);
                    if(term != _E_)
                        goto fail_end;

                    term = stc_popTop(stack);
                    if(term != _sml_)
                        goto fail_end;

                    stc_push(stack, _E_, NULL);
                    break;

                /* E -> E / E */
                case _div_:
                    term = stc_popTop(stack);
                    if(term != _E_)
                        goto fail_end;

                    term = stc_popTop(stack);
                    if(term != _div_)
                        goto fail_end;

                    term = stc_popTop(stack);
                    if(term != _E_)
                        goto fail_end;

                    term = stc_popTop(stack);
                    if(term != _sml_)
                        goto fail_end;

                    stc_push(stack, _E_, NULL);
                    break;

                /* L -> E rel E
                 * L -> S rel S 
                 */
                case _rel_:
                    term = stc_popTop(stack);

                    if(term == _E_)
                    {
                        term = stc_popTop(stack);
                        if(term != _rel_)
                            goto fail_end;

                        term = stc_popTop(stack);
                        if(term != _E_)
                            goto fail_end;

                        term = stc_popTop(stack);  
                        if(term != _sml_)
                            goto fail_end;
                    }
                    else if(term == _S_)
                    {

                        term = stc_popTop(stack);
                        if(term != _rel_)
                            goto fail_end;

                        term = stc_popTop(stack);
                        if(term != _S_)
                            goto fail_end;

                        term = stc_popTop(stack);  
                        if(term != _sml_)
                            goto fail_end;
                    }
                    else
                    {
                        goto fail_end;
                    }

                    stc_push(stack, _L_, NULL);
                    break;

                /* F -> f
                 * F -> f E
                 */
                case _func_:
                    term = stc_popTop(stack);
                    if(term == _E_)
                    {
                        term = stc_popTop(stack);
                        if(term != _func_)
                            goto fail_end;
                    }
                    else if(term != _func_)
                        goto fail_end;

                    term = stc_popTop(stack);
                    if(term != _sml_)
                        goto fail_end;

                    stc_push(stack, _F_, NULL);

                    if(!func_elem->func.is_defined)
                            func_elem->func.n_params = num_params;
                    else if(func_elem->func.n_params != num_params)
                            goto sem_fail;

                    func_read = 1;
                    break;

                /* F -> f E, ... , E */    
                case _coma_:
                    while(42)
                    {
                        term = stc_popTop(stack);
                        if(term != _E_)
                            goto fail_end;

                        term = stc_popTop(stack);
                        if(term == _sml_)
                            break;
                        else if(term != _coma_)
                            goto fail_end;
                    }

                    term = stc_popTop(stack);
                    if(term != _func_)
                        goto fail_end;

                    term = stc_popTop(stack);
                    if(term != _sml_)
                        goto fail_end;  

                    stc_push(stack, _F_, NULL);

                    if(!func_elem->func.is_defined)
                            func_elem->func.n_params = num_params;
                    else if(func_elem->func.n_params != num_params)
                            goto sem_fail;

                    func_read = 1;
                    break;

            }
            stc_print(stack);
            
            if(sa_detectSucEnd(stack, token_term))
            {
                stc_destroy(stack);
#ifdef DEBUG_PREC
                destroyToken(token);
#else
                scanner_unget(que, token, sc_str->str);
#endif
                return SUCCESS;
            }
            else
                continue;
        }
        stc_print(stack);

        if(sa_detectSucEnd(stack, token_term))
        {
            stc_destroy(stack);

#ifdef DEBUG_PREC            
            destroyToken(token);
#else
            scanner_unget(que, token, sc_str->str);
#endif
            return SUCCESS;
        }

        token = scanner_get(sc_str, que);
    }

    stc_destroy(stack);
    return SUCCESS;

fail_end:
    stc_destroy(stack);
    free(token->name);
    free(token);
    return ERR_SYN;

sem_fail:
    stc_destroy(stack);
    free(token->name);
    free(token);
    return ERR_SEM_FUNC;

sem_fail_defined:
    stc_destroy(stack);
    free(token->name);
    free(token);
    return ERR_SEM_UNDEF;
}

////////////////////////////////////////////////////////////////////////
