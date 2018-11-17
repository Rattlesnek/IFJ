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

/**
 * @brief token info union
 * 
 */
typedef union token_info {
    elem_t *ptr;    // either ptr to info-element in hash table
    char *string;   // or string (use only when token name is "STR" or "INT" or "DBL")
} token_info_t;

/**
 * @brief token structure
 * 
 */
typedef struct token {
    char *name;         // token name eg. "ID", "STR", "+", "==", "EOL", ...
    token_info_t info;  // token info      
} token_t;

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
token_t *createToken(char *name, token_info_t info);


/**
 * @brief Destroys token
 * 
 * @param token token to be destroyed
 */
void destroyToken(token_t *token);


#endif
////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////