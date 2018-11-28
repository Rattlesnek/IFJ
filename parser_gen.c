/**CFile****************************************************************

  FileName    [if_while.c]

  SystemName  [IFJ - PROJECT]

  PackageName []

  Synopsis    []

  Author      [Adam Pankuch, Jindrich Sestak]

  Affiliation []

  Date        [20/11/2018]

  Revision    []

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                          INCLUDES                                ///
////////////////////////////////////////////////////////////////////////

#include <stdbool.h>

#include "parser_gen.h"

#include "symtable.h"
#include "dynamic_arr_param.h"
#include "stack_str.h"
#include "list.h"
#include "token.h"
#include "error.h"

////////////////////////////////////////////////////////////////////////
///                       GLOBAL VARIABLES                           ///
////////////////////////////////////////////////////////////////////////

static unsigned long long if_cnt = 0;
static unsigned long long while_cnt = 0;
static unsigned long long func_cnt = 0;

////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
////////////////////////////////////////////////////////////////////////

// unsigned long long takes as string 20 bytes

void print_prolog()
{
    printf( ".IFJcode18\n" 
            "DEFVAR GF@$des\n"
            "DEFVAR GF@$eq\n"
            "DEFVAR GF@$jump\n"
            "DEFVAR GF@$tmp\n"
            "DEFVAR GF@$type\n\n"
            );
}

void expected_LABEL_if(bool in_stat, char label_stat[])
{
    if (! in_stat)  
        sprintf(label_stat, "\nLABEL $endif$%llu\n\n", if_cnt);
}


bool generate_if(list_t *code_buffer, bool in_stat,  stack_str_t *stack, token_t *cond)
{
    ////////////////////////////////////////////////////////////////////////////////////////// TODO

    /*********Jump to ELSE if cond == false*********/
    if (! print_or_append(code_buffer, in_stat, "\nJUMPIFEQ $else$%llu GF@$des bool@false\n\n", if_cnt))
        return false;
    /********* DOING IF statement *************/

    //Must go from bottom to top (LABEL endif -> JMP endif) viz. assembler
    //"LABEL $endif$%llu" has max length 34 bits
    char b[40];
    sprintf(b, "\nLABEL $endif$%llu\n\n", if_cnt);
    if (! stcStr_push(stack, b))
        return false;

    /*********END IF, ELSE branch **************/
    //"LABEL $else$%llu\nJMP $endif$%llu" has max length 66 bits
    char c[70];
    sprintf(c,  "\nJUMP $endif$%llu\n"
                "LABEL $else$%llu\n\n",
                if_cnt, if_cnt
    );
    if (! stcStr_push(stack, c))
        return false;

    if_cnt++;
    return true;
}



void expected_LABEL_while(bool in_stat, char label_stat[])
{
    if (! in_stat)  
        sprintf(label_stat, "\nJUMP $while$%llu\n", while_cnt);
}


bool generate_LABEL_while(list_t *code_buffer, bool in_stat)
{
    if (! print_or_append(code_buffer, in_stat, "\nLABEL $while$%llu\n", while_cnt))
        return false;
    
    return true;
}


bool generate_while_false(list_t *code_buffer, bool in_stat, token_t *cond)
{
    ////////////////////////////////////////////////////////////////////////////////////////// TODO

    if (! print_or_append(code_buffer, in_stat, "JUMPIFEQ $end_while$%llu GF@$des bool@false\n\n", while_cnt))
        return false;
    
    return true;
}


bool generate_while_ending(stack_str_t *stack)
{
    /******** END of While***********/
    //"LABEL $end_while$%llu\nJUMP $while$%llu\n" has max length 72 bytes
    char b[75];
    sprintf(b,  "\nJUMP $while$%llu\n"
                "LABEL $end_while$%llu\n\n",
                while_cnt, while_cnt
    );

    if (! stcStr_push(stack, b))
        return false;

    while_cnt++;
    return true;
}


bool generate_function(stack_str_t *stack_str, elem_t *fun, dynamicArrParam_t *param_arr)
{
    printf( "\n"
            "JUMP $end$function$%s\n"
            "LABEL %s\n"
            "PUSHFRAME\n"
            "DEFVAR LF@%%retval\n"
            "MOVE LF@%%retval nil@nil\n",
            fun->func.key,
            fun->func.key
          );

    for (int i = 0; i < fun->func.n_params; i++)
    {
        printf( "DEFVAR LF@%s\n"
                "MOVE LF@%s LF@%%%d\n",
                param_arr->param_arr[i]->var.key,
                param_arr->param_arr[i]->var.key,
                i + 1
              );
    }
    printf("\n");

    char str[100]; // TODO
    sprintf(str, "\nPOPFRAME\nRETURN\nLABEL $end$function$%llu\n\n", func_cnt);

    if (! stcStr_push(stack_str, str))
        return false;

    func_cnt++;
    return true;
}


int return_right_strings(symtable_t *var_tab, token_t *right_val, char frame[3], char frame_or_type[7], char **var_or_const)
{
    if (strcmp(var_tab->name, "$GT") == 0)
        strcpy(frame, "GF");
    else
        strcpy(frame, "LF");

    if (strcmp(right_val->name, "ID") == 0 || strcmp(right_val->name, "INT_ID") == 0 || strcmp(right_val->name, "DBL_ID") == 0 || strcmp(right_val->name, "STR_ID") == 0)
    {
        strcpy(frame_or_type, "GF");
        *var_or_const = malloc( (strlen("$des") + 1) + sizeof(char) );
        if (*var_or_const == NULL)
            return ERR_INTERNAL;
        strcpy(*var_or_const, "$des");
    }
    else if (strcmp(right_val->name, "%retval") == 0)
    {
        strcpy(frame_or_type, "TF");
        *var_or_const = malloc( (strlen("%retval") + 1) + sizeof(char) );
        if (*var_or_const == NULL)
            return ERR_INTERNAL;
        strcpy(*var_or_const, "%retval");
    }
    else if (strcmp(right_val->name, "INT") == 0)
    {
        strcpy(frame_or_type, "int");
        *var_or_const = malloc( (strlen(right_val->info.string) + 1) + sizeof(char) );
        if (*var_or_const == NULL)
            return ERR_INTERNAL;
        strcpy(*var_or_const, right_val->info.string);
    }
    else if (strcmp(right_val->name, "DBL") == 0)
    {
        strcpy(frame_or_type, "float");
        *var_or_const = malloc( (strlen(right_val->info.string) + 1) + sizeof(char) );
        if (*var_or_const == NULL)
            return ERR_INTERNAL;
        strcpy(*var_or_const, right_val->info.string);
    }
    else if (strcmp(right_val->name, "STR") == 0)
    {
        strcpy(frame_or_type, "string");
        *var_or_const = malloc( (strlen(right_val->info.string) + 1) + sizeof(char) );
        if (*var_or_const == NULL)
            return ERR_INTERNAL;
        strcpy(*var_or_const, right_val->info.string);
    }
    else if (strcmp(right_val->name, "nil") == 0)
    {
        strcpy(frame_or_type, "nil");
        *var_or_const = malloc( (strlen("nil") + 1) + sizeof(char) );
        if (*var_or_const == NULL)
            return ERR_INTERNAL;
        strcpy(*var_or_const, "nil");
    }
    else
        return ERR_SEM_OTHER;

    return SUCCESS;
}


int generate_var(list_t *code_buffer, list_t *defvar_buffer, bool in_stat, symtable_t *var_tab, char *var_name, token_t *right_val)
{
    char frame[3];
    char frame_or_type[7];
    char *var_or_const;

    int ret_val = return_right_strings(var_tab, right_val, frame, frame_or_type, &var_or_const);
    if (ret_val != SUCCESS)
        return ret_val;


    if (symtab_find(var_tab, var_name) == NULL)
    {
        //printf("generate_var .. in_stat: %d\n", in_stat);
        if (! print_or_append(defvar_buffer, in_stat, "DEFVAR %s@%s\n", frame, var_name))
            return ERR_INTERNAL;
    }

    if (! print_or_append(code_buffer, in_stat, "MOVE %s@%s %s@%s\n", frame, var_name, frame_or_type, var_or_const))
        return ERR_INTERNAL;

    free(var_or_const); // it is dynamically allocated
    return SUCCESS;
}


////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////
