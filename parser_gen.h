/**CFile****************************************************************

  FileName    [if_while.h]

  SystemName  [IFJ - PROJECT]

  PackageName []

  Synopsis    []

  Author      [Adam Pankuch, Jindrich Sestak]

  Affiliation []

  Date        [20/11/2018]

  Revision    []

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                          INCLUDES                                ///
////////////////////////////////////////////////////////////////////////
#ifndef PARSER_GEN_H
#define PARSER_GEN_H

#include <stdbool.h>

#include "symtable.h"
#include "dynamic_arr_param.h"
#include "stack_str.h"
#include "token.h"

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
int generate_if(symtable_t *var_tab, stack_str_t *stack, token_t *cond);

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
int generate_while_false(symtable_t *var_tab, token_t *cond);


bool generate_function(stack_str_t *stack_str, elem_t *fun, dynamicArrParam_t *param_arr);


int generate_var(symtable_t *var_tab, char *var_name, token_t *right_val);

int return_right_strings(symtable_t *var_tab, token_t *right_val, char frame[3], char frame_or_type[7], char **var_or_const);

#endif
////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////
