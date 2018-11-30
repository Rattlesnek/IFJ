/**CFile****************************************************************

  FileName    [parser_gen.h]

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

#include "list.h"
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
 * @brief Prints prolog of whole program -- HEADER + definition of temporary variables
 * 
 */
void print_prolog();


/**
 * @brief Prepare expected if label
 * 
 * @param in_stat     determines whether inside if-statement/while-loop or not
 * @param label_stat  expected label storage
 */
void expected_LABEL_if(bool in_stat, char label_stat[]);


/**
 * @brief Generate whole IF
 * 
 * @param code_buffer buffer for code when in if-statement/while-loop
 * @param in_stat     determines whether inside if-statement/while-loop or not
 * @param stack       stack of strings
 * @param cond        condition token
 * @return true       if success
 * @return false      if internal error
 */
bool generate_if(list_t *code_buffer, bool in_stat, stack_str_t *stack, token_t *cond);


bool generate_LABEL_elsif(list_t *code_buffer, bool in_stat, stack_str_t *stack);


bool generate_elsif(list_t *code_buffer, bool in_stat, token_t *cond);


bool generate_else(list_t *code_buffer, bool in_stat, stack_str_t *stack);


/**
 * @brief Prepare expected label of while
 * 
 * @param in_stat     determines whether inside if-statement/while-loop or not
 * @param label_stat  expected label storage
 */
void expected_LABEL_while(bool in_stat, char label_stat[]);


/**
 * @brief Generate label of while
 * 
 * @param code_buffer buffer for code when in if-statement/while-loop
 * @param in_stat     determines whether inside if-statement/while-loop or not
 * @return true       if success
 * @return false      if internal error
 */
bool generate_LABEL_while(list_t *code_buffer, bool in_stat);


/**
 * @brief Generate condition of while
 * 
 * @param code_buffer buffer for code when in if-statement/while-loop
 * @param in_stat     determines whether inside if-statement/while-loop or not
 * @param cond        condition token
 * @return true       if success
 * @return false      if internal error
 */
bool generate_while_false(list_t *code_buffer, bool in_stat, token_t *cond);


/**
 * @brief Generate ending of while
 * 
 * @param stack   stack of strings
 * @return true   if success
 * @return false  if internal error
 */
bool generate_while_ending(stack_str_t *stack);




bool isFunctionEnd(char *generated_code);

/**
 * @brief Generate function definition
 * 
 * @param stack_str stack of strings
 * @param fun       symtable element -- function
 * @param param_arr array of parameters of function
 * @return true     if success
 * @return false    if internal error
 */
bool generate_function(stack_str_t *stack_str, elem_t *fun, dynamicArrParam_t *param_arr);



/**
 * @brief Generate variable
 * 
 * @param code_buffer   buffer for code when in if-statement/while-loop
 * @param defvar_buffer buffer for variable definitions when in if-statement/while-loop
 * @param in_stat       determines whether inside if-statement/while-loop or not
 * @param var_tab       general variable symtable (either global or local)
 * @param var_name      name of variable
 * @param right_val     value on the right side of assignment
 * @return int          exit code of generate var
 */
int generate_var(list_t *code_buffer, list_t *defvar_buffer, bool in_stat, symtable_t *var_tab, char *var_name, token_t *right_val);


#endif
////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////
