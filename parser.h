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

#include "stack_tkn.h"
#include "stack_str.h"
#include "queue.h"
#include "dynamic_str.h"
#include "dynamic_arr_param.h"
#include "list.h"

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
    P_BRACKETS_24,
    P_BRACKETS_25,
    P_BRACKETS_CONT_27,
    P_BRACKETS_CONT_28,
} rules_t;


////////////////////////////////////////////////////////////////////////
///                      MACRO DEFINITIONS                           ///
////////////////////////////////////////////////////////////////////////

#define EXPR_INCLUDE -1
#define EXPR_INCLUDE_TWO -2

#define LL_ROWS 11
#define LL_COLS 15

#define RULES_ROWS 28
#define RULES_COLS 6


////////////////////////////////////////////////////////////////////////
///                       GLOBAL VARIABLES                           ///
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
///                    FUNCTION DECLARATIONS                         ///
////////////////////////////////////////////////////////////////////////

/**
 * @brief Find number of corresponding rule in ll_table
 *
 * @param nonterm   name of nonterminal
 * @param term      name of terminal
 * @return int      number of rule
 */
int LLtableFind(char *nonterm, char *term);

/**
 * @brief Cretes a temporary key (either identifier or function key)
 * 
 * @param key_tmp   temporary key
 * @param name      name of new temporary key
 * @return true     if success
 * @return false    if internal error
 */ 
bool tempKey_create(char **key_tmp, char *name);


/**
 * @brief Destroys temporary key (either identifier or function key)
 * 
 * @param key_tmp key to be destroyed
 */
void tempKey_destroy(char **key_tmp);


/**
 * @brief Pushes reverted rules to the stack of tokens 
 * 
 * @param stack_tkn stack of tokens
 * @param rule      rule which is going to be used
 * @return true     if success
 * @return false    if internal error
 */
bool pushRevertedRules(stack_tkn_t *stack_tkn, int rule);


/**
 * @brief Frees all structures that were allocated in parser
 * 
 * @param gl_var_tab    global variables symtable
 * @param fun_tab       function symtable
 * @param lc_var_tab    local variables symtable
 * @param param_arr     dynamic array of a function parameters
 * @param id_key_tmp    temporary key of an identifier
 * @param func_key_tmp  temporary key of a function
 * @param act           active token from Scanner
 * @param token         temporary token
 * @param id_token_tmp  temporary token of an identifier
 * @param sa_prec_ret   temporary token from Precedenc analysis
 */
void freeAll(symtable_t *gl_var_tab, symtable_t *fun_tab, symtable_t *lc_var_tab,
             dynamicArrParam_t *param_arr,
             char **id_key_tmp, char **func_key_tmp,
             token_t *act, token_t *token, token_t *id_token_tmp, token_t *sa_prec_ret);



/**
 * @brief Parser - the core of compiler
 * 
 * @param stack_tkn     stack of tokens
 * @param stack_str     stack of strings
 * @param code_buffer   buffer for code when in if-statment or while-loop
 * @param defvar_buffer buffer for variable definitions when in if-statment or while-loop
 * @param sc_str        dynamic string for Scanner
 * @param que           queue of tokens for Scanner
 * @return int          Exit code
 */
int parser(stack_tkn_t *stack_tkn, stack_str_t *stack_str, list_t *code_buffer, list_t *defvar_buffer, dynamicStr_t *sc_str, queue_t *que);




/**
 * @brief Checks if a function was defined
 * 
 * @param element symtable element -- function
 * @return true   if function is defined   
 * @return false  if function is not defined
 */
static inline bool checkFun(elem_t *element)
{
    return element->func.is_defined;
}


/**
 * @brief Checks if symbol is terminal
 * 
 * @param name  symbol to be checked
 * @return int  1 if its terminal symbol
 *              0 if its nonterminal symbol
 */
static inline int isTerminal(char *name)
{
    return name[0] != '[';
}


/**
 * @brief Checks if symbol belongs to the category "expression"
 * 
 * @param term  symbol to by checked
 * @return int  1 if its in category "expression"
 *              0 if not      
 */
static inline int isExpr(char *term)
{
    return (strcmp(term, "+") == 0 ||
            strcmp(term, "-") == 0 ||
            strcmp(term, "(") == 0 ||
            strcmp(term, "FUNC") == 0 ||
            strcmp(term, "STR") == 0 ||
            strcmp(term, "INT") == 0 ||
            strcmp(term, "DBL") == 0 ||
            strcmp(term, "nil") == 0 ||
            strcmp(term, "BUILTIN") == 0);
}


#endif
////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////