/**CFile****************************************************************

  FileName    [token.c]

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

#include <stdlib.h>
#include <string.h>

#include "token.h"

////////////////////////////////////////////////////////////////////////
///                       GLOBAL VARIABLES                           ///
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
////////////////////////////////////////////////////////////////////////

/**
 * @brief Creates token and initializes it
 * 
 * @param name token name eg. "ID", "STR", "+", "==", "EOL", ...
 * @param info either info or ptr to corresponding info-element in hashtable
 * @return token_t* initialized token or NULL if not succesful
 */
token_t *tk_CreateToken(char *name, token_info_t info)
{
    token_t *token = malloc(sizeof(token_t));
    if (token == NULL)
        return NULL;
    
    token->name = malloc(sizeof(name));
    if (token->name == NULL)
    {
        free(token);
        return NULL;
    }

    // add name to token
    strcpy(token->name, name);
    // add info to token
    token->info = info;

    return token;
}

/**
 * @brief Destroys token
 * 
 * @param token token to be destroyed
 */
void tk_DestroyToken(token_t *token)
{
    free(token->name);
    free(token);
    // note that corresponding info-element in the hash table may still exist
}


////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////