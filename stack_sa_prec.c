/**CFile****************************************************************

  FileName    [stack_sa_prec.c]

  SystemName  [IFJ - PROJECT]

  PackageName [Syntactic analysis]

  Synopsis    []

  Author      [Lukas Piwowarski]

  Affiliation []

  Date        [11/11/2018]

  Revision    []

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                          INCLUDES                                ///
////////////////////////////////////////////////////////////////////////
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "error.h"
#include "stack_sa_prec.h"
#include "sa_prec.h"

////////////////////////////////////////////////////////////////////////
///                       GLOBAL VARIABLES                           ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
////////////////////////////////////////////////////////////////////////

/**
 * @brief Creates new stack
 *
 * @return stack_sa_t *  Pointer to new stack
 *         NULL          If error ocurres
 */
stack_sa_t *stc_init()
{
    stack_sa_t *new = malloc(sizeof(stack_sa_t));
    if(new == NULL)
        return NULL;

    new->bot = NULL;
    new->top = NULL;

    return new;
}

/**
 * @brief      Pushes term to stack
 *
 * @param      stack  Pointer to stack
 * @param[in]  term   Term to be pushed to stack
 *
 * @return     true   If term is pushed successfuly
 *             false  If error ocurres 
 */
bool stc_push(stack_sa_t *stack, char term, token_t *tok)
{
    stack_item_t *new = malloc(sizeof(stack_item_t));
    if(new == NULL)
        return false;

    new->term = term;
    new->token = tok;
    if(stack->top == NULL && stack->bot == NULL)
    {
        stack->bot = new;
        stack->top = new;
        new->lptr = NULL;
        new->rptr = NULL;
    }
    else
    {
       stack->top->rptr = new;
       new->lptr = stack->top;
       stack->top = new; 
       new->rptr = NULL;
    }

    return true;
}

/**
 * @brief Pops token from top of stack
 *
 * @param  stack_sa_t * Pointer to stack
 * @return char         Term on top of stack
 */
char stc_popTop(stack_sa_t *stack)
{
    char term = stack->top->term;
    
    if(stack->top == stack->bot) 
    {
        if(stack->top->token != NULL)
        {
            destroyToken(stack->top->token);
        }
        free(stack->top);
        stack->top = NULL;
        stack->bot = NULL;
        return term;
    } 
    else
    {
        stack_item_t *tmp = stack->top;
        stack->top = stack->top->lptr;
        stack->top->rptr = NULL;

        if(tmp->token != NULL)
        {
            destroyToken(tmp->token);
        }
        free(tmp);
        return term;
    }

    return term;
}

token_t *stc_tokPopTop(stack_sa_t *stack, table_elem_t *term)
{
    token_t *token = stack->top->token;
    *term = stack->top->term;
    
    if(stack->top == stack->bot) 
    { 
        free(stack->top);
        stack->top = NULL;
        stack->bot = NULL;
        return token;
    } 
    else
    {
        stack_item_t *tmp = stack->top;
        stack->top = stack->top->lptr;
        stack->top->rptr = NULL;

        free(tmp);
        return token;
    }

    return token;
}

char stc_Top(stack_sa_t *stack)
{
    if(stack)
        return stack->top->term;

    return -1;
}

/**
 * @brief Frees memory allocated by stack.
 *
 * @param stack_t * Pointer to stack.
 */
void stc_destroy(stack_sa_t *stack)
{  
    stack_item_t *next = stack->bot;
    while(next != NULL)
    {
        stack_item_t *tmp = next;
        next = next->rptr;
        if(tmp->token != NULL)
        {
            destroyToken(tmp->token);
        }
        free(tmp);
    }

    free(stack);
}

/**
 * @brief Returns non-terminal nearest to top of stack
 *
 * @param   stack Stack containing wanted non-terminal
 * @return  char  Non-terminal nearest to top of stack
 *          EMPTY If there is no terminal in stack
 */
char stc_topTerm(stack_sa_t *stack)
{
    for(stack_item_t *tmp = stack->top; tmp != NULL; tmp = tmp->lptr)
    {
        if(tmp->term == _plus_)
            return _plus_;
        else if(tmp->term == _mult_)
            return _mult_;
        else if(tmp->term == _id_)
            return _id_;
        else if(tmp->term == _empt_)
            return _empt_;
        else if(tmp->term == _lbrc_)
            return _lbrc_;
        else if(tmp->term == _rbrc_)
            return _rbrc_;
        else if(tmp->term == _mins_)
            return _mins_;
        else if(tmp->term == _div_)
            return _div_;
        else if(tmp->term == _rel_)
            return _rel_;
        else if(tmp->term == _str_)
            return _str_;
        else if(tmp->term == _func_)
            return _func_;
        else if(tmp->term == _coma_)
            return _coma_;
    }

    return EMPTY;
}

bool stc_pushAfter(stack_sa_t *stack, table_elem_t term, table_elem_t rule)
{
    stack_item_t *tmp = NULL;
    for(tmp = stack->top; tmp->term != (char)term; tmp = tmp->lptr)
        ;

    stack_item_t *new = malloc(sizeof(stack_item_t));
    if(new == NULL)
        return false;

    new->term = rule;
    new->token = NULL;
    if(tmp->rptr != NULL)
    {
        new->rptr = tmp->rptr;
        tmp->rptr->lptr = new;
        tmp->rptr = new;
        new->lptr = tmp;
    }
    else
    {
        new->lptr = tmp;
        new->rptr = NULL;
        tmp->rptr = new;
        stack->top = new;
    }

    return false;
}


void stc_print(stack_sa_t *stack)
{
    printf("=> sa_prec: STACK:\n");
    for(stack_item_t *tmp = stack->top; tmp != NULL; tmp = tmp->lptr)
    {
        printf("%d\n", tmp->term);
    }

    return;
}
#if 0
int main()
{
    stack_sa_t *stack = stc_init();    
    
    for(int i = 0; i < 20; i++)
        stc_push(stack, i);

    printf("topTerm: %d\n", stc_topTerm(stack));

    for(int i = 0; i < 20; i++)
        printf("%d\n", stc_popTop(stack));

    stc_destroy(stack);
    return 0;
}
#endif 
////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////
