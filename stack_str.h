/**CFile****************************************************************

  FileName    [stack_str.h]

  SystemName  [IFJ - PROJECT]

  PackageName [Data type]

  Synopsis    [Stack for code generation]

  Author      [Lukas Valek]

  Login       [xvalek15]

  Affiliation []

  Date        [16/11/2018]

  Revision    []

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                          INCLUDES                                ///
////////////////////////////////////////////////////////////////////////
///
#ifndef STACK_GEN_H
#define STACK_GEN_H

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


////////////////////////////////////////////////////////////////////////
///                         BASIC TYPES                              ///
////////////////////////////////////////////////////////////////////////

typedef struct stack_str {
  int top;            /* Index of element on the top of the stack */
  int cap;            /* Max capacity of stack                    */
  char **array;    /* Array of pointers to elements of stack   */
} stack_str_t;

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
 * @brief      Creates new stack with capacity STACK_INIT_CAP.
 *
 * @return     NULL    If error ocurred.
 * @return     *stack  Pointer to stack.
 */
stack_str_t *stcStr_create();


/**
 * @brief      Resizes stack when there is no more space.
 *
 * @param      *stack   Pointer to stack
 * @param[in]  new_cap  New capacity of stack
 *
 * @return     false   If error ocurred.
 * @return     true    If function ended successfuly.
 */
bool stcStr_resize(stack_str_t *stack, int new_cap);


/**
 * @brief      Pushes new code to the stack. If there is no more space the stack             is resized.
 *
 * @param      *stack Pointer to stack
 * @param[in]  *code  Pointer to the array of chars
 *
 * @return     false  If error ocurred.
 * @return     true   If function ended successfuly.
 */
bool stcStr_push(stack_str_t *stack, char *code);


/**
 * @brief      Pops code from top of stack.
 *
 * @param      stack  Pointer to stack
 *
 * @return     false  If error ocurred.
 * @return     true   If succesful
 */
bool stcStr_pop(stack_str_t *stack);


/**
 * @brief      Returns code from top of stack
 *
 * @param      *stack Pointer to stack
 *
 * @return     *char  If function ended successfuly.
 * @return     NULL   If error ocurred.
 */
char *stcStr_top(stack_str_t *stack);


/**
 * @brief      Frees memory allocated by stack.
 *
 * @param      *stack  Pointer to stack.
 */
void stcStr_destroy(stack_str_t *stack);



#endif
////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////