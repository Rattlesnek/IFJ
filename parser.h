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

////////////////////////////////////////////////////////////////////////
///                         BASIC TYPES                              ///
////////////////////////////////////////////////////////////////////////

// nonterminal indexes to LL-gramar table
typedef enum {
    ST_LIST_nonterm,
    STAT_nonterm,
    COMMAND_nonterm,
    END_LIST_nonterm,
    IF_LIST_nonterm,
    ID_FUNC_nonterm,
    PARAMS_GEN_nonterm,
    P_BRACKETS_nonterm,
    P_BRACKETS_CONT_nonterm,
    P_WITHOUT_nonterm,
    FUNC_ASSIGN_EXPR_nonterm
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

#define LL_ROWS 11
#define LL_COLS 14

#define RULES_ROWS 29
#define RULES_COLS 7


////////////////////////////////////////////////////////////////////////
///                       GLOBAL VARIABLES                           ///
////////////////////////////////////////////////////////////////////////

// LL-Grammar table
int ll_table[LL_ROWS][LL_COLS] = {
    // if/els/elif/end/whi/def/EOL/EOF/ =/ (/  )/  ,/  id/func
    {   1,	0,	0,	0,	1,	1,	2,	3,	0,	0,	0,	0,	1,	0,	},  // [st-list]
    {   5,	0,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	5,	0,	},  // [stat]
    {   7,	0,	0,	0,	6,	0,	0,	0,	0,	0,	0,	0,	8,	0,	},  // [command]
    {   9,	0,	0,	11,	9,	0,	10,	0,	0,	0,	0,	0,  9,	0,	},  // [end-list]
    {   12,	15,	14,	16,	12,	0,	13,	0,	0,	0,	0,	0,	12,	0,	},  // [if-list]
    {   0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	17,	18,	},  // [id-func]
    {   0,	0,	0,	0,	0,	0,	21,	0,	0,	19,	0,	0,	20,	0,	},  // [params-gen]
    {   0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	23,	0,	22,	0,	},  // [p-brackets]
    {   0,	0,  0,	0,	0,	0,	0,	0,	0,	0,	25,	24,	0,	0,	},  // [p-brackets-cont]
    {   0,	0,	0,	0,	0,	0,	27,	0,	0,	0,	0,	26,	0,	0,	},  // [p-without]
    {   0,	0,	0,	0,	0,	0,	0,	0,	28,	0,	0,	0,	0,	0,	}   // [func-assign-expr]
};


char *reverted_rules[RULES_ROWS][RULES_COLS] = {
    {  NULL, }, // empty rule   
    { "[st-list]", "[stat]", },
    { "[st-list]", "EOL", },
    {  NULL, }, // finishing rule: [st-list] -> epsilon and in next step succ = true
    { "EOL", "[end-list]", "[params-gen]", "[id-func]", "def", },
    { "[command]", },
    { "EOL", "[end-list]", "EOL", "do", "**expr**", "while", },
    { "EOL", "[if-list]", "EOL", "then", "**expr**", "if", },
    { "EOL", "**expr**", "=", "ID", },
    { "[end-list]", "[command]", },
    { "[end-list]", "EOL", },
    { "end", },
    { "[if-list]", "[command]", },
    { "[if-list]", "EOL", },
    { "[if-list]", "EOL", "then", "**expr**", "elif", },
    { "[end-list]", "EOL", "else", },
    { "end", },
    { "ID", },
    { "FUNC", },
    { "[p-brackets]", "(", },
    { "[p-without]", "ID", },
    { "EOL", },
    { "[p-brackets-continue]", "ID", },
    { "EOL", ")", },
    { "[p-brackets-continue]", "ID", ",", },
    { "EOL", ")", },
    { "[p-without]", "ID", ",", },
    { "EOL", },
    { "EOL", "**expr**", "=", }
};

////////////////////////////////////////////////////////////////////////
///                    FUNCTION DECLARATIONS                         ///
////////////////////////////////////////////////////////////////////////


static inline int isTerminal(char *name)
{
    return name[0] != '[';
}



#endif
////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////