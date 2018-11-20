/**CFile****************************************************************

  FileName    [if_while.h]

  SystemName  [IFJ - PROJECT]

  PackageName []

  Synopsis    []

  Author      [Jindrich Sestak]

  Affiliation []

  Date        [20/11/2018]

  Revision    []

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                          INCLUDES                                ///
////////////////////////////////////////////////////////////////////////
#ifndef TEMPLATE_H
#define TEMPLATE_H

#include <stdbool.h>

#include "parser.h"
#include "sa_prec.h"
#include "stackStr.h"
#include "error.h"

////////////////////////////////////////////////////////////////////////
///                         BASIC TYPES                              ///
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
///                      MACRO DEFINITIONS                           ///
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
///                    FUNCTION DECLARATIONS                         ///
////////////////////////////////////////////////////////////////////////

/**
 * @brief Function to generate IF-ELSE
 * 
 * @param stack Stack to store orders
 *              /Ret_val accepted from parser
 * @return true Returns true if everything pushed on stack properly
 * @return false Returns false if error occured
 */
bool generate_if(stack_str_t *stack);

/**
 * @brief Function to generate WHILE
 * 
 * @param stack Stack to store orders
 *              /Ret_val accepted from parser
 * @return true Returns true if everything pushed on stack properly
 * @return false Returns false if error occured
 */
bool generate_while(stack_str_t *stack);


#endif
////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////
