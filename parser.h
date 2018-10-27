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
    FUNC_ASSIGN_EXPR_nonterm,
    ASSIGN_EXPR_nonterm,
    END_LIST_nonterm,
    IF_LIST_nonterm,
    ID_FUNC_nonterm,
    PARAMS_GEN_nonterm,
    P_BRACKETS_nonterm,
    P_BRACKETS_CONT_nonterm,
    P_WITHOUT_nonterm,
    ID_nonterm // [id]
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
    NIL_term,
    STR_term,
    INT_term,
    DBL_term,
    EXPR_term
} terminals_t;


////////////////////////////////////////////////////////////////////////
///                      MACRO DEFINITIONS                           ///
////////////////////////////////////////////////////////////////////////

#define LL_ROWS 13
#define LL_COLS 19

#define RULES_ROWS 39
#define RULES_COLS 7


////////////////////////////////////////////////////////////////////////
///                       GLOBAL VARIABLES                           ///
////////////////////////////////////////////////////////////////////////

// LL-Grammar table
int ll_table[LL_ROWS][LL_COLS] = {
    // if/els/elif/end/whi/def/EOL/EOF/ =/ (/  )/  ,/  id/func/nil/str/int/dbl/**expr**
    {   1,	0,	0,	0,	1,	1,	2,	3,	0,	0,	0,	0,	1,	1,	0,	0,  0,  0,	1   },  // [st-list]
    {   5,	0,	0,	0,	5,	4,	0,	0,	0,	0,	0,	0,	5,	5,	0,	0,  0,  0,	5   },  // [stat]
    {   7,	0,	0,	0,	6,	0,	0,	0,	0,	0,	0,	0,	10,	8,	0,	0,  0,  0,	9   },  // [command]
    {   0,	0,	0,	0,	0,	0,	11,	0,	12,	11,	0,	0,	0,	0,	11,	11,	11, 11, 12  },  // [func-assign-expr]
    {   0,	0,	0,	0,	0,	0,	0,	0,	13,	0,	0,	0,	0,	0,	0,	0,  0,  0,	14  },  // [assign-expr]
    {   15,	0,	0,	17,	15,	0,	16,	0,	0,	0,	0,	0,  15,	15,	0,	0,  0,  0,	15  },  // [end-list]
    {   18,	21,	20,	22,	18,	0,	19,	0,	0,	0,	0,	0,	18,	18,	0,	0,  0,  0,	18  },  // [if-list]
    {   0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	23,	24,	0,	0,  0,  0,	0   },  // [id-func]
    {   0,	0,	0,	0,	0,	0,	27,	0,	0,	25,	0,	0,	26,	0,	26,	26,	26, 26, 0   },  // [params-gen]
    {   0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	29,	0,	28,	0,	28,	28,	28, 28, 0   },  // [p-brackets]
    {   0,	0,  0,	0,	0,	0,	0,	0,	0,	0,	31,	30,	0,	0,	0,	0,  0,  0,	0   },  // [p-brackets-cont]
    {   0,	0,	0,	0,	0,	0,	33,	0,	0,	0,	0,	32,	0,	0,	0,	0,  0,  0,	0   },  // [p-without]
    {   0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	34,	0,	38,	35, 36, 37, 0   }   // [id]
};


char *reverted_rules[RULES_ROWS][RULES_COLS] = {
    {  NULL, },
    { "[st-list]", "[stat]", },
    { "[st-list]", "EOL", },
    {  NULL, }, // finishing rule: [st-list] -> epsilon and in next step succ = true
    { "EOL", "[end-list]", "[params-gen]", "[id-func]", "def", },
    { "[command]" },
    { "EOL", "[end-list]", "EOL", "do", "**expr**", "while", },
    { "EOL", "[if-list]", "EOL", "then", "**expr**", "if", },
    { "[params-gen]", "FUNC", },
    { "EOL", "**expr**", },
    { "[func-assignment-expr]", "ID", },
    { "[params-gen]", },
    { "[assignment-expr]", },
    { "EOL", "**expr**", "=", },
    { "EOL", "**expr**", },
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
    { "[p-without]", "[id]", },
    { "EOL", },
    { "[p-brackets-continue]", "[id]", },
    { "EOL", ")", },
    { "[p-brackets-continue]", "[id]", ",", },
    { "EOL", ")", },
    { "[p-without]", "[id]", ",", },
    { "EOL", },
    { "ID", },
    { "STR", },
    { "INT", },
    { "DBL", },
    { "nil", }
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