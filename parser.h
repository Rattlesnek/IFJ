/**CFile****************************************************************

  FileName    [parser.h]

  SystemName  [IFJ - PROJECT]

  PackageName [Syntactic analysis]

  Synopsis    []

  Author      [Adam Pankuch]

  Affiliation []

  Date        [26/10/2018]

  Revision    []

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                          INCLUDES                                ///
////////////////////////////////////////////////////////////////////////
#ifndef PARSER_H
#define PARSER_H

#include <stdlib.h>

#include "queue.h"
#include "dynamicStr.h"
#include "dynamicArrInt.h"
#include "stack.h"
#include "symtable.h"

////////////////////////////////////////////////////////////////////////
///                         BASIC TYPES                              ///
////////////////////////////////////////////////////////////////////////

// nonterminal indexes to LL-gramar table
typedef enum {
    ST_LIST_nonterm,
    EOF_EOL_nonterm,
    STAT_nonterm,
    COMMAND_nonterm,
    FUNC_ASSIGN_EXPR_nonterm,
    END_LIST_nonterm,
    IF_LIST_nonterm,
    ID_FUNC_nonterm,
    PARAMS_GEN_nonterm,
    P_BRACKETS_nonterm,
    P_BRACKETS_CONT_nonterm,
    P_WITHOUT_nonterm,
} nonterminals_t;

// terminal indexes to LL-gramar table
typedef enum {
    IF_term, 
    ELSE_term,
    ELIF_term,
    END_term,
    WHILE_term,
    DEF_term, 
    EOL_term,
    EOF_term,
    EQUALS_term,
    L_BRACKET_term,
    R_BRACKET_term,
    COMMA_term,
    ID_term, 
    FUNC_term,
} terminals_t;


////////////////////////////////////////////////////////////////////////
///                      MACRO DEFINITIONS                           ///
////////////////////////////////////////////////////////////////////////

#define EXPR_INCLUDE -1
#define EXPR_INCLUDE_TWO -2

#define LL_ROWS 12
#define LL_COLS 14

#define RULES_ROWS 31
#define RULES_COLS 7


////////////////////////////////////////////////////////////////////////
///                       GLOBAL VARIABLES                           ///
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
///                    FUNCTION DECLARATIONS                         ///
////////////////////////////////////////////////////////////////////////


int ll_tableFind(char *nonterm, char *term);

int parser(dynamicStr_t *sc_str, queue_t *que, symtable_t *symtable);


static inline int isTerminal(char *name)
{
    return name[0] != '[';
}




#endif
////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////