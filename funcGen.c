
#include <stdio.h>
#include <stdbool.h>

#include "symtable.h"
#include "dynamicArrParam.h"
#include "stackStr.h"
#include "token.h"


bool funcGen(stack_str_t *stack_str, elem_t *fun, dynamicArrParam_t *param_arr, int param_cnt)
{
    printf( "\n"
            "LABEL $%s\n" 
            "PUSHFRAME\n"
            "DEFVAR LF@%retval\n"
            "MOVE LF@%retval nil@nil\n", 
            fun->func.key
    );
    
    for (int i = 0; i < fun->func.n_params; i++)
        printf( "DEFVAR LF@%s\n"
                "MOVE LF@%s LF@%1\n", 
                param_arr->param_arr[i]->var.key,
                param_arr->param_arr[i]->var.key
        );

    printf("\n");

    if (! stcStr_push(stack_str, "POPFRAME\nRETURN\n\n"))
        return false;

    return true;
}