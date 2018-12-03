/**CFile****************************************************************

  FileName    [parser_gen.c]

  SystemName  [IFJ - PROJECT]

  PackageName [Code generation]

  Synopsis    [Generate code for constructs in parser]

  Author      [Adam Pankuch, Jindrich Sestak]

  Login       [xpanku00]

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

static unsigned long long if_cnt = 0;       // counter for if labels
static unsigned long long endif_cnt = 0;        // counter for endif

static unsigned long long while_cnt = 0;    // counter for while lables
static unsigned long long func_cnt = 0;     // counter for function encapsulate labels

// unsigned long long takes up as string 20 bytes

////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
////////////////////////////////////////////////////////////////////////

void print_prolog()
{
    printf( ".IFJcode18\n"
            "DEFVAR GF@$des\n"
            "DEFVAR GF@$des1\n"
            "DEFVAR GF@$des2\n"
            "DEFVAR GF@$eq\n"
            "DEFVAR GF@$jump\n"
            "DEFVAR GF@$tmp\n"
            "DEFVAR GF@$type\n"
            "MOVE GF@$des nil@nil\n\n"
          );
}


void expected_LABEL_if(bool in_stat, char label_stat[])
{
    if (! in_stat)
        sprintf(label_stat, "LABEL $endif$%llu\n\n", endif_cnt);
}


bool generate_if_condition(list_t *code_buffer, bool in_stat, stack_str_t *stack, token_t *cond)
{
    ////////////////////////////////////////////////////////////////////////////////////////// TODO
    if (! condition_adjust(code_buffer, in_stat, cond))
        return false;

    // jump to ELSE if cond == false
    if (! print_or_append(code_buffer, in_stat, "PUSHS bool@false\nJUMPIFEQS $if$%llu\n\n", if_cnt))
        return false;


    char buffer[80];
    // endif has max length 37 bytes
    sprintf(buffer, "LABEL $endif$%llu\n\n", endif_cnt);
    if (! stcStr_push(stack, buffer))
        return false;

    sprintf(buffer, "LABEL $if$%llu\n", if_cnt);
    if (! stcStr_push(stack, buffer))
        return false;

    sprintf(buffer, "\nJUMP $endif$%llu\n", endif_cnt);
    if (! stcStr_push(stack, buffer))
        return false;


    endif_cnt++;
    if_cnt++;
    /*
        // else has max length 69 bytes
        sprintf(buffer, "\nJUMP $endif$%llu\n"
                        "LABEL $else$%llu\n\n",
                        if_cnt, if_cnt
        );
        if (! stcStr_push(stack, buffer))
            return false;
    */
    return true;
}


bool generate_LABEL_elsif(list_t *code_buffer, bool in_stat, stack_str_t *stack)
{
    char *buffer1 = stcStr_top(stack);
    if (! print_or_append(code_buffer, in_stat, "%s", buffer1))
        return false;

    char *buffer2 = malloc((strlen(buffer1) + 1) * sizeof(char));
    if (buffer2 == NULL)
        return false;
    strcpy(buffer2, buffer1);
    stcStr_pop(stack);

    if (! print_or_append(code_buffer, in_stat, "%s", stcStr_top(stack)))
        return false;
    stcStr_pop(stack);

    char buffer3[80];
    // endif has max length 37 bytes
    sprintf(buffer3, "LABEL $if$%llu\n", if_cnt);
    if (! stcStr_push(stack, buffer3))
        return false;

    if (! stcStr_push(stack, buffer2))
        return false;
    free(buffer2);

    if_cnt++;
    return true;
}


bool generate_elsif_condition(list_t *code_buffer, bool in_stat, token_t *cond)
{
    if (! condition_adjust(code_buffer, in_stat, cond))
        return false;

    if (! print_or_append(code_buffer, in_stat, "PUSHS bool@false\nJUMPIFEQS $if$%llu\n\n", if_cnt))
        return false;

    return true;
}


bool generate_else(list_t *code_buffer, bool in_stat, stack_str_t *stack)
{
    if (! print_or_append(code_buffer, in_stat, "%s", stcStr_top(stack)))
        return false;
    stcStr_pop(stack);

    if (! print_or_append(code_buffer, in_stat, "%s\n", stcStr_top(stack)))
        return false;
    stcStr_pop(stack);
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


bool generate_while_condition(list_t *code_buffer, bool in_stat, token_t *cond)
{
    ////////////////////////////////////////////////////////////////////////////////////////// TODO
    if (! condition_adjust(code_buffer, in_stat, cond))
        return false;

    if (! print_or_append(code_buffer, in_stat, "PUSHS bool@false\n JUMPIFEQS $end_while$%llu\n\n", while_cnt))
        return false;

    return true;
}


bool generate_while_ending(stack_str_t *stack)
{
    // end of while
    // endwhile has max length 74 bytes
    char buffer[80];
    sprintf(buffer, "\nJUMP $while$%llu\n"
            "LABEL $end_while$%llu\n\n"
            "PUSHS nil@nil\n",
            while_cnt, while_cnt
           );

    if (! stcStr_push(stack, buffer))
        return false;

    while_cnt++;
    return true;
}


bool condition_adjust(list_t *code_buffer, bool in_stat, token_t *cond)
{
    if (strcmp(cond->name, "nil") == 0)
    {
        if (! print_or_append(code_buffer, in_stat, "PUSHS bool@false\n"))
            return false;
    }
    else if (strcmp(cond->name, "BOOL_ID") != 0)
    {
        if (! print_or_append(code_buffer, in_stat, "PUSHS bool@true\n"))
            return false;
    }

    return true;
}


bool isFunctionEnd(char *generated_code)
{
    return (strncmp(generated_code, "\nPOPFRAME\nRETURN\n", strlen("\nPOPFRAME\nRETURN\n")) == 0);
}


bool generate_function(stack_str_t *stack_str, elem_t *fun, dynamicArrParam_t *param_arr)
{
    printf( "\n"
            "JUMP $end$function$%llu\n"
            "LABEL %s\n"
            "PUSHFRAME\n",
            func_cnt,
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

    char buffer[70];
    // end function has max length 60 bytes
    sprintf(buffer, "\nPOPFRAME\nRETURN\nLABEL $end$function$%llu\n\n", func_cnt);

    if (! stcStr_push(stack_str, buffer))
        return false;

    func_cnt++;
    return true;
}


int generate_var(list_t *code_buffer, list_t *defvar_buffer, bool in_stat, symtable_t *var_tab, char *var_name, token_t *right_val)
{
    char frame[3] = "LF";
    if (strcmp(var_tab->name, "$GT") == 0)
        strcpy(frame, "GF");



    if (symtab_find(var_tab, var_name) == NULL)
    {
        //printf("generate_var .. in_stat: %d\n", in_stat);
        if (! print_or_append(defvar_buffer, in_stat, "DEFVAR %s@%s\n", frame, var_name))
            return ERR_INTERNAL;
    }


    if (strcmp(right_val->name, "BOOL_ID") == 0)
        return ERR_SEM_TYPE;
    else if (strcmp(right_val->name, "INT") == 0)
    {
        if (! print_or_append(code_buffer, in_stat, "MOVE %s@%s int@%s\n", frame, var_name, right_val->info.string))
            return ERR_INTERNAL;
    }
    else if (strcmp(right_val->name, "STR") == 0)
    {
        if (! print_or_append(code_buffer, in_stat, "MOVE %s@%s string@%s\n", frame, var_name, right_val->info.string))
            return ERR_INTERNAL;
    }
    else if (strcmp(right_val->name, "DBL") == 0)
    {
        if (! print_or_append(code_buffer, in_stat, "MOVE %s@%s float@%s\n", frame, var_name, right_val->info.string))
            return ERR_INTERNAL;
    }
    else if (strcmp(right_val->name, "nil") == 0)
    {
        if (! print_or_append(code_buffer, in_stat, "MOVE %s@%s nil@nil\n", frame, var_name))
            return ERR_INTERNAL;
    }
    else
    {
        if (! print_or_append(code_buffer, in_stat, "POPS %s@%s\n", frame, var_name))
            return ERR_INTERNAL;
    }


    if (strcmp(frame, "LF") == 0)
    {
        if (! print_or_append(code_buffer, in_stat, "PUSHS %s@%s\n", frame, var_name))
            return ERR_INTERNAL;
    }

    return SUCCESS;
}



////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////
