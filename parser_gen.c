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
    printf("\nJUMPIFEQ $else$%llu %s@%s bool@false\n\n", label_n, frame, cond);
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

    printf("JUMPIFEQ $end_while$%llu %s@%s bool@false\n\n", label_n, frame, cond);
    label_n++;
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



void generate_var(symtable_t *var_tab, char *var_name, char *right_val)
{
    char frame[3] = "LF";
    if (strcmp(var_tab->name, "$GT") == 0)
        strcpy(frame, "GF");

    char frame_right_val[3];
    if (strcmp(right_val, "%retval") == 0) // TODO
        strcpy(frame_right_val, "TF");
    else
        strcpy(frame_right_val, frame);

    if (symtab_find(var_tab, var_name) == NULL)
        printf("DEFVAR %s@%s\n", frame, var_name);

    printf("MOVE %s@%s %s@%s\n", frame, var_name, frame_right_val, right_val);
}

token_t *length(symtable_t *symtab, token_t *par)
{

    static unsigned long long label_n = 0;
    char name[20];
    token_info_t info;
    sprintf(name, "LEN%lluSTR", label_n);
    info.ptr = symtab_elem_add(symtab, name);
    token_t *des = createToken("LEN_STR_RET", info);

    char frame_act [3] = "LF";
    char frame_par [7] = "LF";      //from which frame is variable ID
    if (strcmp(symtab->name, "$GT" ) == 0)
        strcpy(frame_act, "GF");

    printf("DEFVAR %s@%s\n"
           "MOVE %s@%s nil@nil\n"
           "DEFVAR %s@$length$tmp%llu\n",
           frame_act, name, frame_act, name,
           frame_act, label_n);

    if (strcmp(par->name, "ID") == 0)
    {
        if (strcmp(symtab->name, "$GT" ) == 0)
        {
            strcpy(frame_par, "GF");
        }
        else
        {
            strcpy(frame_par, "LF");
        }

        printf("MOVE %s@$length$tmp%llu %s@%s\n "
               "JUMPIFEQ $%s$%llu$string %s@$length$tmp%llu string@string\n"
               "EXIT int@4\n"
               "LABEL $%s$%llu$string\n",
               frame_act, label_n, frame_par, par->info.ptr->var.key,
               frame_act, label_n, frame_act, label_n, frame_act, label_n);
    }
    else if (strcmp(par->name, "STR") == 0)
    {
        strcpy(frame_par, "string");

        printf("MOVE %s@$length$tmp%llu %s@%s\n ",
               frame_act, label_n, frame_par, par->info.string);
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

    label_n++;
    return des;
}

//RETURN ascii char of value par<0,255> "chr(42)"
token_t *chr(symtable_t *symtab, token_t *par)  
{
    static unsigned long long label_n = 0;
    char name[24];
    sprintf(name, "CHR%llu", label_n);
    token_info_t info;
    info.ptr = symtab_elem_add(symtab, name);
    token_t *des = createToken("CHR_RET", info);

    char frame[3] = "LF";

    ///////TESTING in scanner main 
    if(symtab == NULL)
    {
        printf("MOVE %s@$chr$tmp%llu int@%s\n",
                frame, label_n, par->info.string);
        printf("INT2CHAR LF@%%retval %s@$chr$tmp%llu\n",    //INT2CHAR takes value <0,255>
            frame, label_n);
    }
    /////////////////////////////////////////
    else
    {
        if (strcmp(symtab->name, "$GT" ) == 0)
            strcpy(frame, "GF");

        //"LABEL CHR\n"
        //"PUSHFRAME\n"
        //"DEFVAR LF@%%retval\n"
        printf("MOVE LF@%%retval nil@nil\n"
            "DEFVAR %s@$chr$tmp%llu\n",
            frame, label_n);

        if (strcmp(par->name, "ID") == 0)
        {
            if (symtab_find(symtab, par->info.ptr->var.key) == NULL)
            {
                //Variable doesn't exist
                printf("EXIT int@4\n");
                label_n++;
                destroyToken(par);
                free(des);
                token_info_t info1;
                token_t *error = createToken("ERR_SEM", info1);
                return error;
            }

            printf("MOVE %s@$chr$tmp%llu %s@%s\n"
                    "DEFVAR %s@$chr$tmp%llu$type\n"
                    "TYPE %s@$chr$tmp%llu$type %s@%s\n"
                    "JUMPIFEQ %s@$chr$tmp%llu$int$true %s@$chr$tmp%llu$type string@int\n"
                    "EXIT int@4\n"
                    "LABEL %s@$chr$tmp%llu$int$true\n", 
                    frame, label_n, frame, par->info.ptr->var.key,
                    frame, label_n,
                    frame, label_n, frame, par->info.ptr->var.key,
                    frame, label_n, frame, label_n,
                    frame, label_n);
        }
        else if (strcmp(par->name, "INT") == 0)
        {
            printf("MOVE %s@$chr$tmp%llu int@%s\n",
                    frame, label_n, par->info.string);
        }
        else
        {
            printf("EXIT int@4\n");
            label_n++;
            destroyToken(par);
            free(des);
            token_info_t info1;
            token_t *error = createToken("ERR_SEM", info1);
            return error;
        }
        printf("INT2CHAR LF@%%retval %s@$chr$tmp%llu\n",        //INT2CHAR takes value <0,255>
                frame, label_n);
        //"POPFRAME\n"
        //"RETURN\n"
    }
    
    destroyToken(par);
    label_n++;
    return des;
}

token_t *ord(symtable_t *symtab, token_t *par1, token_t *par2)      //par1 == string, par2 == position(INT)
{
    static unsigned long long label_n = 0;
    char name[24];
    sprintf(name, "ORD%llu", label_n);
    token_info_t info;
    info.ptr = symtab_elem_add(symtab, name);
    token_t *des = createToken("ORD_RET", info);

    char frame[3] = "LF";

     if (strcmp(symtab->name, "$GT" ) == 0)
            strcpy(frame, "GF");

    printf("MOVE LF@%%retval nil@nil\n"
            "DEFVAR %s@$ord$string%llu\n"
            "DEFVAR %s@$ord$position%llu\n",
            frame, label_n,
            frame, label_n);

    if (strcmp(par1->name, "ID") == 0)
    {
        if (symtab_find(symtab, par1->info.ptr->var.key) == NULL)
        {
            //Variable doesn't exist
            printf("EXIT int@4\n");
            label_n++;
            destroyToken(par1);
            destroyToken(par2);
            free(des);
            token_info_t info1;
            token_t *error = createToken("ERR_SEM", info1);
            return error;
        }

        printf("MOVE %s@$ord$string%llu %s@%s\n"
                "DEFVAR %s@$ord$string%llu$type\n"
                "TYPE %s@$ord$string%llu$type %s@%s\n"
                "JUMPIFEQ %s@$ord$string%llu$string$true %s@$ord$string%llu$type string@string\n"
                "EXIT int@4\n"
                "LABEL %s@$ord$string%llu$string$true\n", 
                frame, label_n, frame, par1->info.ptr->var.key,
                frame, label_n,
                frame, label_n, frame, par1->info.ptr->var.key,
                frame, label_n, frame, label_n,
                frame, label_n);
    }
    else if (strcmp(par1->name, "STR") == 0)
    {
        printf("MOVE %s@$ord$string%llu string@%s\n",
                frame, label_n, par1->info.string);
    }
    else
    {
        printf("EXIT int@4\n");
        label_n++;
        destroyToken(par1);
        destroyToken(par2);
        free(des);
        token_info_t info1;
        token_t *error = createToken("ERR_SEM", info1);
        return error;
    }

    if (strcmp(par2->name, "ID") == 0)
    {
        if (symtab_find(symtab, par2->info.ptr->var.key) == NULL)
        {
            //Variable doesn't exist
            printf("EXIT int@4\n");
            label_n++;
            destroyToken(par1);
            destroyToken(par2);
            free(des);
            token_info_t info1;
            token_t *error = createToken("ERR_SEM", info1);
            return error;
        }

        printf("MOVE %s@$ord$position%llu %s@%s\n"
                "DEFVAR %s@$ord$positiong%llu$type\n"
                "TYPE %s@$ord$position%llu$type %s@%s\n"
                "JUMPIFEQ %s@$ord$position%llu$int$true %s@$ord$position%llu$type string@int\n"
                "EXIT int@4\n"
                "LABEL %s@$ord$string%llu$string$true\n", 
                frame, label_n, frame, par2->info.ptr->var.key,
                frame, label_n,
                frame, label_n, frame, par2->info.ptr->var.key,
                frame, label_n, frame, label_n,
                frame, label_n);
    }
    else if (strcmp(par2->name, "INT") == 0)
    {
        printf("MOVE %s@$ord$position%llu int@%s\n",
                frame, label_n, par2->info.string);
    }
    else
    {
        printf("EXIT int@4\n");
        label_n++;
        destroyToken(par1);
        destroyToken(par2);
        free(des);
        token_info_t info1;
        token_t *error = createToken("ERR_SEM", info1);
        return error;
    }

    printf("STR2INT LF@%%retval %s@$ord$string%llu %s@$ord$position%llu",
            frame, label_n, frame, label_n );

    destroyToken(par1);
    destroyToken(par2);
    label_n++;
    return des;

}


////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////
