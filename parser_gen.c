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

////////////////////////////////////////////////////////////////////////
///                       GLOBAL VARIABLES                           ///
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
////////////////////////////////////////////////////////////////////////
//kdyby jsi mel if a v nem hned

bool generate_if(symtable_t *var_tab, stack_str_t *stack, char *cond)
{
    static unsigned long long label_n = 0;
    char frame[3] = "LF";
    if (strcmp(var_tab->name, "$GT") == 0)
        strcpy(frame, "GF");

    /*********Jump to ELSE if cond == false*********/
    printf("\nJUMPIFEQ $else$%llu %s@%s int@0\n\n", label_n, frame, cond);
    /********* DOING IF statement *************/

    //Must go from bottom to top (LABEL endif -> JMP endif) viz. assembler
    //"LABEL $endif$%llu" has max length 34 bits
    char b[34];
    sprintf(b, "\nLABEL $endif$%llu\n\n", label_n);
    if (stcStr_push(stack, b) == false)
        return false;

    /*********END IF, ELSE branch **************/
    //"LABEL $else$%llu\nJMP $endif$%llu" has max length 66 bits
    char c[66];
    sprintf(c,  "\n"
            "JMP $endif$%llu\n"
            "LABEL $else$%llu\n\n",
            label_n, label_n);
    if (stcStr_push(stack, c) == false)
        return false;

    label_n++;
    return true;
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


void generate_while_false(symtable_t *var_tab, char *cond)
{
    static unsigned long long label_n = 0;
    char frame[3] = "LF";
    if (strcmp(var_tab->name, "$GT") == 0)
        strcpy(frame, "GF");

    printf("JUMPIFEQ $end_while$%llu %s@%s int@0\n\n", label_n, frame, cond);
    label_n++;
}


bool generate_function(stack_str_t *stack_str, elem_t *fun, dynamicArrParam_t *param_arr)
{
    static unsigned long long label_n = 0;
    printf( "\n"
            "LABEL %s$%llu\n"
            "PUSHFRAME\n"
            "DEFVAR LF@%%retval\n"
            "MOVE LF@%%retval nil@nil\n",
            fun->func.key,
            label_n
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
    label_n++;
    if (! stcStr_push(stack_str, "\nPOPFRAME\nRETURN\n\n"))
        return false;

    return true;
}



void generate_var(symtable_t *var_tab, char *var_name, char *right_val)
{
    static unsigned long long label_n = 0;

    char frame[3] = "LF";
    if (strcmp(var_tab->name, "$GT") == 0)
        strcpy(frame, "GF");

    char frame_right_val[3];
    if (strcmp(right_val, "%retval") == 0) // TODO
        strcpy(frame_right_val, "TF");
    else
        strcpy(frame_right_val, frame);

    if (symtab_find(var_tab, var_name) == NULL)
        printf("DEFVAR %s@%s$%llu\n", frame, var_name, label_n);

    printf("MOVE %s@%s$%llu %s@%s\n", frame, var_name, label_n, frame_right_val, right_val);
    label_n++;
}

token_t *length(symtable_t *symtab, token_t *par)
{

    static unsigned long long label_n = 0;
    char name[20];
    token_info_t info;
    sprintf(name, "LEN%lluSTR", label_n);
    info.ptr = symtab_elem_add(symtab, name);
    token_t *des = createToken("INT_ID", info);

    char frame_act [3] = "LF";
    char frame_var [7] = "LF";      //from which frame is variable ID
    char *print;
    if (strcmp(symtab->name, "$GT" ) == 0)
    {
        strcpy(frame_act, "GF");
    }
    else
    {
        strcpy(frame_act, "LF");
    }

    printf("DEFVAR %s@%s\n"
           "MOVE %s@%s nil@nil\n"
           "DEFVAR %s@$length$tmp%llu\n",
           frame_act, name, frame_act, name,
           frame_act, label_n);

    if (strcmp(par->name, "ID") == 0)
    {
        if (strcmp(symtab->name, "$GT" ) == 0)
        {
            strcpy(frame_var, "GF");
        }
        else
        {
            strcpy(frame_var, "LF");
        }

        print = malloc(sizeof(char) * (strlen(par->info.ptr->var.key) + 1));

        printf("MOVE %s@$length$tmp%llu %s@%s\n ",
               frame_act, label_n, frame_var, print);

        strcpy(print, par->info.ptr->var.key);
        printf("JUMPIFEQ $%s$%llu$string %s@$length$tmp%llu string@string\n"
               "EXIT int4\n"
               "LABEL $%s$%llu$string\n",
               frame_act, label_n, frame_act, label_n, frame_act, label_n);
    }
    else if (strcmp(par->name, "STR") == 0)
    {
        strcpy(frame_var, "string");

        print = malloc(sizeof(char) * (strlen(par->info.string) + 1));
        strcpy(print, par->info.string);

        printf("MOVE %s@$length$tmp%llu %s@%s\n ",
               frame_act, label_n, frame_var, print);
    }
    else {
        label_n++;
        destroyToken(par);
        free(des);
        token_info_t info1;
        token_t *error = createToken("ERR_SEM", info1);
        return error;
    }

    printf("STRLEN %s@%s %s@$length$tmp%llu\n",
           frame_act, name, frame_act, label_n);

    free(print);
    label_n++;
    return des;
}



////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////
