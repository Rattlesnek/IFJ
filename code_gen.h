/**CFile****************************************************************

  FileName    []

  SystemName  [IFJ - PROJECT]

  PackageName [Code generation implementation]

  Synopsis    []

  Author      [Lukas Valek]

  Affiliation []

  Date        [17/11/2018]

  Revision    []

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                          INCLUDES                                ///
////////////////////////////////////////////////////////////////////////
#ifndef CODEGEN_H
#define CODEGEN_H

#include <stdio.h>
#include "token.h"
#include "symtable.h"
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

// enum na indexy
typedef enum index {
  INT_index,
  DBL_index,
  STR_index,
  ID_index

} index_t;


// enum na switchdexy
typedef enum index_switch {
  INT_INT,
  INT_DBL,
  INT_STR,
  INT_ID,
  DBL_INT,
  DBL_DBL,
  DBL_STR,
  DBL_ID,
  STR_INT,
  STR_DBL,
  STR_STR,
  STR_ID,
  ID_INT,
  ID_DBL,
  ID_STR,
  ID_ID,
  NIL_ID,
  ID_NIL
} index_switch_t;


/**
 * @brief      Determines if int.
 *
 * @param      token  The token
 *
 * @return     True if int, False otherwise.
 */
bool isINT(token_t *token);

/**
 * @brief      Determines if dbl.
 *
 * @param      token  The token
 *
 * @return     True if dbl, False otherwise.
 */
bool isDBL(token_t *token);
/**
 * @brief      Determines if str.
 *
 * @param      token  The token
 *
 * @return     True if str, False otherwise.
 */
bool isSTR(token_t *token);
/**
 * @brief      Determines if id.
 *
 * @param      token  The token
 *
 * @return     True if id, False otherwise.
 */
bool isID(token_t *token);
/**
 * @brief      Determines if int id.
 *
 * @param      token  The token
 *
 * @return     True if int id, False otherwise.
 */
bool isINT_ID(token_t *token);
/**
 * @brief      Determines if dbl id.
 *
 * @param      token  The token
 *
 * @return     True if dbl id, False otherwise.
 */
bool isDBL_ID(token_t *token);
/**
 * @brief      Determines if str id.
 *
 * @param      token  The token
 *
 * @return     True if str id, False otherwise.
 */
bool isSTR_ID(token_t *token);

/**
 * @brief      { function_description }
 *
 * @param      op  The parameter 1
 * @param      alternative  The parameter 2
 *
 * @return     { description_of_the_return_value }
 */
char *operator(char *op, bool alternative);
/**
 * @brief      { function_description }
 *
 * @param      param1  The parameter 1
 * @param      param2  The parameter 2
 *
 * @return     { description_of_the_return_value }
 */
int type(token_t *param1, token_t *param2);
/**
 * @brief      { function_description }
 *
 * @param      op      The operation
 * @param      par1    The par 1
 * @param      par2    The par 2
 * @param      symtab  The symtab
 *
 * @return     { description_of_the_return_value }
 */
token_t *nil(token_t *op, token_t *par1, token_t *par2, list_t *code_buffer, bool in_stat);
/**
 * @brief      { function_description }
 *
 * @param      op      The operation
 * @param      par1    The par 1
 * @param      par2    The par 2
 * @param      symtab  The symtab
 *
 * @return     { description_of_the_return_value }
 */
token_t *int_int(token_t *op, token_t *par1, token_t *par2, symtable_t *symtab, bool variant, list_t *code_buffer, bool in_stat);
/**
 * @brief      { function_description }
 *
 * @param      op      The operation
 * @param      par1    The par 1
 * @param      par2    The par 2
 * @param      symtab  The symtab
 *
 * @return     { description_of_the_return_value }
 */
token_t *dbl_dbl(token_t *op, token_t *par1, token_t *par2, symtable_t *symtab, list_t *code_buffer, bool in_stat);
/**
 * @brief      { function_description }
 *
 * @param      op        The operation
 * @param      par1      The par 1
 * @param      par2      The par 2
 * @param      symtab    The symtab
 * @param[in]  switched  The switched
 *
 * @return     { description_of_the_return_value }
 */
token_t *int_dbl(token_t *op, token_t *par1, token_t *par2, symtable_t *symtab, bool switched, list_t *code_buffer, bool in_stat);
/**
 * @brief      { function_description }
 *
 * @param      op      The operation
 * @param      par1    The par 1
 * @param      par2    The par 2
 * @param      symtab  The symtab
 *
 * @return     { description_of_the_return_value }
 */
token_t *int_str(token_t *op, token_t *par1, token_t *par2, list_t *code_buffer, bool in_stat);
/**
 * @brief      { function_description }
 *
 * @param      op      The operation
 * @param      par1    The par 1
 * @param      par2    The par 2
 * @param      symtab  The symtab
 *
 * @return     { description_of_the_return_value }
 */
token_t *dbl_str(token_t *op, token_t *par1, token_t *par2, symtable_t *symtab, list_t *code_buffer, bool in_stat);
/**
 * @brief      { function_description }
 *
 * @param      op        The operation
 * @param      par1      The par 1
 * @param      par2      The par 2
 * @param      symtab    The symtab
 * @param[in]  switched  The switched
 *
 * @return     { description_of_the_return_value }
 */
token_t *int_id(token_t *op, token_t *par1, token_t *par2, symtable_t *symtab, bool switched, list_t *code_buffer, bool in_stat);
/**
 * @brief      { function_description }
 *
 * @param      op        The operation
 * @param      par1      The par 1
 * @param      par2      The par 2
 * @param      symtab    The symtab
 * @param[in]  switched  The switched
 *
 * @return     { description_of_the_return_value }
 */
token_t *dbl_id(token_t *op, token_t *par1, token_t *par2, symtable_t *symtab, bool switched, list_t *code_buffer, bool in_stat);
/**
 * @brief      { function_description }
 *
 * @param      op      The operation
 * @param      par1    The par 1
 * @param      par2    The par 2
 * @param      symtab  The symtab
 *
 * @return     { description_of_the_return_value }
 */
token_t *str_str(token_t *op, token_t *par1, token_t *par2, symtable_t *symtab, list_t *code_buffer, bool in_stat);
/**
 * @brief      { function_description }
 *
 * @param      op        The operation
 * @param      par1      The par 1
 * @param      par2      The par 2
 * @param      symtab    The symtab
 * @param[in]  switched  The switched
 *
 * @return     { description_of_the_return_value }
 */
token_t *str_id(token_t *op, token_t *par1, token_t *par2, symtable_t *symtab, bool switched, list_t *code_buffer, bool in_stat);
/**
 * @brief      { function_description }
 *
 * @param      op        The operation
 * @param      par1      The par 1
 * @param      par2      The par 2
 * @param      symtab    The symtab
 * @param[in]  switched  The switched
 *
 * @return     { description_of_the_return_value }
 */
token_t *id_id(token_t *op, token_t *par1, token_t *par2, symtable_t *symtab, bool switched, list_t *code_buffer, bool in_stat);
/**
 * @brief      { function_description }
 *
 * @param      op      The operation
 * @param      param1  The parameter 1
 * @param      param2  The parameter 2
 * @param      symtab  The symtab
 *
 * @return     { description_of_the_return_value }
 */
token_t *gen_expr(token_t *op, token_t *param1, token_t *param2, symtable_t *symtab, list_t *code_buffer, bool in_stat);


#endif
////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////