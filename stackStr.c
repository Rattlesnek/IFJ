/**CFile****************************************************************

  FileName    [stack_gen.c]

  SystemName  [IFJ - PROJECT]

  PackageName [Data type]

  Synopsis    [Stack for code generation]

  Author      [Lukas Valek]

  Affiliation []

  Date        [16/11/2018]

  Revision    []

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                          INCLUDES                                ///
////////////////////////////////////////////////////////////////////////

#include "stackStr.h"


////////////////////////////////////////////////////////////////////////
///                       GLOBAL VARIABLES                           ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
////////////////////////////////////////////////////////////////////////

/**
 * @brief      Creates new stack with capacity STACK_INIT_CAP.
 *
 * @return     NULL    If error ocurred.
 * @return     *stack  Pointer to stack.
 */
stack_str_t *stcStr_create()
{
    stack_str_t *stack = malloc(sizeof(stack_str_t));
    if (stack == NULL)
        return NULL;

    stack->array = malloc(sizeof(char *) * (STACK_INIT_CAP));
    if (stack->array == NULL)
        return NULL;

    stack->top = EMPTY;
    stack->cap = STACK_INIT_CAP;

    return stack;
}


/**
 * @brief      Resizes stack when there is no more space.
 *
 * @param      *stack   Pointer to stack
 * @param[in]  new_cap  New capacity of stack
 *
 * @return     false   If error ocurred.
 * @return     true    If function ended successfuly.
 */
bool stcStr_resize(stack_str_t *stack, int new_cap)
{
    stack->array = realloc(stack->array, sizeof(char *) * (new_cap));
    if (stack == NULL)
        return false;

    stack->cap = new_cap;

    return true;
}


/**
 * @brief      Pushes new code to the stack. If there is no more space the stack             is resized.
 *
 * @param      *stack Pointer to stack
 * @param[in]  *code  Pointer to the array of chars
 *
 * @return     false  If error ocurred.
 * @return     true   If function ended successfuly.
 */
bool stcStr_push(stack_str_t *stack, char *code)
{
    if (stack->top == stack->cap - 1)
        if (stcStr_resize(stack, stack->cap + RESIZE_STEP) == false)
            return false;

    stack->array[++stack->top] = malloc(sizeof(char) * (strlen(code) + 1));
    if (stack->array[stack->top] == NULL)
    {
        return false; 
    }

    strcpy(stack->array[stack->top], code);
    return true;
}

/**
 * @brief      Returns code from top of stack
 *
 * @param      *stack Pointer to stack
 *
 * @return     *char  If function ended successfuly.
 * @return     NULL   If error ocurred.
 */
char *stcStr_top(stack_str_t *stack)
{
    if (stack->top == EMPTY)
        return NULL;

    return stack->array[stack->top];
}


/**
 * @brief      Pops code from top of stack.
 *
 * @param      stack  Pointer to stack
 *
 * @return     false  If error ocurred.
 * @return     true   If succesful
 */
bool stcStr_pop(stack_str_t *stack)
{
    if (stack->top == EMPTY)
        return false;
    free(stack->array[stack->top--]);
    return true;
}


/**
 * @brief      Frees memory allocated by stack.
 *
 * @param      *stack  Pointer to stack.
 */
void stcStr_destroy(stack_str_t *stack)
{
    bool succes = false;
    while ((succes = stcStr_pop(stack)) != false)
        ;        

    free(stack->array);
    free(stack);
}

/*
int main()
{
    char *array = "1111111111111111111111111111111111111112";
    char *pole = "random text ktery je assembley kod";
    char *pole1 = "dalsi random assembley kod";
    stack_str_t *stack = stcStr_create();

    stcStr_push(stack, array);
    stcStr_push(stack, pole);
    stcStr_push(stack, pole1);
    printf("%s\n", stcStr_top(stack));
    stcStr_pop(stack);
    printf("%s\n", stcStr_top(stack));
    stcStr_pop(stack);
    printf("%s\n", stcStr_top(stack));

    stcStr_destroy(stack);
    return 0;
}*/


////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////