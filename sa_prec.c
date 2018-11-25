/**CFile****************************************************************

  FileName    [sa_prec.c]

  SystemName  [IFJ - PROJECT]

  PackageName [Syntactic analysis]

  Synopsis    []

  Author      [Lukas Piwowarski, Lukas Valek]

  Affiliation []

  Date        [01/11/2018]

  Revision    []

***********************************************************************/
/* 
 * TODO:
 *  
 */
////////////////////////////////////////////////////////////////////////
///                          INCLUDES                                ///
////////////////////////////////////////////////////////////////////////
#include "token.h"
#include "scanner.h"
#include "sa_prec.h"
#include "stack_sa_prec.h"
#include "error.h"
#include "stack_tkn.h"
#include "code_gen.h"
#include "builtin_gen.h"
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

////////////////////////////////////////////////////////////////////////
///                       GLOBAL VARIABLES                           ///
////////////////////////////////////////////////////////////////////////
#define INVALID_TOKEN -1
#define TEST_FUNC 1

//#define SA_PREC_PRINT 0
#ifdef SA_PREC_PRINT
#define DEBUG_PRINT(...) do{ printf( __VA_ARGS__ ); } while(0)
#else
#define DEBUG_PRINT(...) do{ } while(0)
#endif


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

#define handleError(err)            \
    if(err == ERR_SEM_TYPE)         \
        goto sem_gen;               \
    else if(err == ERR_INTERNAL)    \
        goto sem_internal;          \
    else if(err == ERR_SYN)         \
        goto fail_end;              \

#define callFunc(is_builtin)                                    \
do {                                                            \
    if(is_builtin)                                              \
    {                                                           \
        DEBUG_PRINT("Je to builtin: %d\n", is_builtin);         \
        switch(is_builtin)                                      \
        {                                                       \
            case _inputs_: if(num_params != 0) goto sem_fail;   \
                break;                                          \
            case _inputi_: if(num_params != 0) goto sem_fail;   \
                break;                                          \
            case _inputf_: if(num_params != 0) goto sem_fail;   \
                break;                                          \
            case _length_: if(num_params != 1) goto sem_fail;   \
                break;                                          \
            case _chr_   : if(num_params != 1) goto sem_fail;   \
                break;                                          \
            case _ord_   : if(num_params != 2) goto sem_fail;   \
                break;                                          \
            case _substr_: if(num_params != 3) goto sem_fail;   \
                break;                                          \
            case _print_ : if(num_params == 0) goto sem_fail;   \
        }                                                       \
        result = sa_callFunc(tok_stack, is_builtin, loc_symtab);\
    }                                                           \
    else                                                        \
    {                                                           \
        if(!func_elem->func.is_defined)                         \
            func_elem->func.n_params = num_params;              \
        else if(func_elem->func.n_params != num_params)         \
            goto sem_fail;                                      \
                                                                \
        sa_callFunc(tok_stack, false, loc_symtab);              \
    }                                                           \
                                                                \
    func_read = 1;                                              \
} while(0);                                                     \

static inline int destroyTokenArr(token_t *token_arr [], int state)
{
    switch(state)
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
    DEBUG_PRINT("=>Check_err: %s\n", token->name);
    if(strcmp(token->name, "ERR_SEM") == 0)
    {
        destroyTokenArr(token_arr, state);
        destroyToken(token);
        return ERR_SEM_TYPE;
    }
    else if(strcmp(token->name, "ERR_INT") == 0)
    {
        destroyTokenArr(token_arr, state);
        destroyToken(token);
        return ERR_INTERNAL;
    }
    else if(term != correct_term)
    {
        destroyTokenArr(token_arr, state);
        return ERR_SYN;
    } 

    return SUCCESS;
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

char sa_getTokenIndex(token_t *token)
{
    //printf("=> sa_getTokenIndex: %s\n", token->name);
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
    else if(sa_isEndToken(token))
        return _empt_;

    return INVALID_TOKEN;
}

int sa_builtinType(token_t *token)
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

static inline bool sa_isNonTerm(table_elem_t term)
{
    if(term == _E_)
        return true;
    else if(term == _L_)
        return true;
    else if(term == _S_)
        return true;
    else if(term == _F_)
        return true;

    return false;
}

static inline bool sa_detectSucEnd(stack_sa_t *stack, table_elem_t token)
{
    return (sa_isNonTerm(stack->top->term) 
            && stack->top->lptr->term == _empt_ 
            && token == _empt_);
}

static inline bool sa_isExprVar(token_t *token)
{
    if(strcmp(token->name, "INT") == 0 ||
       strcmp(token->name, "DBL") == 0 ||
       strcmp(token->name, "STR") == 0)
        return true;

    return false;
}

char *strToLower(char str[])
{
    int i = 0;
    while(str[i])
    {
        str[i] = tolower(str[i]);
        i++;
    }
    return str;
}

char *strToUpper(char str[])
{
    int i = 0;
    while(str[i])
    {
        str[i] = toupper(str[i]);
        i++;
    }
    return str;
}

token_t *sa_callFunc(stack_tkn_t *stack, char is_builtin, symtable_t *symtable)
{
    DEBUG_PRINT("=>sa_callFunc: %d\n", is_builtin);
    if(!is_builtin)
    {
        token_t *func = stcTkn_pop(stack);

        printf("CREATEFRAME\n");
        token_t *param;
        int i = 1;
        char *val = NULL;

        while((param = stcTkn_pop(stack)) != NULL)
        {
            val = sa_isExprVar(param) ? param->info.string : 
                                        param->info.ptr->var.key;
                                        
            printf("DEFVAR TF@%%%d\n"
                   "MOVE TF@%%%d %s@%s\n",
                    i,
                    i,
                    strToLower(param->name),
                    val
                  );

            strToUpper(param->name);
            i++;
            destroyToken(param);
        }

        if(is_builtin)
            printf("CALL $%s\n", func->info.string);
        else
            printf("CALL %s\n", func->info.ptr->func.key);

        destroyToken(func);
        return NULL;
    }
    else
    {
        token_t *result = NULL;
        token_t *func_name = stcTkn_pop(stack);
        token_t *array[3];
        free(func_name);
        switch(is_builtin)
        {
            case _inputs_: result = input(symtable, 2);
                break;
            case _inputi_: result = input(symtable, 0);
                break;
            case _inputf_: result = input(symtable, 1);
                break;
            case _length_: array[0] = stcTkn_pop(stack); 
                           result = length(symtable, array[0]);
                break;
            case _chr_   : array[0] = stcTkn_pop(stack);
                           result = chr(symtable, array[0]);
                           if(strcmp(result->name, "ERR_SEM") == 0)
                           {
                                destroyToken(result);
                                return NULL;
                           }
                break;
            case _ord_   : array[0] = stcTkn_pop(stack);
                           array[1] = stcTkn_pop(stack);
                           result = ord(symtable, array[0], array[1]);
                           if(strcmp(result->name, "ERR_SEM") == 0)
                           {
                                destroyToken(result);
                                return NULL;
                           }
                break;
            case _substr_: array[0] = stcTkn_pop(stack);
                           array[1] = stcTkn_pop(stack);
                           array[2] = stcTkn_pop(stack);
                           result = substr(symtable, array[0], array[1], array[2]);
                           if(strcmp(result->name, "ERR_SEM") == 0)
                           {
                                destroyToken(result);
                                return NULL;
                           }
                break;
            case _print_ : result = print(symtable, stack);
                           if(strcmp(result->name, "ERR_SEM") == 0)
                           {
                                destroyToken(result);
                                return NULL;
                           }
                break; 
        }

        return result;
    }

}

bool sa_isOperator(table_elem_t term)
{
    if(term == _plus_)
        return true;
    else if(term == _mins_)
        return true;
    else if(term == _mult_)
        return true;
    else if(term == _div_)
        return true;
    else if(term == _rel_)
        return true;

    return false;
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
int sa_prec(dynamicStr_t *sc_str, queue_t *que, symtable_t *loc_symtab, symtable_t *func_symtab, token_t **ret_token)
{
    stack_sa_t *stack = stc_init();
    token_info_t info;
    token_t *empty_token = createToken("$", info);
    stc_push(stack, _empt_, empty_token);

    token_t *token = scanner_get(sc_str, que);
    int token_term = 0;
    int stack_top_term = 0;
    char rule = 0;
    char detect_func = 0;
    int num_params = 0;
    elem_t *loc_elem = NULL;
    elem_t *func_elem = NULL;
    int count = 0;
    char func_read = 0;
    table_elem_t term;
    token_t *ptr_tok[3];
    stack_tkn_t *tok_stack = stcTkn_create();
    token_t *result = NULL;
    int err = 0;
    int builtin_func = 0;
    while(42)
    {
        stack_top_term = stc_topTerm(stack);
    
        token_term = sa_getTokenIndex(token);
        if(token_term == INVALID_TOKEN)
            goto fail_end;
        if(token_term == _func_)
        {
            detect_func = 1;
            builtin_func = sa_builtinType(token);
        }

        if(token_term == _id_ && (strcmp(token->name, "ID") == 0))
        {
            loc_elem = symtab_find(loc_symtab, sc_str->str);
            if(!detect_func)
            {
                func_elem = symtab_find(func_symtab, sc_str->str);
                token->info.ptr = func_elem;
            }

            if(loc_elem != NULL)
            {
                token_term = _id_;
                token->info.ptr = loc_elem;
            }
            else if(func_elem != NULL)
            {
                if(detect_func == 1 || count != 0)
                    goto sem_fail_defined;

                token->info.ptr = func_elem;
                token_term = _func_;
                detect_func = 1;
            }
            else
            {
                if(strcmp(loc_symtab->name, "$GT") == 0 && func_symtab->name == NULL)
                    goto sem_fail_defined;

                if(detect_func == 1 || count != 0)
                    goto fail_end;

                func_elem = symtab_elem_add(func_symtab, sc_str->str);
                func_elem->func.is_defined = false;
                token->info.ptr = func_elem;
                token_term = _func_;
                detect_func = 1;
            }

        }
        count++;

        rule = sa_prec_table[stack_top_term][token_term];

        if(rule == 'X')
        {
            goto fail_end;
        }
        else if(rule == '=')
        {
            if(func_read)
                goto fail_end;

            stc_push(stack, token_term, token);

        }
        else if(rule == '<')
        {
            if(func_read)
                goto fail_end;

            if(sa_isOperator(token_term) && detect_func)
                goto fail_end;

            stc_pushAfter(stack, stack_top_term, _sml_);
            stc_push(stack, token_term, token);
            stc_print(stack);
        }
        else if(rule == '>')
        {
            switch(stack_top_term)
            {
                /* E -> i */
                case _id_:
                    //term = stc_Top(stack);
                    //token_t *token = stc_tokPopTop(stack, &term);
                    ptr_tok[0] = stc_tokPopTop(stack, &term);
                    if(term != _id_)
                    {
                        destroyToken(ptr_tok[0]);
                        goto fail_end;
                    }
            
                    term = stc_popTop(stack);
                    if(term != _sml_)
                        goto fail_end;

                    stc_push(stack, _E_, ptr_tok[0]);
                    

                    if(detect_func)
                        num_params++;

                    break;

                /* E -> E + E */
                case _plus_:
                    ptr_tok[0] = stc_tokPopTop(stack, &term);
                    if((err = Check_err(ptr_tok[0], ptr_tok, 1, term, _E_)) != SUCCESS)
                    {
                        handleError(err); 
                    }
                    
                    ptr_tok[1] = stc_tokPopTop(stack, &term);
                    if((err = Check_err(ptr_tok[1], ptr_tok, 2, term, _plus_)) != SUCCESS)
                    {
                        handleError(err); 
                    }
                    
                    ptr_tok[2] = stc_tokPopTop(stack, &term);
                    if((err = Check_err(ptr_tok[2], ptr_tok, 3, term, _E_)) != SUCCESS)
                    {
                        handleError(err); 
                    }

                    term = stc_popTop(stack);
                    if(term != _sml_)
                        goto fail_end;

                    result = gen_expr(ptr_tok[1], ptr_tok[2], ptr_tok[0], loc_symtab);
                    if((err = Check_err(result, ptr_tok, 0, 0, 0)) != SUCCESS)
                    {
                        handleError(err);
                    }

                    stc_push(stack, _E_, result);  
                    break;

                /* E -> E * E */
                case _mult_:
                    ptr_tok[0] = stc_tokPopTop(stack, &term);
                    if((err = Check_err(ptr_tok[0], ptr_tok, 1, term, _E_)) != SUCCESS)
                    {
                        handleError(err); 
                    }

                    ptr_tok[1] = stc_tokPopTop(stack, &term);
                    if((err = Check_err(ptr_tok[1], ptr_tok, 2, term, _mult_)) != SUCCESS)
                    {
                        handleError(err); 
                    }

                    ptr_tok[2] = stc_tokPopTop(stack, &term);
                    if((err = Check_err(ptr_tok[2], ptr_tok, 3, term, _E_)) != SUCCESS)
                    {
                        handleError(err); 
                    }

                    term = stc_popTop(stack);
                    if(term != _sml_)
                        goto fail_end;

                    result = gen_expr(ptr_tok[1], ptr_tok[2], ptr_tok[0], loc_symtab);
                    if((err = Check_err(result, ptr_tok, 0, 0, 0)) != SUCCESS)
                    {
                        handleError(err);
                    }

                    stc_push(stack, _E_, result);    
                    break;

                /* E -> E - E */
                case _mins_:
                    ptr_tok[0] = stc_tokPopTop(stack, &term);
                    if((err = Check_err(ptr_tok[0], ptr_tok, 1, term, _E_)) != SUCCESS)
                    {
                        handleError(err); 
                    }

                    ptr_tok[1] = stc_tokPopTop(stack, &term);
                    if((err = Check_err(ptr_tok[1], ptr_tok, 2, term, _mins_)) != SUCCESS)
                    {
                        handleError(err); 
                    }

                    ptr_tok[2] = stc_tokPopTop(stack, &term);
                    if((err = Check_err(ptr_tok[2], ptr_tok, 3, term, _E_)) != SUCCESS)
                    {
                        handleError(err); 
                    }

                    term = stc_popTop(stack);
                    if(term != _sml_)
                        goto fail_end;

                    result = gen_expr(ptr_tok[1], ptr_tok[2], ptr_tok[0], loc_symtab);
                    if((err = Check_err(result, ptr_tok, 0, 0, 0)) != SUCCESS)
                    {
                        handleError(err); 
                    }

                    stc_push(stack, _E_, result);    
                    break;

                /* E -> E / E */
                case _div_:
                    ptr_tok[0] = stc_tokPopTop(stack, &term);
                    if((err = Check_err(ptr_tok[0], ptr_tok, 1, term, _E_)) != SUCCESS)
                    {
                        handleError(err); 
                    }

                    ptr_tok[1] = stc_tokPopTop(stack, &term);
                    if((err = Check_err(ptr_tok[1], ptr_tok, 2, term, _div_)) != SUCCESS)
                    {
                        handleError(err); 
                    }
                    
                    ptr_tok[2] = stc_tokPopTop(stack, &term);
                    if((err = Check_err(ptr_tok[2], ptr_tok, 3, term, _E_)) != SUCCESS)
                    {
                        handleError(err); 
                    }

                    term = stc_popTop(stack);
                    if(term != _sml_)
                        goto fail_end;

                    result = gen_expr(ptr_tok[1], ptr_tok[2], ptr_tok[0], loc_symtab);
                    if((err = Check_err(result, ptr_tok, 0, 0, 0)) != SUCCESS)
                    {
                        handleError(err); 
                    }

                    stc_push(stack, _E_, result);
                    break;

                /* L -> E rel E
                 * L -> S rel S 
                 */
                case _rel_:
                    ptr_tok[0] = stc_tokPopTop(stack, &term);
                    if((err = Check_err(ptr_tok[0], ptr_tok, 1, term, _E_)) != SUCCESS)
                    {
                        handleError(err); 
                    }

                    ptr_tok[1] = stc_tokPopTop(stack, &term);
                    if((err = Check_err(ptr_tok[1], ptr_tok, 2, term, _rel_)) != SUCCESS)
                    {
                        handleError(err); 
                    }

                    ptr_tok[2] = stc_tokPopTop(stack, &term);
                    if((err = Check_err(ptr_tok[2], ptr_tok, 3, term, _E_)) != SUCCESS)
                    {
                        handleError(err); 
                    }
                
                    term = stc_popTop(stack);  
                    if(term != _sml_)
                        goto fail_end;
                     
                    result = gen_expr(ptr_tok[1], ptr_tok[2], ptr_tok[0], loc_symtab);
                    if((err = Check_err(result, ptr_tok, 0, 0, 0)) != SUCCESS)
                    {
                        handleError(err); 
                    }

                    stc_push(stack, _L_, result);     // TODO
                    break;

                ///////////////////////////////////////////////////////////////////////////// 
                ////////                        FUNKCE                               //////// 
                ///////////////////////////////////////////////////////////////////////////// 
                
                /* F -> f
                 * F -> f E
                 */
                case _func_:
                    ptr_tok[0] = stc_tokPopTop(stack, &term);
                    if(term == _E_)
                    {
                        stcTkn_push(tok_stack, ptr_tok[0]);
                        ptr_tok[0] = stc_tokPopTop(stack, &term);
                        stcTkn_push(tok_stack, ptr_tok[0]);
                        if(term != _func_)
                            goto fail_end;
                    }
                    else if(term == _func_)
                    {  
                       stcTkn_push(tok_stack, ptr_tok[0]); 
                    }
                    else
                    {
                        goto fail_end;
                    }

                    term = stc_popTop(stack);
                    if(term != _sml_)
                        goto fail_end;

                    callFunc(builtin_func);       // Zmena tady
                    stc_push(stack, _F_, result); // Zmena tady

                    //callFunc(builtin_func);
                    break;

                /* F -> f E, ... , E */    
                case _coma_:
                    while(42)
                    {
                        //term = stc_popTop(stack);
                        ptr_tok[0] = stc_tokPopTop(stack, &term);
                        stcTkn_push(tok_stack, ptr_tok[0]);
                        if(term != _E_)
                            goto fail_end;

                        term = stc_popTop(stack);
                        if(term == _sml_)
                            break;
                        else if(term != _coma_)
                            goto fail_end;
                    }

                    //term = stc_popTop(stack);
                    ptr_tok[0] = stc_tokPopTop(stack, &term);
                    stcTkn_push(tok_stack, ptr_tok[0]);
                    if(term != _func_)
                        goto fail_end;

                    term = stc_popTop(stack);
                    if(term != _sml_)
                        goto fail_end;  

                    callFunc(builtin_func);           // Zmena tady
                    stc_push(stack, _F_, result);     // Zmena tady
                   // callFunc(builtin_func);
                    break;

                /* 
                 * E -> (E)
                 * F -> f()
                 * F -> f(E)
                 * F -> f(E, ... , E)
                 */
                case _rbrc_:
                    term = stc_popTop(stack);
                    //ptr_tok[0] = stc_tokPopTop(stack, &term);
                    //stcTkn_push(tok_stack, ptr_tok[0]);
                    if(term != _rbrc_)
                        goto fail_end;

                    token_t *expr_in_brc = ptr_tok[0] = stc_tokPopTop(stack, &term);
                    if(term == _lbrc_)
                    {
                        destroyToken(ptr_tok[0]);
                        //term = stc_popTop(stack);
                        ptr_tok[0] = stc_tokPopTop(stack, &term);
                        stcTkn_push(tok_stack, ptr_tok[0]);
                        if(term != _func_)
                            goto fail_end;

                        term = stc_popTop(stack);
                        if(term != _sml_)
                            goto fail_end;

                        callFunc(builtin_func);         // Zmena tady
                        stc_push(stack, _F_, result);   // Zmena tady
                        //callFunc(builtin_func);
                   
                        break;
                    }
                    else if(term == _E_)
                    {
                        //##stcTkn_push(tok_stack, ptr_tok[0]);
                        term = stc_popTop(stack);
                        if(term == _lbrc_)
                        {
                            //term = stc_popTop(stack);
                            ptr_tok[0] = stc_tokPopTop(stack, &term);
                            //###stcTkn_push(tok_stack, ptr_tok[0]);
                            if(term == _func_)
                            {
                                stcTkn_push(tok_stack, expr_in_brc);  // Odkomentuj ### a dva radky smaz
                                stcTkn_push(tok_stack, ptr_tok[0]);
                                stcTkn_print(tok_stack);
                                //stcTkn_push(tok_stack, ptr_tok[0]);

                                term = stc_popTop(stack);
                                if(term != _sml_)
                                    goto fail_end;

                                callFunc(builtin_func);         // Zmena tady
                                stc_push(stack, _F_, result);   // Zmena tady
                                //callFunc(builtin_func);
                             
                                break;
                            }
                            else if(term == _sml_)
                            {
                                destroyToken(ptr_tok[0]);
                                stc_push(stack, _E_, expr_in_brc);
                                break;
                            }
                        }
                        else if(term == _coma_)
                        {
                            stcTkn_push(tok_stack, expr_in_brc);  // Odkomentuj ### a dva radky smaz
                            //stcTkn_push(tok_stack, ptr_tok[0]);
                            while(42)
                            {
                                //term = stc_popTop(stack);
                                ptr_tok[0] = stc_tokPopTop(stack, &term);
                                stcTkn_push(tok_stack, ptr_tok[0]);
                                if(term != _E_)
                                    goto fail_end;

                                //ptr_tok[0] = stc_tokPopTop(stack, &term);
                                term = stc_popTop(stack);
                                if(term == _lbrc_)
                                    break;
                                else if(term != _coma_)
                                    goto fail_end;
                            }

                            //term = stc_popTop(stack);
                            ptr_tok[0] = stc_tokPopTop(stack, &term);
                            stcTkn_push(tok_stack, ptr_tok[0]);
                            if(term != _func_)
                                goto fail_end;

                            term = stc_popTop(stack);
                            if(term != _sml_)
                                goto fail_end;

                            callFunc(builtin_func);
                            stc_push(stack, _F_, result);
                            //callFunc(builtin_func);
                            break;
                        }

                    }
                  
                    break;
            }
            stc_print(stack);
            if(sa_detectSucEnd(stack, token_term))
            {
                if(detect_func)
                {
                    if(builtin_func)
                    {
                        if(result == NULL)
                            goto sem_gen;
                        /*
                        char *func_retval = malloc(strlen(result->info.ptr->var.key) * sizeof(char) + 1);
                        strcpy(func_retval, result->info.ptr->var.key);
                        *ret_code = func_retval;
                        */
                        
                        *ret_token = stc_tokPopTop(stack, &term); 
                    }
                    else
                    {
                        /*
                        char *func_retval = malloc(strlen("%retval") * sizeof(char) + 1);
                        strcpy(func_retval, "%retval");
                        *ret_code = func_retval;
                        */
                        token_info_t info;
                        *ret_token = createToken("%retval", info);
                        /*
                        *ret_code = malloc((strlen("%retval") + 1) * sizeof(char));
                        strcpy(*ret_code, "%retval");
                        DEBUG_PRINT("=> Expr: %s\n", *ret_code);
                        */
                    }
                }
                else if(result != NULL)
                {
                    token_t *tmp = stc_tokPopTop(stack, &term);
                    *ret_token = tmp;
                    stc_print(stack);
                    /*
                    *ret_code = malloc((strlen(result->info.ptr->var.key) + 1) * sizeof(char));
                    strcpy(*ret_code, result->info.ptr->var.key);
                    DEBUG_PRINT("=> Expr: %s\n", result->info.ptr->var.key);
                    */
                }
                else
                {
                    /*
                    token_t *ret_tok = stc_tokPopTop(stack, &term);
                    result = gen_expr(NULL, ret_tok, NULL, loc_symtab);
                    if((err = Check_err(result, ptr_tok, 0, 0, 0)) != SUCCESS)
                    {
                        handleError(err); 
                    } 

                    *ret_code = malloc((strlen(result->info.ptr->var.key) + 1) * sizeof(char));
                    strcpy(*ret_code, result->info.ptr->var.key);
                    DEBUG_PRINT("=> Expr: %s\n", result->info.ptr->var.key);
                    */
                    token_t *tmp = stc_tokPopTop(stack, &term);
                    *ret_token = tmp;
                }
                stc_print(stack);
                //stcTkn_print(tok_stack);
                if(!builtin_func)
                    stcTkn_destroy(tok_stack);
                stc_destroy(stack);
                scanner_unget(que, token, sc_str->str);
                return SUCCESS;
            }
            else
                continue;
        }
        stc_print(stack);

/* Tohle prijde pravdepodobne vymazat. */
#if 1
        if(sa_detectSucEnd(stack, token_term))
        {
            if(detect_func)
            {
              char *func_retval = malloc(strlen("%retval") * sizeof(char) + 1);
              strcpy(func_retval, "%retval");
              DEBUG_PRINT("=> Expr: %s\n", func_retval);
            }
            else if(result != NULL)
            {
                DEBUG_PRINT("=> Expr: %s\n", token->name);
            }
            else
            {
                token_t *ret_tok = stc_tokPopTop(stack, &term);
                result = gen_expr(NULL, ret_tok, NULL, loc_symtab);
                if((err = Check_err(result, ptr_tok, 0, 0, 0)) != SUCCESS)
                {
                        handleError(err); 
                } 
                DEBUG_PRINT("=> Expr: %s\n", ret_tok->info.string);
            }
            stcTkn_destroy(tok_stack);
            stc_destroy(stack);
            scanner_unget(que, token, sc_str->str);
            return SUCCESS;
        }
#endif

        token = scanner_get(sc_str, que);
    }

    stc_destroy(stack);
    return SUCCESS;

sem_gen:
    stcTkn_destroy(tok_stack);
    destroyToken(token);
    stc_destroy(stack);
    //destroyToken(token);
    return ERR_SEM_TYPE;

sem_internal:
    stcTkn_destroy(tok_stack);
    destroyToken(token);
    stc_destroy(stack);
    //destroyToken(token);
    return ERR_INTERNAL;

fail_end:
    stcTkn_destroy(tok_stack);
    destroyToken(token);
    stc_destroy(stack);
    //destroyToken(token);
    return ERR_SYN;

sem_fail:
    if(!builtin_func)
        stcTkn_destroy(tok_stack);
    destroyToken(token);
    stc_destroy(stack);
    //destroyToken(token);
    return ERR_SEM_FUNC;

sem_fail_defined:
    stcTkn_destroy(tok_stack);
    destroyToken(token);
    stc_destroy(stack);
    return ERR_SEM_UNDEF;

}

////////////////////////////////////////////////////////////////////////
