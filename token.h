/**CFile****************************************************************
  FileName    [token.h]
  SystemName  [IFJ - PROJECT]
  PackageName [Lexical analysis]
  Synopsis    [Token manipulation]
  Author      [Adam Pankuch]
  Affiliation []
  Date        [18/10/2018]
  Revision    []
***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                          INCLUDES                                ///
////////////////////////////////////////////////////////////////////////
#ifndef TOKEN_H
#define TOKEN_H
#include <stdio.h>
#include <stdbool.h>
#include "symtable.h"

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
 * @brief Creates token and initializes it
 * 
 * @param name token name eg. "ID", "STR", "+", "==", "EOL", ...
 * @param info either info or ptr to corresponding info-element in hashtable
 * @return token_t* initialized token or NULL if not succesful
 */
token_t *tk_CreateToken(char *name, token_info_t info);


/**
 * @brief Destroys token
 * 
 * @param token token to be destroyed
 */
void tk_DestroyToken(token_t *token);


#endif
////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////