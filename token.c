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
  if (token == NULL)
    return;

  // if token is STR then also free info.string
  if (strcmp(token->name, "STR") == 0 || strcmp(token->name, "INT") == 0 || strcmp(token->name, "DBL" ) == 0)
    free(token->info.string);

  free(token->name);
  free(token);
  // note that corresponding info-element in the hash table may still exist
}


////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////
#if 0

printf("DEFVAR %s@%s%llu$type\n"
       "DEFVAR %s@%s%llu$tmp\n"
       "TYPE %s@%s%llu$type %s@%s\n"
       "MOVE %s@%s%llu$tmp %s@%s\n"
       "JUMPIFEQ $%s$%llu$int %s@%s%llu$type string@int\n"
       "JUMPIFEQ $%s$%llu$float %s@%s%llu$type string@float\n"
       "EXIT int@4\n"
       "LABEL $%s$%llu$int\n"
       "INT2FLOAT %s@%s%llu$tmp %s@%s\n"
       "LABEL $%s$%llu$float\n"
       "%s %s@%s %s@%s%llu$tmp %s@%s\n",
       frame,
       print2,
       label_n,//
       frame,
       print2,
       label_n,//
       frame,
       print2,
       label_n,
       frame,
       print2,//
       frame,
       print2,
       label_n,
       frame,
       print2,//
       symtab->name,
       label_n,
       frame,
       print2,
       label_n,//
       symtab->name,
       label_n,
       frame,
       print2,
       label_n,//
       symtab->name,
       label_n,//
       frame,
       print2,
       label_n,
       frame,
       print2,
       symtab->name,
       label_n,//
       operator(op->name),
       frame,
       des->info.ptr->var.key,
       frame,
       print2,
       label_n,
       param1,
       print1//
      );

#endif