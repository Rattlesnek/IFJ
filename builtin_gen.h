/**CFile****************************************************************

  FileName    [builtin_gen.h]

  SystemName  [IFJ - PROJECT]

  PackageName []

  Synopsis    []

  Author      [Jindrich Sestak, Lukas Valek]

  Affiliation []

  Date        [--/--/2018]

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
#include "stack_tkn.h"
#include "list.h"

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
 * @brief Reads one line from input
 * 
 * @param symtab Symtable in which is function called
 * @param type Which form of function input we want(inputs,inputi,inputf)
 * @return token_t* 
 */
token_t *input(symtable_t *symtab, int type);

/**
 * @brief Returns ASCII value of char in string par1 on position par2
 * 
 * @param symtab Symtable in which is function called
 * @param par1  String
 * @param par2 Position of char in string
 * @return token_t* 
 */
token_t *ord(list_t *code_buffer, bool in_stat, symtable_t *symtab, token_t *par1, token_t *par2);

/**
 * @brief Return char (in ASCII) which is represented by number par
 * 
 * @param symtab Symtable in which is function called
 * @param par number which is converted to char in ASCII
 * @return token_t* 
 */
token_t *chr(list_t *code_buffer, bool in_stat,symtable_t *symtab, token_t *par);

/**
 * @brief Returns length of string in par
 * 
 * @param symtab Symtable in which is function called
 * @param par String, which length should be measured
 * @return token_t* 
 */
token_t *length(symtable_t *symtab, token_t *par);

/**
 * @brief Retrns substring starting on position begin and long end chars
 * 
 * @param symtab Symtable in which is function called
 * @param string String which substring we want
 * @param begin Position where we start
 * @param end Length of substring
 * @return token_t* 
 */
token_t *substr(list_t *code_buffer, bool in_stat, symtable_t *symtab, token_t *string, token_t *begin, token_t *end);

token_t *print(symtable_t *symtab, stack_tkn_t *stack, list_t *code_buffer, bool in_stat);

#endif
////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////