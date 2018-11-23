/**CFile****************************************************************

  FileName    [builtin_gen.c]

  SystemName  [IFJ - PROJECT]

  PackageName []

  Synopsis    []

  Author      [Jindrich Sestak, Lukas Valek]

  Affiliation []

  Date        [--/--/2018]

  Revision    []

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                          INCLUDES                                ///
////////////////////////////////////////////////////////////////////////

#include <stdbool.h>

#include "builtin_gen.h"

#include "symtable.h"
#include "dynamicArrParam.h"
#include "stackStr.h"
#include "token.h"
#include "stackTkn.h"

////////////////////////////////////////////////////////////////////////
///                       GLOBAL VARIABLES                           ///
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
////////////////////////////////////////////////////////////////////////

token_t *length(symtable_t *symtab, token_t *par)
{

    static unsigned long long label_n = 0;
    char name[20];
    token_info_t info;
    sprintf(name, "LEN%lluSTR", label_n);
    info.ptr = symtab_elem_add(symtab, name);
    token_t *des = createToken("INT_ID", info);

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
    token_t *des = createToken("STR_ID", info);

    char frame[3] = "LF";

    ///////TESTING in scanner main
    if (symtab == NULL)
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
    token_t *des = createToken("INT_ID", info);

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

    printf("STR2INT LF@%%retval %s@$ord$string%llu %s@$ord$position%llu\n",
           frame, label_n, frame, label_n );

    destroyToken(par1);
    destroyToken(par2);
    label_n++;
    return des;

}

token_t *input(symtable_t *symtab, int type)
{

    static unsigned long long label_n = 0;
    char name[20];
    token_info_t info;
    token_t *des;
    char er[10];
    char param[7];      //from which frame is variable ID
    char frame[3] = "LF";
    if (strcmp(symtab->name, "$GT" ) == 0)
        strcpy(frame, "GF");

    if (type == 0)
    {
        sprintf(name, "IN%lluINT", label_n);
        info.ptr = symtab_elem_add(symtab, name);
        des = createToken("INT_ID", info);
        strcpy(param, "int");
        strcpy(er, "int@0");

    }
    else if (type == 1)
    {
        sprintf(name, "IN%lluDBL", label_n);
        info.ptr = symtab_elem_add(symtab, name);
        des = createToken("DBL_ID", info);
        strcpy(param, "float");
        strcpy(er, "float@0.0");
    }
    else if (type == 2)
    {
        sprintf(name, "IN%lluSTR", label_n);
        info.ptr = symtab_elem_add(symtab, name);
        des = createToken("STR_ID", info);
        strcpy(param, "string");
        strcpy(er, "nil@nil");
    }
    else
    {
        return NULL;
    }



    printf("DEFVAR %s@%s\n"
           "READ %s@%s string@%s\n",
           frame, name, frame, name, param);

    printf("JUMPIFEQ $%s$%llu$%s %s@%s string@%s\n"
           "MOVE %s@%s %s\n"
           "LABEL $%s$%llu$%s\n",
           symtab->name, label_n, param, frame, name, param, frame, name, er,  symtab->name, label_n, param);

    label_n++;
    return des;
}

token_t *print(symtable_t *symtab, stack_tkn_t *stack)
{

    static unsigned long long label_n = 0;
    char name[20];
    token_info_t info;
    sprintf(name, "PRINT%llu", label_n);
    info.ptr = symtab_elem_add(symtab, name);
    token_t *des = createToken("STR_ID", info);
    char param[7];      //from which frame is variable ID
    char frame[3] = "LF";
    char *print;
    if (strcmp(symtab->name, "$GT" ) == 0)
        strcpy(frame, "GF");

    token_t *tmp;

    printf("DEFVAR %s@%s\n"
           "MOVE %s@%s nil@nil\n",
           frame, des->info.ptr->var.key, frame, des->info.ptr->var.key);

    for (int i = 0; i < stack->top + 1; ++i)
    {
        if (strcmp(tmp->name, "ID") == 0)
        {
            tmp = stcTkn_pop(stack);
            if (strcmp(symtab->name, "$GT" ) == 0)
            {
                strcpy(param, "GF");
            }
            else
            {
                strcpy(param, "LF");
            }

            print = malloc(sizeof(char) * (strlen(tmp->info.ptr->var.key) + 1));
            strcpy(print, tmp->info.ptr->var.key);
        }
        else
        {
            print = malloc(sizeof(char) * (strlen(tmp->info.string) + 1));
            strcpy(print, tmp->info.string);
            if (strcmp(tmp->name, "INT") == 0)
            {
                strcpy(param, "int");
            }
            else if (strcmp(tmp->name, "DBL") == 0)
            {
                strcpy(param, "float");
            }
            else if (strcmp(tmp->name, "STR") == 0)
            {
                strcpy(param, "string");
            }
        }

        printf("WRITE %s@%s\n", param, print);
    }

    label_n++;
    return des;
}

#if 0


int main()
{
    elem_t elem;
    elem.var.key = "jsemID1";
    token_info_t info;
    info.ptr = &elem;
    token_t *token = createToken("ID", info);

    elem_t elem5;
    elem5.var.key = "jsemID2";
    token_info_t info6;
    info6.ptr = &elem5;
    token_t *token5 = createToken("ID", info6);


    elem_t elem1;
    elem1.var.key = "jsem0INT_ID";
    token_info_t info2;
    info2.ptr = &elem1;
    token_t *token1 = createToken("INT_ID", info2);

    elem_t elem2;
    elem2.var.key = "jsem0STR_ID";
    token_info_t info5;
    info5.ptr = &elem2;
    token_t *token4 = createToken("STR_ID", info5);


    symtable_t *symtable = symtab_init("foo", VARIABLES);

    token_info_t tmp;
    tmp.ptr = NULL;


    token_t *op = createToken("+", tmp);
    token_info_t info3;
    info3.string = "ahoj";
    token_info_t info4;
    info4.string = "7.1";
    token_t *token2 = createToken("STR", info3);
    token_t *token3 = createToken("DBL", info4);

    token_t *tmp12 = input(symtable, 2);
    //token_t *tmp12 = gen_expr(NULL,  token3, NULL, symtable);
    /*printf("---%s---\n", tmp12->info.ptr->var.key);
    printf("---%s---\n", tmp12->name);*/
    //token_t *tmp13 = gen_expr(op, tmp12, token2, symtable);



    /*free(info);
    free(info6);
    free(tmp12);
    //free(token);
    free(token1);
    //free(op);
    free(token2);
    free(token3);
    free(token4);*/
    //free(token5);
    symtab_free(symtable);
    return 0;
}

#endif

////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////

