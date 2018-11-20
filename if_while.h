/**CFile****************************************************************

  FileName    []

  SystemName  [IFJ - PROJECT]

  PackageName []

  Synopsis    []

  Author      []

  Affiliation []

  Date        [--/--/2018]

  Revision    []

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                          INCLUDES                                ///
////////////////////////////////////////////////////////////////////////
#ifndef TEMPLATE_H
#define TEMPLATE_H

#include <stdbool.h>

#include "parser.h"

#include "scanner.h"
#include "sa_prec.h"

#include "dynamicArrParam.h"
#include "dynamicStr.h"
#include "stackStr.h"
#include "stackTkn.h"
#include "queue.h"

#include "token.h"
#include "symtable.h"

#include "funcGen.h"

#include "error.h"

////////////////////////////////////////////////////////////////////////
///                         BASIC TYPES                              ///
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
///                      MACRO DEFINITIONS                           ///
////////////////////////////////////////////////////////////////////////
#define IF_LENGTH 

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
