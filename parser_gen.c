/**CFile****************************************************************

  FileName    [if_while.c]

  SystemName  [IFJ - PROJECT]

  PackageName []

  Synopsis    []

  Author      [Jindrich Sestak]

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
#include "dynamicArrParam.h"
#include "stackStr.h"
#include "token.h"
#include "error.h"

////////////////////////////////////////////////////////////////////////
///                       GLOBAL VARIABLES                           ///
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
////////////////////////////////////////////////////////////////////////
//kdyby jsi mel if a v nem hned

int generate_if(symtable_t *var_tab, stack_str_t *stack, token_t *cond)
{
    if (strcmp(cond->name, "BOOL_ID") != 0)
        return ERR_SEM_OTHER;
    
    static unsigned long long label_n = 0;
    char frame[3] = "LF";
    if (strcmp(var_tab->name, "$GT") == 0)
        strcpy(frame, "GF");

    /*********Jump to ELSE if cond == false*********/
    printf("\nJUMPIFEQ $else$%llu %s@%s bool@false\n\n", label_n, frame, cond->info.ptr->var.key);
    /********* DOING IF statement *************/

    //Must go from bottom to top (LABEL endif -> JMP endif) viz. assembler
    //"LABEL $endif$%llu" has max length 34 bits
    char b[34];
    sprintf(b, "\nLABEL $endif$%llu\n\n", label_n);
    if (stcStr_push(stack, b) == false)
        return ERR_INTERNAL;

    /*********END IF, ELSE branch **************/
    //"LABEL $else$%llu\nJMP $endif$%llu" has max length 66 bits
    char c[66];
    sprintf(c,  "\n"
            "JUMP $endif$%llu\n"
            "LABEL $else$%llu\n\n",
            label_n, label_n);
    if (stcStr_push(stack, c) == false)
        return ERR_INTERNAL;

    label_n++;

    return SUCCESS;
}


bool generate_while_ending(stack_str_t *stack)
{
    static unsigned long long label_n = 0;
    /******** END of While***********/
    //"LABEL $end_while$%llu\nJUMP $while$%llu\n" has max length 72 bits
    char b[72];
    sprintf(b,  "\n"
            "JUMP $while$%llu\n"
            "LABEL $end_while$%llu\n\n",
            label_n, label_n);
    if (stcStr_push(stack, b) == false)
        return false;

    /*  //"JUMPIFEQ $end_while$%llu COND int@0\n" has max length 53 bits
      char *d =malloc(sizeof(char) * 53);
      sprintf(d, "JUMPIFEQ $end_while$%llu COND int@0\n", label_n);
      if (stcStr_push(stack, d) == false)
          return false;
    */
    label_n++;
    return true;
}


void generate_LABEL_while()
{
    static unsigned long long label_n = 0;
    /******* LABEL WHILE*************/
    printf("\nLABEL $while$%llu\n", label_n);
    label_n++;
}


int generate_while_false(symtable_t *var_tab, token_t *cond)
{
    if (strcmp(cond->name, "BOOL_ID") != 0)
        return ERR_SEM_OTHER;
    
    static unsigned long long label_n = 0;
    char frame[3] = "LF";
    if (strcmp(var_tab->name, "$GT") == 0)
        strcpy(frame, "GF");


    printf("JUMPIFEQ $end_while$%llu %s@%s bool@false\n\n", label_n, frame, cond->info.ptr->var.key);
    label_n++;

    return SUCCESS;
}


bool generate_function(stack_str_t *stack_str, elem_t *fun, dynamicArrParam_t *param_arr)
{
    printf( "\n"
            "LABEL %s\n"
            "PUSHFRAME\n"
            "DEFVAR LF@%%retval\n"
            "MOVE LF@%%retval nil@nil\n",
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

    if (! stcStr_push(stack_str, "\nPOPFRAME\nRETURN\n\n"))
        return false;

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
        strcpy(frame_or_type, frame);
        *var_or_const = malloc( (strlen(right_val->info.ptr->var.key) + 1) + sizeof(char) );
        if (*var_or_const == NULL)
            return ERR_INTERNAL;
        strcpy(*var_or_const, right_val->info.ptr->var.key);
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


int generate_var(symtable_t *var_tab, char *var_name, token_t *right_val)
{
    char frame[3];
    char frame_or_type[7];
    char *var_or_const;

    int ret_val = return_right_strings(var_tab, right_val, frame, frame_or_type, &var_or_const);
    if (ret_val != SUCCESS)
        return ret_val;


    if (symtab_find(var_tab, var_name) == NULL)
        printf("DEFVAR %s@%s\n", frame, var_name);

    printf("MOVE %s@%s %s@%s\n", frame, var_name, frame_or_type, var_or_const);
    
    free(var_or_const); // it is dynamically allocated
    return SUCCESS;
}


////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////
