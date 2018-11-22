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

#include "symtable.h"
#include "dynamicArrParam.h"
#include "stackStr.h"

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
bool generate_if(symtable_t *var_tab, stack_str_t *stack, char *cond);

/**
 * @brief Function to generate WHILE
 * 
 * @param stack Stack to store orders
 *              /Ret_val accepted from parser
 * @return true Returns true if everything pushed on stack properly
 * @return false Returns false if error occured
 */
bool generate_while_ending(stack_str_t *stack);

/**
 * @brief Prints "LABEL $while$%llu\n"
 */
void generate_LABEL_while();

/**
 * @brief Prints "JUMPIFEQ $end_while$%llu COND int@0\n" ergo ==> Condition of while
 * 
 */
void generate_while_false(symtable_t *var_tab, char *cond);


bool generate_function(stack_str_t *stack_str, elem_t *fun, dynamicArrParam_t *param_arr);


void generate_var(symtable_t *var_tab, char *var_name, char *right_val);


#endif
////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////
