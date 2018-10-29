/**CFile****************************************************************

  FileName    [stack.c]

  SystemName  [IFJ - PROJECT]

  PackageName [Data type]

  Synopsis    [Stack implementation]

  Author      [Lukas Piwowarski]

  Affiliation []

  Date        [20/10/2018]

  Revision    []

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                          INCLUDES                                ///
////////////////////////////////////////////////////////////////////////
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "error.h"
#include "stack.h"

////////////////////////////////////////////////////////////////////////
///                       GLOBAL VARIABLES                           ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
////////////////////////////////////////////////////////////////////////

/**
 * @brief Creates new stack with capacity STACK_INIT_CAP tokens.
 *
 * @return NULL        If error ocurred.
 *         stack_t *   Pointer to stack.
 */
stack_t *stc_create()
{
    stack_t *stack = malloc(sizeof(stack_t));
    if(stack == NULL)
        return NULL;

    stack->array = malloc(sizeof(char *) * (STACK_INIT_CAP));
    if(stack->array == NULL)
        return NULL;

    stack->top = EMPTY;
    stack->cap = STACK_INIT_CAP;

    return stack;
}

/**
 * @brief Resizes stack when there is no more space. 
 *
 * @param  stack_t * Pointer to stack
 *         new_cap   New capacity of stack
 * @return false   If error ocurred.
 *         true    If function ended successfuly.
 */
bool stc_resize(stack_t *stack, int new_cap)
{
    stack->array = realloc(stack->array,sizeof(char *)*(new_cap));
    if(stack == NULL)
        return false;

    stack->cap = new_cap;

    return true;
}

/**
 * @brief Pushes new token to top of stack. If there is no more space the stack
 *        is resized.
 *
 * @param  stack_t * Pointer to stack
 *         *c        Pointer to token
 * @return false   If error ocurred.
 *         true    If function ended successfuly.
 */
bool stc_push(stack_t *stack, token_t *token)
{
    if(stack->top == stack->cap - 1)
        if(stc_resize(stack, stack->cap + RESIZE_STEP) == false)
            return false;

    stack->array[++stack->top] = token;
    return true;
}

/**
 * @brief Returns token from top of the stack
 *
 * @param stack_t *  Pointer to stack
 * @return NULL      If error ocurred.
 *         token_t * If function ended successfuly.
 */
token_t *stc_top(stack_t *stack)
{
    if(stack->top == EMPTY)
        return NULL;

    return stack->array[stack->top];
}


/**
 * @brief Pops token from top of stack. 
 *
 * @param stack_t *  Pointer to stack
 * @return NULL      If error ocurred.
 *         token_t * If function ended successfuly.
 */
token_t *stc_pop(stack_t *stack)
{
    if(stack->top == EMPTY)
        return NULL;

    return stack->array[stack->top--];
}

/**
 * @brief Frees memory allocated by stack.
 *
 * @param stack_t * Pointer to stack.
 */
void stc_destroy(stack_t *stack)
{
    free(stack->array);
    free(stack);
}

/**
int main()
{
    char array[40] = "1111111111111111111111111111111111111112";
    stack_t *stack = stc_create();
    for(int i = 0; i < 40; i++)
    {
        //printf("%d. push\n",i+1);
        stc_push(stack, &array[i]);
    }

    for(int i = 0; i < 40; i++)
    {
        char *c;
        c = stc_pop(stack);
        printf("%d: %c\n", i, *c);
    }

    stc_destroy(stack);
    return 0;
}
*/

////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////