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
    P_WITHOUT_nonterm
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
    EXPR_term
} terminals_t;

// rules indexes
typedef enum {
    RULE_0,
    ST_LIST_1,
    ST_LIST_2,
    ST_LIST_3,
    EOL_EOF_4,
    EOL_EOF_5,
    STAT_6,
    STAT_7,
    COMMAND_8,
    COMMAND_9,
    COMMAND_10,
    FUNC_ASSIGN_EXPR_11,
    END_LIST_12,
    END_LIST_13,
    END_LIST_14,
    IF_LIST_15,
    IF_LIST_16,
    IF_LIST_17,
    IF_LIST_18,
    IF_LIST_19,
    ID_FUNC_20,
    ID_FUNC_21,
    PARAMS_GEN_22,
    PARAMS_GEN_23,
    PARAMS_GEN_24,
    P_BRACKETS_25,
    P_BRACKETS_26,
    P_BRACKETS_CONT_27,
    P_BRACKETS_CONT_28,
    P_WITHOUT_29,
    P_WITHOUT_30
} rules_t;


////////////////////////////////////////////////////////////////////////
///                      MACRO DEFINITIONS                           ///
////////////////////////////////////////////////////////////////////////

#define EXPR_INCLUDE -1
#define EXPR_INCLUDE_TWO -2

#define LL_ROWS 12
#define LL_COLS 15

#define RULES_ROWS 31
#define RULES_COLS 7


////////////////////////////////////////////////////////////////////////
///                       GLOBAL VARIABLES                           ///
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
///                    FUNCTION DECLARATIONS                         ///
////////////////////////////////////////////////////////////////////////


int ll_tableFind(char *nonterm, char *term);

int parser(dynamicStr_t *sc_str, queue_t *que);


static inline int isTerminal(char *name)
{
    return name[0] != '[';
}


static inline int isExpr(char *term)
{
    return (strcmp(term, "+") == 0 ||
            strcmp(term, "-") == 0 ||
            strcmp(term, "(") == 0 ||
            strcmp(term, "FUNC") == 0 ||
            strcmp(term, "STR") == 0 ||
            strcmp(term, "INT") == 0 ||
            strcmp(term, "DBL") == 0 ||
            strcmp(term, "nil") == 0);
}


#endif
////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////