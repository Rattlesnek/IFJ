/**CFile****************************************************************

  FileName    []

  SystemName  [IFJ - PROJECT]

  PackageName [Code generation implementation]

  Synopsis    []

  Author      [Lukas Vlake]

  Affiliation []

  Date        [17/11/2018]

  Revision    []

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                          INCLUDES                                ///
////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include "token.h"
#include "symtable.h"
#include "codeGen.h"




////////////////////////////////////////////////////////////////////////
///                       GLOBAL VARIABLES                           ///
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
////////////////////////////////////////////////////////////////////////



bool isINT(token_t *token)
{
    if (strcmp(token->name, "INT") == 0)
    {
        return true;
    }
    return false;
}

bool isDBL(token_t *token)
{
    if (strcmp(token->name, "DBL") == 0)
    {
        return true;
    }
    return false;
}

bool isSTR(token_t *token)
{
    if (strcmp(token->name, "STR") == 0)
    {
        return true;
    }
    return false;
}

bool isID(token_t *token)
{
    if (strcmp(token->name, "ID") == 0)
    {
        return true;
    }
    return false;
}

bool isINT_ID(token_t *token)
{
    if (strcmp(token->name, "INT_ID") == 0)
    {
        return true;
    }
    return false;
}

bool isDBL_ID(token_t *token)
{
    if (strcmp(token->name, "INT_DBL") == 0)
    {
        return true;
    }
    return false;
}

bool isSTR_ID(token_t *token)
{
    if (strcmp(token->name, "STR_ID") == 0)
    {
        return true;
    }
    return false;
}



int matrix[4][4] = {
//             INT_index       DBL_index    STR_index   ID_index
    /*INT_index */   { INT_INT, INT_DBL, INT_STR, INT_ID },
    /*DBL_index */   { DBL_INT, DBL_DBL, DBL_STR, DBL_ID },
    /*STR_index */   { STR_INT, STR_DBL, STR_STR, STR_ID },
    /*ID_index */    { ID_INT , ID_DBL , ID_STR , ID_ID  }
};

char *operator(char *op)
{

    if (strcmp(op, "+") == 0)
    {
        return "ADD";
    }
    else if (strcmp(op, "-") == 0)
    {
        return "SUB";
    }
    else if (strcmp(op, "*") == 0)
    {
        return "MUL";
    }
    else if (strcmp(op, "/") == 0)
    {
        return "DIV";
    }
    else if (strcmp(op, "==") == 0)
    {
        return "EQ";
    }
    else if (strcmp(op, "<=") == 0)
    {
        return "LT";
    }
    else if (strcmp(op, ">=") == 0)
    {
        return "GT";
    }
    else if (strcmp(op, "!=") == 0)
    {
        return "EQ";
    }
    else if (strcmp(op, "<") == 0)
    {
        return "LT";
    }
    else if (strcmp(op, ">") == 0)
    {
        return "GT";
    }
    else
    {
        fprintf(stderr, "Wrong operand\n");
        return "-1";
    }
}


int type(char *param1, char *param2)
{
    int index_param1, index_param2;
    if (strncmp(param1, "INT", 3) == 0)
    {
        index_param1 = INT_index;
    }
    else if (strncmp(param1, "DBL", 3) == 0)
    {
        index_param1 = DBL_index;
    }
    else if (strncmp(param1, "STR", 3) == 0)
    {
        index_param1 = STR_index;
    }
    else if (strcmp(param1, "ID") == 0)
    {
        index_param1 = ID_index;
    }

    if (strncmp(param2, "INT", 3) == 0)
    {
        index_param2 = INT_index;
    }
    else if (strncmp(param2, "DBL", 3) == 0)
    {
        index_param2 = DBL_index;
    }
    else if (strncmp(param2, "STR", 3) == 0)
    {
        index_param2 = STR_index;
    }
    else if (strcmp(param2, "ID") == 0)
    {
        index_param2 = ID_index;
    }

    return matrix[index_param1][index_param2];
}

token_t *int_int(token_t *op, token_t *par1, token_t *par2, symtable_t *symtab)
{
    static unsigned long long label_n = 0;

    char param1[10];
    char param2[10];
    char frame[3];
    char name[20];
    char div[2] = "";
    token_info_t *info = malloc(sizeof(token_info_t));
    sprintf(name, "INT%lluINT", label_n);
    info->ptr = symtab_elem_add(symtab, name);
    token_t *des = createToken("INT_ID", *info);

    char *print1;
    char *print2;

    if (strcmp(symtab->name, "GT" ) == 0)
    {
        strcpy(frame, "GF");
    }
    else
    {
        strcpy(frame, "LF");
    }

    if (strcmp(par1->name, "INT_ID") == 0)
    {
        if (strcmp(symtab->name, "GT" ) == 0)
        {
            strcpy(param1, "GF");
        }
        else
        {
            strcpy(param1, "LF");
        }

        print1 = malloc(sizeof(char) * (strlen(par1->info.ptr->var.key) + 1));
        strcpy(print1, par1->info.ptr->var.key);
    }
    else if (strcmp(par1->name, "INT") == 0)
    {
        strcpy(param1, "int");

        print1 = malloc(sizeof(char) * (strlen(par1->info.string) + 1));
        strcpy(print1, par1->info.string);
    }
    if (strcmp(par2->name, "INT_ID") == 0)
    {
        if (strcmp(symtab->name, "GT" ) == 0)
        {
            strcpy(param2, "GF");
        }
        else
        {
            strcpy(param2, "LF");
        }

        print2 = malloc(sizeof(char) * (strlen(par2->info.ptr->var.key) + 1));
        strcpy(print2, par2->info.ptr->var.key);
    }
    else if (strcmp(par2->name, "INT") == 0)
    {
        strcpy(param2, "int");

        print2 = malloc(sizeof(char) * (strlen(par1->info.string) + 1));
        strcpy(print2, par2->info.string);
    }

    if (strcmp(op->name, "/") == 0)
    {
        strcpy(div, "I");
    }
    printf("%s%s %s@%s %s@%s %s@%s\n", div, operator(op->name), frame, des->info.ptr->var.key,
           param1, print1, param2, print2);

    if (strcmp(op->name, "<=") == 0 || strcmp(op->name, ">=") == 0)
    {
        printf("DEFVAR %s@%s$eq\n"
               "DEFVAR %s@%s$or\n"
               "MOVE %s@%s$or %s@%s\n"
               "EQ %s@%s$eq %s@%s %s@%s\n"
               "OR %s@%s %s@%s$eq %s@%s$or\n",
               frame,
               des->info.ptr->var.key,//
               frame,
               des->info.ptr->var.key,//
               frame,
               des->info.ptr->var.key,
               frame,
               des->info.ptr->var.key,//
               frame,
               des->info.ptr->var.key,
               param2,
               print2,
               param1,
               print1,//
               frame,
               des->info.ptr->var.key,
               frame,
               des->info.ptr->var.key,
               frame,
               des->info.ptr->var.key
              );
    }
    label_n++;
    /*destroyToken(par1);
    destroyToken(par2);*/
    destroyToken(op);
    free(print1);
    free(print2);
    return des;
}

token_t *dbl_dbl(token_t *op, token_t *par1, token_t *par2, symtable_t *symtab)
{
    static unsigned long long label_n = 0;

    char param1[10];
    char param2[10];
    char frame[3];
    char name[20];
    char div[2] = "";
    token_info_t *info = malloc(sizeof(token_info_t));
    sprintf(name, "DBL%lluDBL", label_n);
    info->ptr = symtab_elem_add(symtab, name);
    token_t *des = createToken("DBL_ID", *info);

    char *print1;
    char *print2;

    if (strcmp(symtab->name, "GT" ) == 0)
    {
        strcpy(frame, "GF");
    }
    else
    {
        strcpy(frame, "LF");
    }

    if (strcmp(par1->name, "DBL_ID") == 0)
    {
        if (strcmp(symtab->name, "GT" ) == 0)
        {
            strcpy(param1, "GF");
        }
        else
        {
            strcpy(param1, "LF");
        }

        print1 = malloc(sizeof(char) * (strlen(par1->info.ptr->var.key) + 1));
        strcpy(print1, par1->info.ptr->var.key);
    }
    else if (strcmp(par1->name, "DBL") == 0)
    {
        strcpy(param1, "float");

        print1 = malloc(sizeof(char) * (strlen(par1->info.string) + 1));
        strcpy(print1, par1->info.string);
    }
    if (strcmp(par2->name, "DBL_ID") == 0)
    {
        if (strcmp(symtab->name, "GT" ) == 0)
        {
            strcpy(param2, "GF");
        }
        else
        {
            strcpy(param2, "LF");
        }

        print2 = malloc(sizeof(char) * (strlen(par2->info.ptr->var.key) + 1));
        strcpy(print2, par2->info.ptr->var.key);
    }
    else if (strcmp(par2->name, "DBL") == 0)
    {
        strcpy(param2, "float");

        print2 = malloc(sizeof(char) * (strlen(par1->info.string) + 1));
        strcpy(print2, par2->info.string);
    }

    printf("%s%s %s@%s %s@%s %s@%s\n", div, operator(op->name), frame, des->info.ptr->var.key,
           param1, print1, param2, print2);

    if (strcmp(op->name, "<=") == 0 || strcmp(op->name, ">=") == 0)
    {
        printf("DEFVAR %s@%s$eq\n"
               "DEFVAR %s@%s$or\n"
               "MOVE %s@%s$or %s@%s\n"
               "EQ %s@%s$eq %s@%s %s@%s\n"
               "OR %s@%s %s@%s$eq %s@%s$or\n",
               frame,
               des->info.ptr->var.key,//
               frame,
               des->info.ptr->var.key,//
               frame,
               des->info.ptr->var.key,
               frame,
               des->info.ptr->var.key,//
               frame,
               des->info.ptr->var.key,
               param2,
               print2,
               param1,
               print1,//
               frame,
               des->info.ptr->var.key,
               frame,
               des->info.ptr->var.key,
               frame,
               des->info.ptr->var.key
              );
    }
    label_n++;
    /*destroyToken(par1);
    destroyToken(par2);*/
    destroyToken(op);
    free(print1);
    free(print2);
    return des;
}

token_t *int_dbl(token_t *op, token_t *par1, token_t *par2, symtable_t *symtab)
{

    static unsigned long long label_n = 0;
    char param1[10];
    char param2[10];
    char frame[3];
    char name[20];
    token_info_t *info = malloc(sizeof(token_info_t));
    sprintf(name, "INT%lluDBL", label_n);
    info->ptr = symtab_elem_add(symtab, name);
    token_t *des = createToken("DBL_ID", *info);

    char *print1;
    char *print2;

    if (strcmp(symtab->name, "GT" ) == 0)
    {
        strcpy(frame, "GF");
    }
    else
    {
        strcpy(frame, "LF");
    }

    if (strcmp(par1->name, "INT_ID") == 0 || strcmp(par1->name, "DBL_ID") == 0)
    {
        if (strcmp(symtab->name, "GT" ) == 0)
        {
            strcpy(param1, "GF");
        }
        else
        {
            strcpy(param1, "LF");
        }

        print1 = malloc(sizeof(char) * (strlen(par1->info.ptr->var.key) + 1));
        strcpy(print1, par1->info.ptr->var.key);
    }
    else if (strcmp(par1->name, "INT" ) == 0 || strcmp(par1->name, "DBL") == 0)
    {
        if (strcmp(par1->name, "INT") == 0)
        {
            strcpy(param1, "int");
        }
        else if (strcmp(par1->name, "DBL") == 0)
        {
            strcpy(param1, "float");
        }

        print1 = malloc(sizeof(char) * (strlen(par1->info.string) + 1));
        strcpy(print1, par1->info.string);
    }
    if (strcmp(par2->name, "INT_ID") == 0 || strcmp(par2->name, "DBL_ID") == 0)
    {
        if (strcmp(symtab->name, "GT" ) == 0)
        {
            strcpy(param2, "GF");
        }
        else
        {
            strcpy(param2, "LF");
        }

        print2 = malloc(sizeof(char) * (strlen(par2->info.ptr->var.key) + 1));
        strcpy(print2, par2->info.ptr->var.key);

    }
    else if (strcmp(par2->name, "INT") == 0 || strcmp(par2->name, "DBL") == 0)
    {
        if (strcmp(par2->name, "INT") == 0)
        {
            strcpy(param2, "int");
        }
        else if (strcmp(par2->name, "DBL") == 0)
        {
            strcpy(param2, "float");
        }

        print2 = malloc(sizeof(char) * (strlen(par2->info.string) + 1));
        strcpy(print2, par2->info.string);
    }


    printf("DEFVAR %s@%llu$tmp$1\n"
           "INT2FLOAT %s@%llu$tmp$1 %s@%s\n"
           "%s %s@%s %s@%s %s@%llu$tmp$1\n",

           frame,
           label_n,//
           frame,
           label_n,
           param1,
           print1,//
           operator(op->name),
           frame,
           des->info.ptr->var.key,
           param2,
           print2,
           frame,
           label_n//
          );
    if (strcmp(op->name, "<=") == 0 || strcmp(op->name, ">=") == 0)
    {
        //udelat == do temp promenne, OR temp promenne a vysledku operace predtim

        printf("DEFVAR %s@%s$eq\n"
               "DEFVAR %s@%s$or\n"
               "MOVE %s@%s$or %s@%s\n"
               "EQ %s@%s$eq %s@%s %s@%llu$tmp$1\n"
               "OR %s@%s %s@%s$eq %s@%s$or\n",
               frame,
               des->info.ptr->var.key,//
               frame,
               des->info.ptr->var.key,//
               frame,
               des->info.ptr->var.key,//
               frame,
               des->info.ptr->var.key,
               frame,
               des->info.ptr->var.key,
               param2,
               print2,
               frame,
               label_n,//
               frame,
               des->info.ptr->var.key,
               frame,
               des->info.ptr->var.key,
               frame,
               des->info.ptr->var.key
              );
    }

    label_n++;
    /*destroyToken(par1);
    destroyToken(par2);*/
    destroyToken(op);
    free(print1);
    free(print2);
    return des;

}

token_t *int_str(token_t *op, token_t *par1, token_t *par2, symtable_t *symtab)
{
    static unsigned long long label_n = 0;

    char param1[10];
    char param2[10];
    char frame[3];
    char name[20];
    token_info_t *info = malloc(sizeof(token_info_t));
    sprintf(name, "INT%lluSTR", label_n);
    info->ptr = symtab_elem_add(symtab, name);
    token_t *des = createToken("BOOL_ID", *info);

    char *print1;
    char *print2;

    if (strcmp(symtab->name, "GT" ) == 0)
    {
        strcpy(frame, "GF");
    }
    else
    {
        strcpy(frame, "LF");
    }

    if (strcmp(par1->name, "INT_ID") == 0)
    {
        if (strcmp(symtab->name, "GT" ) == 0)
        {
            strcpy(param1, "GF");
        }
        else
        {
            strcpy(param1, "LF");
        }

        print1 = malloc(sizeof(char) * (strlen(par1->info.ptr->var.key) + 1));
        strcpy(print1, par1->info.ptr->var.key);
    }
    else if (strcmp(par1->name, "INT") == 0)
    {
        strcpy(param1, "int");

        print1 = malloc(sizeof(char) * (strlen(par1->info.string) + 1));
        strcpy(print1, par1->info.string);
    }
    if (strcmp(par2->name, "STR_ID") == 0)
    {
        if (strcmp(symtab->name, "GT" ) == 0)
        {
            strcpy(param2, "GF");
        }
        else
        {
            strcpy(param2, "LF");
        }

        print2 = malloc(sizeof(char) * (strlen(par2->info.ptr->var.key) + 1));
        strcpy(print2, par2->info.ptr->var.key);
    }
    else if (strcmp(par2->name, "STR") == 0)
    {
        strcpy(param2, "string");

        print2 = malloc(sizeof(char) * (strlen(par1->info.string) + 1));
        strcpy(print2, par2->info.string);
    }

    if (strcmp(op->name, "==") == 0)
    {
        printf("%s %s@%s %s@%s %s@%s\n", operator(op->name), frame, des->info.ptr->var.key,
               param1, print1, param2, print2);
    }
    else if (strcmp(op->name, "!=") == 0)
    {
        printf("%s %s@%s %s@%s %s@%s\n", operator(op->name), frame, des->info.ptr->var.key,
               param1, print1, param2, print2);

        printf("DEFVAR %s@%s$tmp\n"
               "MOVE %s@%s$tmp %s@%s\n"
               "NOT %s@%s %s@%s$tmp\n",
               frame,
               des->info.ptr->var.key,
               frame,
               des->info.ptr->var.key,
               frame,
               des->info.ptr->var.key,
               frame,
               des->info.ptr->var.key,
               frame,
               des->info.ptr->var.key
              );
    }
    else
    {
        fprintf(stderr, "%d", 4);
        label_n++;
        /*destroyToken(par1);
        destroyToken(par2);*/
        destroyToken(op);
        free(print1);
        free(print2);
        free(des);
        return NULL;
    }
    label_n++;
    /*destroyToken(par1);
    destroyToken(par2);*/
    destroyToken(op);
    free(print1);
    free(print2);
    return des;
}

token_t *dbl_str(token_t *op, token_t *par1, token_t *par2, symtable_t *symtab)
{
    static unsigned long long label_n = 0;

    char param1[10];
    char param2[10];
    char frame[3];
    char name[20];
    token_info_t *info = malloc(sizeof(token_info_t));
    sprintf(name, "DBL%lluSTR", label_n);
    info->ptr = symtab_elem_add(symtab, name);
    token_t *des = createToken("BOOL_ID", *info);

    char *print1;
    char *print2;

    if (strcmp(symtab->name, "GT" ) == 0)
    {
        strcpy(frame, "GF");
    }
    else
    {
        strcpy(frame, "LF");
    }

    if (strcmp(par1->name, "DBL_ID") == 0)
    {
        if (strcmp(symtab->name, "GT" ) == 0)
        {
            strcpy(param1, "GF");
        }
        else
        {
            strcpy(param1, "LF");
        }

        print1 = malloc(sizeof(char) * (strlen(par1->info.ptr->var.key) + 1));
        strcpy(print1, par1->info.ptr->var.key);
    }
    else if (strcmp(par1->name, "DBL") == 0)
    {
        strcpy(param1, "float");

        print1 = malloc(sizeof(char) * (strlen(par1->info.string) + 1));
        strcpy(print1, par1->info.string);
    }
    if (strcmp(par2->name, "STR_ID") == 0)
    {
        if (strcmp(symtab->name, "GT" ) == 0)
        {
            strcpy(param2, "GF");
        }
        else
        {
            strcpy(param2, "LF");
        }

        print2 = malloc(sizeof(char) * (strlen(par2->info.ptr->var.key) + 1));
        strcpy(print2, par2->info.ptr->var.key);
    }
    else if (strcmp(par2->name, "STR") == 0)
    {
        strcpy(param2, "string");

        print2 = malloc(sizeof(char) * (strlen(par1->info.string) + 1));
        strcpy(print2, par2->info.string);
    }

    if (strcmp(op->name, "==") == 0)
    {
        printf("%s %s@%s %s@%s %s@%s\n", operator(op->name), frame, des->info.ptr->var.key,
               param1, print1, param2, print2);
    }
    else if (strcmp(op->name, "!=") == 0)
    {
        printf("%s %s@%s %s@%s %s@%s\n", operator(op->name), frame, des->info.ptr->var.key,
               param1, print1, param2, print2);

        printf("DEFVAR %s@%s$tmp\n"
               "MOVE %s@%s$tmp %s@%s\n"
               "NOT %s@%s %s@%s$tmp\n",
               frame,
               des->info.ptr->var.key,
               frame,
               des->info.ptr->var.key,
               frame,
               des->info.ptr->var.key,
               frame,
               des->info.ptr->var.key,
               frame,
               des->info.ptr->var.key
              );
    }
    else
    {
        fprintf(stderr, "%d", 4);
        label_n++;
        /*destroyToken(par1);
        destroyToken(par2);*/
        destroyToken(op);
        free(print1);
        free(print2);
        free(des);
        return NULL;
    }
    label_n++;
    /*destroyToken(par1);
    destroyToken(par2);*/
    destroyToken(op);
    free(print1);
    free(print2);
    return des;
}


token_t *int_id(token_t *op, token_t *par1, token_t *par2, symtable_t *symtab)
{

    static unsigned long long label_n = 0;

    char param1[10];
    char frame[3];
    char name[20];
    token_info_t *info = malloc(sizeof(token_info_t));
    sprintf(name, "INT%lluID", label_n);
    info->ptr = symtab_elem_add(symtab, name);
    token_t *des = createToken("ID", *info);

    char *print1;
    char *print2;

    if (strcmp(symtab->name, "GT" ) == 0)
    {
        strcpy(frame, "GF");
    }
    else
    {
        strcpy(frame, "LF");
    }

    if (strcmp(par1->name, "INT_ID") == 0)
    {
        if (strcmp(symtab->name, "GT" ) == 0)
        {
            strcpy(param1, "GF");
        }
        else
        {
            strcpy(param1, "LF");
        }

        print1 = malloc(sizeof(char) * (strlen(par1->info.ptr->var.key) + 1));
        if (print1 == NULL)
        {
            return NULL;
        }
        strcpy(print1, par1->info.ptr->var.key);
    }
    else if (strcmp(par1->name, "INT" ) == 0)
    {

        strcpy(param1, "int");

        print1 = malloc(sizeof(char) * (strlen(par1->info.string) + 1));
        if (print1 == NULL)
        {
            return NULL;
        }
        strcpy(print1, par1->info.string);
    }

    print2 = malloc(sizeof(char) * (strlen(par2->info.ptr->var.key) + 1));
    if (print2 == NULL)
    {
        return NULL;
    }
    strcpy(print2, par2->info.ptr->var.key);


    if (strcmp(op->name, "==") == 0 || strcmp(op->name, "!=") == 0)
    {
        printf("DEFVAR %s@%s%llu$type\n"
               "DEFVAR %s@%s%llu$tmp\n"
               "DEFVAR %s@%llu$tmp$int\n"
               "TYPE %s@%s%llu$type %s@%s\n"
               "MOVE %s@%s%llu$tmp %s@%s\n"
               "MOVE %s@%llu$tmp$int %s@%s\n"
               "JUMPIFEQ $%s$%llu$int %s@%s%llu$type string@int\n"
               "JUMPIFEQ $%s$%llu$int %s@%s%llu$type string@string\n"
               "INT2FLOAT %s@%llu$tmp$int %s@%s\n"
               "JUMP $%s$%llu$int\n"
               "LABEL $%s$%llu$string\n",
               frame,
               print2,
               label_n,//
               frame,
               print2,
               label_n,//
               frame,
               label_n,//
               frame,
               print2,
               label_n,
               frame,
               print2,//
               frame,
               print2,
               label_n,
               frame,
               print2,//`
               frame,
               label_n,
               param1,
               print1,//
               symtab->name,
               label_n,
               frame,
               print2,
               label_n,//
               symtab->name,
               label_n,
               frame,
               print2,
               label_n,//
               frame,
               label_n,
               param1,
               print1,//
               symtab->name,
               label_n,//
               symtab->name,
               label_n//
              );

        if (strcmp(op->name, "!=") == 0)
        {
            printf("DEFVAR %s@%s$not\n"
                   "MOVE %s@%s$not %s@%s\n"
                   "NOT %s@%s %s@%s$not\n",
                   frame,
                   des->info.ptr->var.key,
                   frame,
                   des->info.ptr->var.key,
                   frame,
                   des->info.ptr->var.key,
                   frame,
                   des->info.ptr->var.key,
                   frame,
                   des->info.ptr->var.key
                  );
        }
        printf(
            "JUMP $%s$%llu$end\n"
            "LABEL $%s$%llu$int\n"
            "%s %s@%s %s@%s%llu$tmp %s@%llu$tmp$int\n"
            "JUMPIFEQ $%s$%llu$string %s@%s%llu$type string@string\n"
            "LABEL $%s$%llu$end\n",
            symtab->name,
            label_n,//
            symtab->name,
            label_n,//
            operator(op->name),
            frame,
            des->info.ptr->var.key,
            frame,
            print2,
            label_n,
            frame,
            label_n,//
            symtab->name,
            label_n,
            frame,
            print2,
            label_n,//
            symtab->name,
            label_n//
        );
    }
    else
    {
        printf("DEFVAR %s@%s%llu$type\n"
               "DEFVAR %s@%s%llu$tmp\n"
               "DEFVAR %s@%llu$tmp$int\n"
               "TYPE %s@%s%llu$type %s@%s\n"
               "MOVE %s@%s%llu$tmp %s@%s\n"
               "MOVE %s@%llu$tmp$int %s@%s\n"
               "JUMPIFEQ $%s$%llu$int %s@%s%llu$type string@int\n"
               "JUMPIFEQ $%s$%llu$float %s@%s%llu$type string@float\n"
               "EXIT int@4\n"
               "LABEL $%s$%llu$float\n"
               "INT2FLOAT %s@%llu$tmp$int %s@%s\n"
               "LABEL $%s$%llu$int\n"
               "%s %s@%s %s@%s%llu$tmp %s@%llu$tmp$int\n",
               frame,
               print2,
               label_n,//
               frame,
               print2,
               label_n,//
               frame,
               label_n,//
               frame,
               print2,
               label_n,
               frame,
               print2,//
               frame,
               print2,
               label_n,
               frame,
               print2,//
               frame,
               label_n,
               param1,
               print1,//
               symtab->name,
               label_n,
               frame,
               print2,
               label_n,//
               symtab->name,
               label_n,
               frame,
               print2,
               label_n,//
               symtab->name,
               label_n,//
               frame,
               label_n,
               param1,
               print1,//
               symtab->name,
               label_n,//
               operator(op->name),
               frame,
               des->info.ptr->var.key,
               frame,
               print2,
               label_n,
               frame,
               label_n//
              );
        if (strcmp(op->name, "<=") == 0 || strcmp(op->name, ">=") == 0)
        {
            printf("DEFVAR %s@%s$eq\n"
                   "DEFVAR %s@%s$or\n"
                   "MOVE %s@%s$or %s@%s\n"
                   "EQ %s@%s$eq %s@%s%llu$tmp %s@%llu$tmp$int\n"
                   "OR %s@%s %s@%s$eq %s@%s$or\n",
                   frame,
                   des->info.ptr->var.key,//
                   frame,
                   des->info.ptr->var.key,//
                   frame,
                   des->info.ptr->var.key,//
                   frame,
                   des->info.ptr->var.key,
                   frame,
                   des->info.ptr->var.key,
                   frame,
                   print2,
                   label_n,
                   frame,
                   label_n,//
                   frame,
                   des->info.ptr->var.key,
                   frame,
                   des->info.ptr->var.key,
                   frame,
                   des->info.ptr->var.key
                  );
        }

    }

    label_n++;
    /*destroyToken(par1);
    destroyToken(par2);*/
    destroyToken(op);
    free(print1);
    free(print2);
    return des;
}

token_t *dbl_id(token_t *op, token_t *par1, token_t *par2, symtable_t *symtab)
{

    static unsigned long long label_n = 0;

    char param1[10];
    char frame[3];
    char name[20];
    token_info_t *info = malloc(sizeof(token_info_t));
    sprintf(name, "DBL%lluID", label_n);
    info->ptr = symtab_elem_add(symtab, name);
    token_t *des = createToken("ID", *info);

    char *print1;
    char *print2;

    if (strcmp(symtab->name, "GT" ) == 0)
    {
        strcpy(frame, "GF");
    }
    else
    {
        strcpy(frame, "LF");
    }

    if (strcmp(par1->name, "DBL_ID") == 0)
    {
        if (strcmp(symtab->name, "GT" ) == 0)
        {
            strcpy(param1, "GF");
        }
        else
        {
            strcpy(param1, "LF");
        }

        print1 = malloc(sizeof(char) * (strlen(par1->info.ptr->var.key) + 1));
        if (print1 == NULL)
        {
            return NULL;
        }
        strcpy(print1, par1->info.ptr->var.key);
    }
    else if (strcmp(par1->name, "DBL" ) == 0)
    {

        strcpy(param1, "float");

        print1 = malloc(sizeof(char) * (strlen(par1->info.string) + 1));
        if (print1 == NULL)
        {
            return NULL;
        }
        strcpy(print1, par1->info.string);
    }

    print2 = malloc(sizeof(char) * (strlen(par2->info.ptr->var.key) + 1));
    if (print2 == NULL)
    {
        return NULL;
    }
    strcpy(print2, par2->info.ptr->var.key);


    if (strcmp(op->name, "==") == 0 || strcmp(op->name, "!=") == 0)
    {
        printf("DEFVAR %s@%s%llu$type\n"
               "DEFVAR %s@%s%llu$tmp\n"
               "TYPE %s@%s%llu$type %s@%s\n"
               "MOVE %s@%s%llu$tmp %s@%s\n"
               "JUMPIFEQ $%s$%llu$float %s@%s%llu$type string@float\n"
               "JUMPIFEQ $%s$%llu$float %s@%s%llu$type string@string\n"
               "INT2FLOAT %s@%s%llu$tmp %s@%s\n"
               "JUMP $%s$%llu$float\n"
               "LABEL $%s$%llu$string\n",
               frame,
               print2,
               label_n,//
               frame,
               print2,
               label_n,//
               frame,
               print2,
               label_n,
               frame,
               print2,//
               frame,
               print2,
               label_n,
               frame,
               print2,//
               symtab->name,
               label_n,
               frame,
               print2,
               label_n,//
               symtab->name,
               label_n,
               frame,
               print2,
               label_n,//
               frame,
               print2,
               label_n,
               frame,
               print2,//
               symtab->name,
               label_n,//
               symtab->name,
               label_n//
              );

        if (strcmp(op->name, "!=") == 0)
        {
            printf("DEFVAR %s@%s$not\n"
                   "MOVE %s@%s$not %s@%s\n"
                   "NOT %s@%s %s@%s$not\n",
                   frame,
                   des->info.ptr->var.key,
                   frame,
                   des->info.ptr->var.key,
                   frame,
                   des->info.ptr->var.key,
                   frame,
                   des->info.ptr->var.key,
                   frame,
                   des->info.ptr->var.key
                  );
        }
        printf(
            "JUMP $%s$%llu$end\n"
            "LABEL $%s$%llu$float\n"
            "%s %s@%s %s@%s%llu$tmp %s@%llu$tmp$float\n"
            "JUMPIFEQ $%s$%llu$string %s@%s%llu$type string@string\n"
            "LABEL $%s$%llu$end\n",
            symtab->name,
            label_n,//
            symtab->name,
            label_n,//
            operator(op->name),
            frame,
            des->info.ptr->var.key,
            frame,
            print2,
            label_n,
            frame,
            label_n,//
            symtab->name,
            label_n,
            frame,
            print2,
            label_n,//
            symtab->name,
            label_n//
        );
    }
    else
    {
        printf("DEFVAR %s@%s%llu$type\n"
               "DEFVAR %s@%s%llu$tmp\n"
               "TYPE %s@%s%llu$type %s@%s\n"
               "MOVE %s@%s%llu$tmp %s@%s\n"
               "JUMPIFEQ $%s$%llu$int %s@%s%llu$type string@int\n"
               "JUMPIFEQ $%s$%llu$float %s@%s%llu$type string@float\n"
               "EXIT int@4\n"
               "LABEL $%s$%llu$int\n"
               "INT2FLOAT %s@%s%llu$tmp %s@%s\n"
               "LABEL $%s$%llu$float\n"
               "%s %s@%s %s@%s%llu$tmp %s@%s\n",
               frame,
               print2,
               label_n,//
               frame,
               print2,
               label_n,//
               frame,
               print2,
               label_n,
               frame,
               print2,//
               frame,
               print2,
               label_n,
               frame,
               print2,//
               symtab->name,
               label_n,
               frame,
               print2,
               label_n,//
               symtab->name,
               label_n,
               frame,
               print2,
               label_n,//
               symtab->name,
               label_n,//
               frame,
               print2,
               label_n,
               frame,
               print2,
               symtab->name,
               label_n,//
               operator(op->name),
               frame,
               des->info.ptr->var.key,
               frame,
               print2,
               label_n,
               param1,
               print1//
              );
        if (strcmp(op->name, "<=") == 0 || strcmp(op->name, ">=") == 0)
        {
            printf("DEFVAR %s@%s$eq\n"
                   "DEFVAR %s@%s$or\n"
                   "MOVE %s@%s$or %s@%s\n"
                   "EQ %s@%s$eq %s@%s%llu$tmp %s@%s\n"
                   "OR %s@%s %s@%s$eq %s@%s$or\n",
                   frame,
                   des->info.ptr->var.key,//
                   frame,
                   des->info.ptr->var.key,//
                   frame,
                   des->info.ptr->var.key,//
                   frame,
                   des->info.ptr->var.key,
                   frame,
                   des->info.ptr->var.key,
                   frame,
                   print2,
                   label_n,
                   param1,
                   print1,//
                   frame,
                   des->info.ptr->var.key,
                   frame,
                   des->info.ptr->var.key,
                   frame,
                   des->info.ptr->var.key
                  );
        }

    }

    label_n++;
    /*destroyToken(par1);
    destroyToken(par2);*/
    destroyToken(op);
    free(print1);
    free(print2);
    return des;
}

token_t *gen_expr(token_t *op, token_t *param1, token_t *param2, symtable_t *symtab)
{
    switch (type(param1->name, param2->name))
    {
        case INT_INT:
            return int_int(op, param1, param2, symtab);
            break;
        case INT_DBL:
            return int_dbl(op, param1, param2, symtab);
            break;
        case INT_STR:
            return int_str(op, param1, param2, symtab);
            break;
        case INT_ID:
            return int_id(op, param1, param2, symtab);
            break;
        case DBL_INT:
            return int_dbl(op, param2, param1, symtab);
            break;
        case DBL_DBL:
            return dbl_dbl(op, param1, param2, symtab);
            break;
        case DBL_STR:
            return dbl_str(op, param1, param2, symtab);
            break;
        case DBL_ID:
            return dbl_id(op, param1, param2, symtab);
            break;
        case STR_INT:
            return int_str(op, param2, param1, symtab);
            break;
        case STR_DBL:
            return dbl_str(op, param2, param1, symtab);
            break;
        case STR_STR:
            //return int_int(op, param1, param2, symtab);
            break;
        case STR_ID:
            //return int_int(op, param1, param2, symtab);
            break;
        case ID_INT:
            return int_id(op, param2, param1, symtab);
            break;
        case ID_DBL:
            return dbl_id(op, param2, param1, symtab);
            break;
        case ID_STR:
            //return int_int(op, param1, param2, symtab);
            break;
        case ID_ID:
            //return int_int(op, param1, param2, symtab);
            break;

        default: break;
    }
    return (token_t *) NULL;
}

#if 0
#endif

int main()
{
    elem_t elem;
    elem.var.key = "jsem0ID";
    token_info_t *info = malloc(sizeof(token_info_t));
    info->ptr = &elem;
    token_t *token = createToken("ID", *info);


    elem_t elem1;
    elem1.var.key = "jsem0INT_ID";
    token_info_t info2;
    info2.ptr = &elem1;
    token_t *token1 = createToken("INT_ID", info2);

    elem_t elem2;
    elem2.var.key = "jsem0STR_ID";
    token_info_t info5;
    info5.ptr = &elem2;
    token_t *token4 = createToken("DBL_ID", info5);


    symtable_t *symtable = symtab_init("GT", VARIABLES);

    token_info_t tmp;
    tmp.ptr = NULL;


    token_t *op = createToken("!=", tmp);
    token_info_t info3;
    info3.string = "5";
    token_info_t info4;
    info4.string = "7.1";
    token_t *token2 = createToken("INT", info3);
    token_t *token3 = createToken("DBL", info4);
    token_t *tmp12 = gen_expr(op, token2, token, symtable);
    /*printf("---%s---\n", tmp12->info.ptr->var.key);
    printf("---%s---\n", tmp12->name);*/
    //token_t *tmp13 = gen_expr(op, tmp12, token2, symtable);

    if (tmp12 == NULL)
    {
        printf("\nC\nH\nY\nB\nA\n");
    }

    free(tmp12);
    free(token);
    free(token1);
    free(op);
    free(token2);
    free(token3);
    free(token4);
    return 0;
}



////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////

