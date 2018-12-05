/**CFile****************************************************************

  FileName    [sa_prec.c]

  SystemName  [IFJ - PROJECT]

  PackageName [Syntactic analysis]

  Synopsis    []

  Author      [Lukas Piwowarski]

  Affiliation []

  Date        [01/11/2018]

  Revision    []

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                          INCLUDES                                ///
////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <stdbool.h>

#include "expr.h"

#include "token.h"
#include "scanner.h"
#include "code_gen.h"
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
char prec_table[PREC_TABLE_ROWS][PREC_TABLE_COLS] = {
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

#define handleError(err)            \
    if(err == ERR_SEM_TYPE)         \
        goto err_sem_type;          \
    else if(err == ERR_INTERNAL)    \
        goto err_sem_internal;      \
    else if(err == ERR_SYN)         \
        goto err_syn;               \

#define callFunc(is_builtin)                                                           \
do {                                                                                   \
    if(is_builtin)                                                                     \
    {                                                                                  \
        switch(is_builtin)                                                             \
        {                                                                              \
            case _inputs_: if(num_params != 0) goto err_sem_func;                      \
                break;                                                                 \
            case _inputi_: if(num_params != 0) goto err_sem_func;                      \
                break;                                                                 \
            case _inputf_: if(num_params != 0) goto err_sem_func;                      \
                break;                                                                 \
            case _length_: if(num_params != 1) goto err_sem_func;                      \
                break;                                                                 \
            case _chr_   : if(num_params != 1) goto err_sem_func;                      \
                break;                                                                 \
            case _ord_   : if(num_params != 2) goto err_sem_func;                      \
                break;                                                                 \
            case _substr_: if(num_params != 3) goto err_sem_func;                      \
                break;                                                                 \
            case _print_ : if(num_params == 0) goto err_sem_func;                      \
        }                                                                              \
        result = genFuncCode(func_stack, is_builtin, var_symtab, code_buffer, in_stat);\
    }                                                                                  \
    else                                                                               \
    {                                                                                  \
        if(!func_elem->func.is_defined)                                                \
            func_elem->func.n_params = num_params;                                     \
        else if(func_elem->func.n_params != num_params)                                \
            goto err_sem_func;                                                         \
                                                                                       \
        genFuncCode(func_stack, false, var_symtab, code_buffer, in_stat);              \
    }                                                                                  \
                                                                                       \
    func_called = true;                                                                \
} while(0);                                                                            \

bool isExprVar(token_t *token)
{
    if (strcmp(token->name, "INT") == 0 ||
            strcmp(token->name, "DBL") == 0 ||
            strcmp(token->name, "STR") == 0 ||
            strcmp(token->name, "nil") == 0)
        return true;

    return false;
}

char *retType(char *type, char *gl_lf)
{
    if (strcmp(type, "INT") == 0)
        return "int";
    else if (strcmp(type, "DBL") == 0)
        return "float";
    else if (strcmp(type, "STR") == 0)
        return "string";
    else if (strcmp(type, "nil") == 0)
        return "nil";
    else
        return gl_lf;

    return NULL;
}

token_t *genFuncCode(stack_tkn_t *stack, char is_builtin, symtable_t *symtable, list_t *code_buffer, bool in_stat)
{
    if (!is_builtin)
    {
        token_t *func = stcTkn_pop(stack);

        print_or_append(code_buffer, in_stat, "CREATEFRAME\n");
        token_t *param;
        int i = 1;
        char *val = NULL;

        while ((param = stcTkn_pop(stack)) != NULL)
        {
            val = isExprVar(param) ? param->info.string :
                  param->info.ptr->var.key;

            if(val == NULL)
                val = "nil";
            
            char *gl_lf = strcmp(symtable->name, "$GT") == 0 ?
                          "GF" :
                          "LF";

            print_or_append(code_buffer, in_stat,
                            "DEFVAR TF@%%%d\n"
                            "MOVE TF@%%%d %s@%s\n",
                            i,
                            i,
                            retType(param->name, gl_lf),
                            val
                           );

            i++;
            destroyToken(param);
        }

        if (is_builtin)
            print_or_append(code_buffer, in_stat, "CALL $%s\n", func->info.string);
        else
            print_or_append(code_buffer, in_stat, "CALL %s\n", func->info.ptr->func.key);

        destroyToken(func);
        return NULL;
    }
    else
    {
        token_t *result = NULL;
        token_t *func_name = stcTkn_pop(stack);
        token_t *array[3];
        destroyToken(func_name);
        switch (is_builtin)
        {
            case _inputs_: result = input(code_buffer, in_stat, symtable, 2);
                break;
            case _inputi_: result = input(code_buffer, in_stat, symtable, 0);
                break;
            case _inputf_: result = input(code_buffer, in_stat, symtable, 1);
                break;
            case _length_: array[0] = stcTkn_pop(stack);
                result = length(code_buffer, in_stat, symtable, array[0]);
                if (strcmp(result->name, "ERR_SEM") == 0)
                {
                    destroyToken(result);
                    return NULL;
                }
                break;
            case _chr_   : array[0] = stcTkn_pop(stack);
                result = chr(code_buffer, in_stat, symtable, array[0]);
                if (strcmp(result->name, "ERR_SEM") == 0)
                {
                    destroyToken(result);
                    return NULL;
                }
                break;
            case _ord_   : array[0] = stcTkn_pop(stack);
                array[1] = stcTkn_pop(stack);
                result = ord(code_buffer, in_stat, symtable, array[0], array[1]);
                if (strcmp(result->name, "ERR_SEM") == 0)
                {
                    destroyToken(result);
                    return NULL;
                }
                break;
            case _substr_: array[0] = stcTkn_pop(stack);
                array[1] = stcTkn_pop(stack);
                array[2] = stcTkn_pop(stack);
                result = substr(code_buffer, in_stat, symtable, array[0], array[1], array[2]);
                if (strcmp(result->name, "ERR_SEM") == 0)
                {
                    destroyToken(result);
                    return NULL;
                }
                break;
            case _print_ : result = print(symtable, stack, code_buffer, in_stat);
                if (strcmp(result->name, "ERR_SEM") == 0)
                {
                    destroyToken(result);
                    return NULL;
                }
                break;
        }

        return result;
    }

}

/*
 * @brief Returns type of builtin function
 *
 * @param  token  Token with name of the function (token->name == "FUNC")
 * @return Type of builtin function. If token contains invalid name of 
 *         function returns INVALID_TOKEN.
 */
builtin_func_t getBuiltinFuncType(token_t *token)
{
    if(strcmp(token->info.string, "inputs") == 0)
        return _inputs_;
    else if(strcmp(token->info.string, "inputi") == 0)
        return _inputi_;
    else if(strcmp(token->info.string, "inputf") == 0)
        return _inputf_;
    else if(strcmp(token->info.string, "length") == 0)
        return _length_;
    else if(strcmp(token->info.string, "chr") == 0)
        return _chr_;
    else if(strcmp(token->info.string, "ord") == 0)
        return _ord_;
    else if(strcmp(token->info.string, "substr") == 0)
        return _substr_;
    else if(strcmp(token->info.string, "print") == 0)
        return _print_;

    return INVALID_TOKEN;
}

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

bool isIdentifier(token_t *token, table_elem_t term)
{
    return (term == _id_ && (strcmp(token->name, "ID") == 0)) || 
           (strcmp(token->name, "FUNC")) == 0;
}

bool inGlobalFrame(symtable_t *var_symtab, symtable_t *func_symtab)
{
    return strcmp(var_symtab->name, "$GT") == 0 && func_symtab->name == NULL;
}

bool isOperator(table_elem_t term)
{
    if (term == _plus_)
        return true;
    else if (term == _mins_)
        return true;
    else if (term == _mult_)
        return true;
    else if (term == _div_)
        return true;
    else if (term == _rel_)
        return true;

    return false;
}

int destroyTokenArr(token_t *token_arr [], int state)
{
    switch (state)
    {
        case 0:
            break;
        case 1:
            destroyToken(token_arr[0]);
            break;
        case 2:
            destroyToken(token_arr[0]);
            destroyToken(token_arr[1]);
            break;
        case 3:
            destroyToken(token_arr[0]);
            destroyToken(token_arr[1]);
            destroyToken(token_arr[2]);
            break;
    }

    return SUCCESS;
}

int Check_err(token_t *token, token_t *token_arr[], int state, table_elem_t term,
              table_elem_t correct_term) {

    if (strcmp(token->name, "ERR_SEM") == 0)
    {
        destroyTokenArr(token_arr, state);
        destroyToken(token);
        return ERR_SEM_TYPE;
    }
    else if (strcmp(token->name, "ERR_INT") == 0)
    {
        destroyTokenArr(token_arr, state);
        destroyToken(token);
        return ERR_INTERNAL;
    }
    else if (term != correct_term)
    {
        destroyTokenArr(token_arr, state);
        return ERR_SYN;
    }

    return SUCCESS;
}

int idOperandId(stack_sa_t *stack, symtable_t *var_symtab, list_t *code_buffer, 
                bool in_stat, table_elem_t operator) {

    token_t *ptr_token[3] = {NULL};
    int err = 0;
    table_elem_t term;

    ptr_token[0] = stc_tokPopTop(stack, &term);
    if((err = Check_err(ptr_token[0], ptr_token, 1, term, _E_)) != SUCCESS)
      return err;

    ptr_token[1] = stc_tokPopTop(stack, &term);
    if((err = Check_err(ptr_token[1], ptr_token, 2, term, operator)) != SUCCESS)
      return err;

    ptr_token[2] = stc_tokPopTop(stack, &term);
    if((err = Check_err(ptr_token[2], ptr_token, 3, term, _E_)) != SUCCESS)
      return err;

    term = stc_popTop(stack);
    if (term != _sml_)
      return ERR_SYN;

    token_t *result = gen_expr(ptr_token[1], ptr_token[2], ptr_token[0], var_symtab, 
                               code_buffer, in_stat);

    if((err = Check_err(result, ptr_token, 0, 0, 0)) != SUCCESS)
      return err;

    stc_push(stack, _E_, result);
    return SUCCESS;
}

bool isNonTerm(table_elem_t term)
{
    if (term == _E_)
        return true;
    else if (term == _F_)
        return true;

    return false;
}

bool detectSucEnd(stack_sa_t *stack, table_elem_t token)
{

    return (isNonTerm(stack->top->term)
            && stack->top->lptr->term == _empt_
            && token == _empt_);
}

void pushToStack(token_t *token, list_t *code_buffer, bool in_stat, symtable_t *symtab)
{

    if (strcmp(token->name, "ID") == 0)
    {
        if (token->info.ptr == NULL)
        {
            print_or_append(code_buffer, in_stat, "PUSHS GF@$des\n");
        }
        else
        {    
            print_or_append(code_buffer, in_stat, "PUSHS %s@%s\n",
                           (strcmp(symtab->name, "$GT") == 0) ? "GF" : "LF",
                            token->info.ptr->var.key);
        }
    }
    else if(strcmp(token->name, "INT") == 0)
    {
        print_or_append(code_buffer, in_stat, "PUSHS int@%s\n",
                        token->info.string);
    }
    else if(strcmp(token->name, "DBL") == 0)
    {
        print_or_append(code_buffer, in_stat, "PUSHS float@%s\n",
                        token->info.string);
    }
    else if(strcmp(token->name, "STR") == 0)
    {
        print_or_append(code_buffer, in_stat, "PUSHS string@%s\n",
                        token->info.string);
    }
    else if(strcmp(token->name, "nil") == 0)
    {
       print_or_append(code_buffer, in_stat, "PUSHS nil@nil\n"); 
    } 
    else
    {
        print_or_append(code_buffer, in_stat, "PUSHS GF@$des\n");
    }  

    return;
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
int sa_prec(dynamicStr_t *sc_str, queue_t *que, symtable_t *var_symtab, 
            symtable_t *func_symtab, token_t **ret_token, list_t *code_buffer, 
            bool in_stat) {

    /* Create main stack for Operator-Precedence Parser */
    stack_sa_t *prec_stack = stc_init();   
    token_info_t info;
    stc_push(prec_stack, _empt_, createToken("$", info));

    /* Create stack for function parameters */
    stack_tkn_t *func_stack = stcTkn_create();

    /* Read first token */
    int err_code = 0;
    token_t *scanner_token = scanner_get(sc_str, que);
    if((err_code = errTokenCheck(scanner_token)) != SUCCESS)
      return err_code;

    bool detect_func = false;
    bool func_called = false;
    elem_t *var_elem = NULL;
    elem_t *func_elem = NULL;
    int n_loops = 0;                /* Counter of cycles             */
    int err = 0;                    /* Checks error return codes     */
    int num_params = 0;             /* Number of params in function  */
    token_t *result = NULL;
    builtin_func_t builtin_func_type = 0;     

    table_elem_t stack_top_term;    /* Term on the top of prec_stack */
    table_elem_t input_token;       /* Token from scanner            */
    while(true)
    {
        stc_print(prec_stack);
        stack_top_term = stc_topTerm(prec_stack);
        input_token = getTermIndex(scanner_token);
        if((int)input_token == INVALID_TOKEN)
        {
            if(strcmp(scanner_token->name, "ERR_LEX") == 0)
                goto err_lex;
            else if(strcmp(scanner_token->name, "ERR_INTERNAL") == 0)
                goto err_internal;

            goto err_syn;
        }
        if(input_token == _func_)
        {
            detect_func = true;
            builtin_func_type = getBuiltinFuncType(scanner_token);
        }

        /* When identifier is read -> check if defined */
        if(isIdentifier(scanner_token, input_token))
        {
            /* Check if identifier is in function or variable symtable */
            var_elem = symtab_find(var_symtab, sc_str->str);
            if(!detect_func)
            {
                func_elem = symtab_find(func_symtab, sc_str->str);
                scanner_token->info.ptr = func_elem;
            }

            if(var_elem != NULL)        /* If identifier is variable    */
            {
                input_token = _id_;
                scanner_token->info.ptr = var_elem;
            }
            else if(func_elem != NULL)  /* If identifier is function    */
            {
                if(detect_func || n_loops)
                  goto err_sem_undef;

                input_token = _func_;
                scanner_token->info.ptr = func_elem;
                detect_func = true;
            }
            else                        /* If identifier is not defined */
            {
                if(inGlobalFrame(var_symtab, func_symtab))
                  goto err_sem_undef;

                if(detect_func || n_loops)
                  goto err_sem_undef;

                /* Potential functions -> add to function symtable */
                func_elem = symtab_elem_add(func_symtab, sc_str->str);
                func_elem->func.is_defined = false;

                input_token = _func_;
                scanner_token->info.ptr = func_elem;
                detect_func = true;
            }
        }

        n_loops++;

        char rule = prec_table[stack_top_term][input_token];
        if(rule == 'X')
        {
            goto err_syn;
        }
        else if (rule == '=')
        {
            if(func_called)
              goto err_syn;

            stc_push(prec_stack, input_token, scanner_token);
        }
        else if(rule == '<')
        {
            if(func_called)
              goto err_syn;

            if(isOperator(input_token) && detect_func)
              goto err_syn;

            stc_pushAfter(prec_stack, stack_top_term, _sml_);
            stc_push(prec_stack, input_token, scanner_token);
        }
        else if(rule == '>')
        {
            token_t *ptr_token[3] = {NULL};
            table_elem_t stack_term;

            if(!detect_func)
            {
                switch(stack_top_term)
                {
                    case _id_:
                      ptr_token[0] = stc_tokPopTop(prec_stack, &stack_term);
                      if (stack_term != _id_)
                      {
                          destroyToken(ptr_token[0]);
                          goto err_syn;
                      }

                      stack_term = stc_popTop(prec_stack);
                      if (stack_term != _sml_)
                          goto err_syn;

                      stc_push(prec_stack, _E_, ptr_token[0]);
                      break;

                    case _rbrc_:
                      stack_term = stc_popTop(prec_stack);
                      if (stack_term != _rbrc_)
                          goto err_syn;

                      ptr_token[0] = stc_tokPopTop(prec_stack, &stack_term);
                      if (stack_term != _E_)
                      {
                          destroyToken(ptr_token[0]);
                          goto err_syn;
                      }

                      stack_term = stc_popTop(prec_stack);
                      if (stack_term != _lbrc_)
                          goto err_syn;

                      stack_term = stc_popTop(prec_stack);
                      if (stack_term != _sml_)
                          goto err_syn;

                      stc_push(prec_stack, _E_, ptr_token[0]);
                      break;

                    case _plus_:
                      if((err = idOperandId(prec_stack, var_symtab,
                                            code_buffer, in_stat,
                                            _plus_)) != SUCCESS) 
                      {
                          handleError(err);
                      }
                      break;

                    case _mult_:
                      if((err = idOperandId(prec_stack, var_symtab,
                                            code_buffer, in_stat,
                                            _mult_)) != SUCCESS) 
                      {
                          handleError(err);
                      }
                      break;

                    case _mins_:
                      if((err = idOperandId(prec_stack, var_symtab,
                                            code_buffer, in_stat,
                                            _mins_)) != SUCCESS) 
                      {
                          handleError(err);
                      }
                      break;

                    case _div_:
                      if((err = idOperandId(prec_stack, var_symtab,
                                            code_buffer, in_stat,
                                            _div_)) != SUCCESS) 
                      {
                          handleError(err);
                      }

                      break;

                    case _rel_:
                      if((err = idOperandId(prec_stack, var_symtab,
                                            code_buffer, in_stat, 
                                            _rel_)) != SUCCESS) 
                      {
                          handleError(err);
                      }
                      break;

                    default:
                      goto err_syn;
                      break;
                }
            }
            else
            {

                switch(stack_top_term)
                {
                    case _id_:
                      ptr_token[0] = stc_tokPopTop(prec_stack, &stack_term);
                      if (stack_term != _id_)
                      {
                          destroyToken(ptr_token[0]);
                          goto err_syn;
                      }

                      stack_term = stc_popTop(prec_stack);
                      if (stack_term != _sml_)
                          goto err_syn;

                      stc_push(prec_stack, _E_, ptr_token[0]);
                      num_params++;
                      break;

                    /* F -> f
                     * F -> f E
                     */
                    case _func_:
                      ptr_token[0] = stc_tokPopTop(prec_stack, &stack_term);
                      if (stack_term == _E_)
                      {
                          stcTkn_push(func_stack, ptr_token[0]);

                          ptr_token[0] = stc_tokPopTop(prec_stack, &stack_term);
                          stcTkn_push(func_stack, ptr_token[0]);
                          if (stack_term != _func_)
                              goto err_syn;
                      }
                      else if (stack_term == _func_)
                      {
                          stcTkn_push(func_stack, ptr_token[0]);
                      }
                      else
                      {
                          goto err_syn;
                      }

                      stack_term = stc_popTop(prec_stack);
                      if (stack_term != _sml_)
                          goto err_syn;

                      callFunc(builtin_func_type);       
                      stc_push(prec_stack, _F_, result);
                      break;

                    /* F -> f E, ... , E */  
                    case _coma_:
                      while (42)
                      {
                          ptr_token[0] = stc_tokPopTop(prec_stack, &stack_term);
                          stcTkn_push(func_stack, ptr_token[0]);
                          if (stack_term != _E_)
                             goto err_syn;

                          stack_term = stc_popTop(prec_stack);
                          if (stack_term == _sml_)
                              break;
                          else if (stack_term != _coma_)
                              goto err_syn;
                      }

                      ptr_token[0] = stc_tokPopTop(prec_stack, &stack_term);
                      stcTkn_push(func_stack, ptr_token[0]);
                      if (stack_term != _func_)
                          goto err_syn;

                      stack_term = stc_popTop(prec_stack);
                      if (stack_term != _sml_)
                          goto err_syn;

                      callFunc(builtin_func_type);         
                      stc_push(prec_stack, _F_, result);     
                      break;

                    case _rbrc_:
                      stack_term = stc_popTop(prec_stack);
                      if(stack_term != _rbrc_)
                          goto err_syn;

                      ptr_token[0] = stc_tokPopTop(prec_stack, &stack_term);
                      if(stack_term == _lbrc_)
                      {
                          destroyToken(ptr_token[0]);

                          ptr_token[0] = stc_tokPopTop(prec_stack, &stack_term);
                          stcTkn_push(func_stack, ptr_token[0]);
                          if(stack_term != _func_)
                              goto err_syn;

                          stack_term = stc_popTop(prec_stack);
                          if (stack_term != _sml_)
                              goto err_syn;

                          callFunc(builtin_func_type);
                          stc_push(prec_stack, _F_, result);

                          break;  /* F -> f () */
                      }
                      else if(stack_term == _E_)
                      {
                          stcTkn_push(func_stack, ptr_token[0]);

                          stack_term = stc_popTop(prec_stack);
                          if(stack_term == _lbrc_)
                          {
                              ptr_token[0] = stc_tokPopTop(prec_stack, &stack_term);
                              stcTkn_push(func_stack, ptr_token[0]);
                              if(stack_term != _func_)
                                goto err_syn;

                              stack_term = stc_popTop(prec_stack);
                              if(stack_term != _sml_)
                                goto err_syn;

                              callFunc(builtin_func_type);
                              stc_push(prec_stack, _F_, result);

                              break; /* F -> f (E) */
                          }
                          else if(stack_term == _coma_)
                          {
                              while(42)
                              {
                                  ptr_token[0] = stc_tokPopTop(prec_stack, &stack_term);
                                  stcTkn_push(func_stack, ptr_token[0]);
                                  if(stack_term != _E_)
                                      goto err_syn;

                                  stack_term = stc_popTop(prec_stack);
                                  if(stack_term == _lbrc_)
                                     break;  /* while() */
                                  else if(stack_term != _coma_)
                                      goto err_syn;

                              }

                              ptr_token[0] = stc_tokPopTop(prec_stack, &stack_term);
                              stcTkn_push(func_stack, ptr_token[0]);
                              if(stack_term != _func_)
                                  goto err_syn;

                              stack_term = stc_popTop(prec_stack);
                              if(stack_term != _sml_)
                                  goto err_syn;

                              callFunc(builtin_func_type);
                              stc_push(prec_stack, _F_, result);

                              break; /* F -> f (E, E, ..., E) */
                      }
                    }
                      break;

                    default:
                      goto err_syn;
                      break; 

                }
            }

            if(detectSucEnd(prec_stack, input_token))
            {
                if(detect_func)
                {
                    if(builtin_func_type)
                    {
                        if(result == NULL)
                            goto err_sem_type;

                        *ret_token = stc_tokPopTop(prec_stack, &stack_term);
                    }
                    else
                    {

                        token_info_t info;
                        *ret_token = createToken("%%retval", info);
                    }
                }
                else
                {     

                    token_t *tmp = stc_tokPopTop(prec_stack, &stack_term);
                    *ret_token = tmp;
                    pushToStack(*ret_token, code_buffer, in_stat, var_symtab);
                }

                stcTkn_destroy(func_stack);
                stc_destroy(prec_stack);
                scanner_unget(que, scanner_token, sc_str->str);
                

                return SUCCESS;
            }
            else
              continue;
        }

        scanner_token = scanner_get(sc_str, que);
    }
    return SUCCESS;

err_syn:
  stcTkn_destroy(func_stack);
  stc_destroy(prec_stack);
  destroyToken(scanner_token);
  return ERR_SYN;

err_lex:
  stcTkn_destroy(func_stack);
  stc_destroy(prec_stack);
  destroyToken(scanner_token);
  return ERR_LEX;

err_internal:
  stcTkn_destroy(func_stack);
  stc_destroy(prec_stack);
  destroyToken(scanner_token);
  return ERR_INTERNAL;

err_sem_undef:
  stcTkn_destroy(func_stack);
  stc_destroy(prec_stack);
  destroyToken(scanner_token);
  return ERR_SEM_UNDEF;

err_sem_type:
  stcTkn_destroy(func_stack);
  stc_destroy(prec_stack);
  destroyToken(scanner_token);
  return ERR_SEM_TYPE;

err_sem_internal:
  stcTkn_destroy(func_stack);
  stc_destroy(prec_stack);
  destroyToken(scanner_token);
  return ERR_INTERNAL;

err_sem_func:
  stcTkn_destroy(func_stack);
  stc_destroy(prec_stack);
  destroyToken(scanner_token);
  return ERR_SEM_FUNC;

}

