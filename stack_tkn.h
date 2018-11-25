/**CFile****************************************************************

  FileName    [stack.h]

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
#ifndef STACK_H
#define STACK_H

#include <stdbool.h>

#include "token.h"

////////////////////////////////////////////////////////////////////////
///                         BASIC TYPES                              ///
////////////////////////////////////////////////////////////////////////

typedef struct stack_tkn {
    int top;            /* Index of element on the top of the stack */
    int cap;            /* Max capacity of stack                    */
    token_t **array;    /* Array of pointers to elements of stack   */    /* <= Predela se na token_t ** */
} stack_tkn_t;

////////////////////////////////////////////////////////////////////////
///                      MACRO DEFINITIONS                           ///
////////////////////////////////////////////////////////////////////////
#define STACK_INIT_CAP 20 /* Initial capacity of stack              */
#define RESIZE_STEP 10    /* Used when there is no more space       */
#define EMPTY -1          /* Top == -1 when stack is empty          */

////////////////////////////////////////////////////////////////////////
///                    FUNCTION DECLARATIONS                         ///
////////////////////////////////////////////////////////////////////////

/**
 * @brief Creates new stack with capacity STACK_INIT_CAP tokens.
 *
 * @return NULL        If error ocurred.
 *         stack_tkn_t *   Pointer to stack.
 */
stack_tkn_t *stcTkn_create();

/**
 * @brief Resizes stack when there is no more space. 
 *
 * @param  stack_tkn_t * Pointer to stack
 *         new_cap   New capacity of stack
 * @return false     If error ocurred.
 *         true      If function ended successfuly.
 */
bool stcTkn_resize(stack_tkn_t *stack, int new_cap);

/**
 * @brief Pushes new token to top of stack. If there is no more space the stack
 *        is resized.
 *
 * @param  stack_tkn_t * Pointer to stack
 *         *c        Pointer to token
 * @return false     If error ocurred.
 *         true      If function ended successfuly.
 */
bool stcTkn_push(stack_tkn_t *stack, token_t *token);

/**
 * @brief Pops token from top of stack. 
 *
 * @param stack_tkn_t *  Pointer to stack
 * @return NULL      If error ocurred.
 *         token_t * If function ended successfuly.
 */
token_t *stcTkn_pop(stack_tkn_t *stack);

/**
 * @brief Returns token from top of stack
 *
 * @param stack_tkn_t *  Pointer to stack
 * @return NULL      If error ocurred.
 *         token_t * If function ended successfuly.
 */
token_t *stcTkn_top(stack_tkn_t *stack);

/**
 * @brief Frees memory allocated by stack.
 *
 * @param stack_tkn_t * Pointer to stack.
 */
void stcTkn_destroy(stack_tkn_t *stack);



#endif 
////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////