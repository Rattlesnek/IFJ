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

token_t *createToken(char *name, token_info_t info)
{
    token_t *token = malloc(sizeof(token_t));
    if (token == NULL)
        return NULL;
    
    token->name = malloc( (strlen(name) + 1) * sizeof(char) );
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


void destroyToken(token_t *token)
{
    // if token is STR then also free info.string
    if (strcmp(token->name, "STR") == 0)
        free(token->info.string);
    
    free(token->name);
    free(token);
    // note that corresponding info-element in the hash table may still exist
}


////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////