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
#include "dynamicArrParam.h"
#include "stackStr.h"
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

token_t *input(symtable_t *symtab, int type);

token_t *ord(symtable_t *symtab, token_t *par1, token_t *par2);

token_t *chr(symtable_t *symtab, token_t *par);

token_t *length(symtable_t *symtab, token_t *par);

token_t *substr(symtable_t *symtab, token_t *string, token_t *begin, token_t *end);

#endif
////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////