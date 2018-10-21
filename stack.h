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
#include "token.h"

////////////////////////////////////////////////////////////////////////
///                         BASIC TYPES                              ///
////////////////////////////////////////////////////////////////////////
typedef struct stack {
    int top;            /* Index of element on the top of the stack */
    int cap;            /* Max capacity of stack                    */
    char **array;       /* Array of pointers to elements of stack   */    /* <= Predela se na token_t ** */
} stack_t;

////////////////////////////////////////////////////////////////////////
///                      MACRO DEFINITIONS                           ///
////////////////////////////////////////////////////////////////////////
#define STACK_INIT_CAP 20 /* Initial capacity of stack              */
#define RESIZE_STEP 10    /* Used when there is no more space       */
#define EMPTY -1          /* Top == -1 when stack is empty          */
#define SUCCESS 1         /* Return value                           */
#define FAILURE 0         /* Return value                           */

////////////////////////////////////////////////////////////////////////
///                    FUNCTION DECLARATIONS                         ///
////////////////////////////////////////////////////////////////////////






#endif 
////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////