/**CFile****************************************************************

  FileName    [sa_prec.c]

  SystemName  [IFJ - PROJECT]

  PackageName [Syntactic analysis]

  Synopsis    []

  Author      [Lukas Valek, Adam Pankuch, Jindrich Sestak, 
               Lukas Piwowarski]

  Affiliation []

  Date        [01/11/2018]

  Revision    []

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                          INCLUDES                                ///
////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <stdbool.h>

#include "sa_prec.h"

#include "token.h"
#include "scanner.h"
#include "code_gen.h"
#include "builtin_gen.h"
#include "stack_sa_prec.h"
#include "stack_tkn.h"
#include "error.h"

////////////////////////////////////////////////////////////////////////
///                         BASIC TYPES                              ///
////////////////////////////////////////////////////////////////////////

/* Number of parameters of built-in functions */
typedef enum {
  _inputs_ = 1,       // 0 param
  _inputi_,           // 0 param
  _inputf_,           // 0 param
  _length_,           // 1 param
  _chr_,              // 1 param
  _ord_,              // 2 params
  _substr_,           // 3 params
  _print_,            // X params
} builtin_func_t;

////////////////////////////////////////////////////////////////////////
///                      MACRO DEFINITIONS                           ///
////////////////////////////////////////////////////////////////////////

#define PREC_TABLE_ROWS 17   /* Max number of rows in prec_table     */
#define PREC_TABLE_COLS 17   /* Max number of columns in prec_table  */

#define INVALID_TOKEN -1     /* Token which should not occur in expr */

////////////////////////////////////////////////////////////////////////
///                       GLOBAL VARIABLES                           ///
////////////////////////////////////////////////////////////////////////

/* Precedence-table */
char sa_prec_table[PREC_TABLE_ROWS][PREC_TABLE_COLS] = {
/*          +      *     (     )     i     -     /     rel    str    f    ,    $  */
/*  +  */{ '>' , '<' , '<' , '>' , '<' , '>' , '<' ,  '>' , '<' , '<' , '>' , '>' }, 
/*  *  */{ '>' , '>' , '<' , '>' , '<' , '>' , '>' ,  '>' , 'X' , '<' , '>' , '>' },
/*  (  */{ '<' , '<' , '<' , '=' , '<' , '<' , '<' ,  '<' , 'X' , '<' , '=' , 'X' },
/*  )  */{ '>' , '>' , 'X' , '>' , 'X' , '>' , '>' ,  '>' , 'X' , 'X' , '>' , '>' },
/*  i  */{ '>' , '>' , 'X' , '>' , 'X' , '>' , '>' ,  '>' , 'X' , 'X' , '>' , '>' },
/*  -  */{ '>' , '<' , '<' , '>' , '<' , '>' , '<' ,  '>' , 'X' , 'X' , '>' , '>' },
/*  /  */{ '>' , '>' , '<' , '>' , '<' , '>' , '>' ,  '>' , 'X' , 'X' , '>' , '>' },
/* rel */{ '<' , '<' , '<' , '>' , '<' , '<' , '<' ,  'X' , '<' , 'X' , '>' , '>' },
/* str */{ '>' , 'X' , 'X' , 'X' , 'X' , 'X' , 'X' ,  '>' , 'X' , 'X' , 'X' , '>' },
/*  f  */{ 'X' , 'X' , '=' , 'X' , '<' , 'X' , 'X' ,  'X' , 'X' , 'X' , '<' , '>' },
/*  ,  */{ '<' , '<' , '<' , '=' , '<' , '<' , '<' ,  '<' , '<' , '<' , '=' , '>' },
/*  $  */{ '<' , '<' , '<' , 'X' , '<' , '<' , '<' ,  '<' , '<' , '<' , 'X' , 'X' }
};

////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
////////////////////////////////////////////////////////////////////////

/**
 * @brief  If token indicates end of expression (EOL, then, do)
 *
 * @param  token  Token to be analysed
 * @return true   If token is 'EOL', 'then' or 'do'
 *         false  If token is NOT 'EOL', 'then' or 'do'
 */
bool sa_isEndToken(token_t *token)
{
    if(strcmp(token->name, "EOL") == 0)
        return true;
    else if(strcmp(token->name, "then") == 0)
        return true;
    else if(strcmp(token->name, "do") == 0)
        return true;
    else if(strcmp(token->name, "EOF") == 0)
        return true;

    return false;
}

/*
 * @brief Checks token whether indicates an error. If it does, destroys 
 *        the token.
 *
 * @params  token    Token to be checked 
 * @return  Error constant defined in error.h
 */
int errTokenCheck(token_t *token)
{
    if(strcmp(token->name, "ERR_INTERNAL") == 0)
    {
      destroyToken(token);
      return ERR_INTERNAL;
    }
    else if(strcmp(token->name, "ERR_LEX") == 0)
    {
      destroyToken(token);
      return ERR_LEX;
    }

    return SUCCESS;
}

/* 
 * @brief Returns index to Precedence-Table.
 *
 * @param  token  Input token
 * @return Index to Precedence-Table.
 */
table_elem_t getTermIndex(token_t *token)
{
    if(strcmp(token->name, "+") == 0)
        return _plus_;
    else if(strcmp(token->name, "*") == 0)
        return _mult_;
    else if(strcmp(token->name, "ID") == 0)
        return _id_;
    else if(strcmp(token->name, "INT") == 0)
        return _id_;
    else if(strcmp(token->name, "DBL") == 0)
        return _id_;
    else if(strcmp(token->name, "nil") == 0)
        return _id_;
    else if(strcmp(token->name, "(") == 0)
        return _lbrc_;
    else if(strcmp(token->name, ")") == 0)
        return _rbrc_;
    else if(strcmp(token->name, "-") == 0)
        return _mins_;
    else if(strcmp(token->name, "/") == 0)
        return _div_;
    else if(strcmp(token->name, "STR") == 0)
        return _id_;
    else if(strcmp(token->name, "<=") == 0)
        return _rel_;
    else if(strcmp(token->name, ">=") == 0)
        return _rel_;
    else if(strcmp(token->name, "==") == 0)
        return _rel_;
    else if(strcmp(token->name, "!=") == 0)
        return _rel_;
    else if(strcmp(token->name, "<") == 0)
        return _rel_;
    else if(strcmp(token->name, ">") == 0)
        return _rel_;
    else if(strcmp(token->name, ",") == 0)
        return _coma_;
    else if(strcmp(token->name, "BUILTIN") == 0)
        return _func_;
    else if(strcmp(token->name, "FUNC") == 0)
        return _id_;
    else if(sa_isEndToken(token))
        return _empt_;

    return INVALID_TOKEN;
}
/* 
 * @brief Operator-precedence parser
 *
 * @params  sc_str    Param for scanner
 *          que       Param for scanner
 *          symtable  Param for scanner
 * @return  true      If analysed expression is correct  
 *          false     If analysed expression is incorrect 
 */  
int sa_prec(dynamicStr_t *sc_str, queue_t *que, symtable_t *loc_symtab, 
            symtable_t *func_symtab) {

    /* Create main stack for Operator-Precedence Parser */
    stack_sa_t *prec_stack = stc_init();   
    token_info_t info;
    stc_push(prec_stack, _empt_, createToken("$", info));

    /* Read first token */
    int err_code = 0;
    token_t *scanner_token = scanner_get(sc_str, que);
    if((err_code = errTokenCheck(scanner_token)) != SUCCESS)
      return err_code;

    bool detect_func = false;
    builtin_func_t type_builtin;    /* 

    table_elem_t stack_top_term;    /* Term on the top of prec_stack */
    table_elem_t input_token;       /* Token from scanner            */
    while(true)
    {
        stack_top_term = stc_topTerm(prec_stack);
        input_token = getTermIndex(scanner_token);
        if(input_token == INVALID_TOKEN)
          goto err_syn;
        if(input_token == _func_)
        {
            detect_func = true;

        }

    }
    return SUCCESS;

err_syn:
  stc_destroy(prec_stack);
  destroyToken(scanner_token);
  return ERR_SYN;
}

