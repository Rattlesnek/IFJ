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


void print_prolog();


void expected_LABEL_if(bool in_stat, char label_stat[]);


bool generate_if(list_t *code_buffer, bool in_stat, stack_str_t *stack, token_t *cond);


void expected_LABEL_while(bool in_stat, char label_stat[]);


bool generate_LABEL_while(list_t *code_buffer, bool in_stat);



bool generate_while_false(list_t *code_buffer, bool in_stat, token_t *cond);



bool generate_while_ending(stack_str_t *stack);



bool generate_function(stack_str_t *stack_str, elem_t *fun, dynamicArrParam_t *param_arr);



int return_right_strings(symtable_t *var_tab, token_t *right_val, char frame[3], char frame_or_type[7], char **var_or_const);


int generate_var(list_t *code_buffer, list_t *defvar_buffer, bool in_stat, symtable_t *var_tab, char *var_name, token_t *right_val);


#endif
////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////
