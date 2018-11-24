/**CFile****************************************************************

  FileName    []

  SystemName  [IFJ - PROJECT]

  PackageName [Code generation implementation]

  Synopsis    []

  Author      [Lukas Valek]

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
    /*ID_index */    { ID_INT , ID_DBL , ID_STR , ID_ID  },
};

char *operator(char *op, bool alternative)
{

    if (strcmp(op, "+") == 0)
    {
        if (alternative)
        {
            return "CONCAT";
        }
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
        return NULL;
    }
}


int type(token_t *param1, token_t *param2)
{
    int index_param1, index_param2;
    /*if (param2 == NULL)
    {
        return NULL_NULL;
    }*/
    if (strncmp(param1->name, "INT", 3) == 0)
    {
        index_param1 = INT_index;
    }
    else if (strncmp(param1->name, "DBL", 3) == 0)
    {
        index_param1 = DBL_index;
    }
    else if (strncmp(param1->name, "STR", 3) == 0)
    {
        index_param1 = STR_index;
    }
    else if (strcmp(param1->name, "ID") == 0)
    {
        index_param1 = ID_index;
    }

    if (strncmp(param2->name, "INT", 3) == 0)
    {
        index_param2 = INT_index;
    }
    else if (strncmp(param2->name, "DBL", 3) == 0)
    {
        index_param2 = DBL_index;
    }
    else if (strncmp(param2->name, "STR", 3) == 0)
    {
        index_param2 = STR_index;
    }
    else if (strcmp(param2->name, "ID") == 0)
    {
        index_param2 = ID_index;
    }

    return matrix[index_param1][index_param2];
}

/*token_t *null_null(token_t *par1, symtable_t *symtab)
{
    static unsigned long long label_n = 0;

    char param1[10];
    char frame[3];

    char *print1;
    token_t *des;
    char name[20];
    token_info_t info;

    if (strcmp(symtab->name, "$GT" ) == 0)
    {
        strcpy(frame, "GF");
    }
    else
    {
        strcpy(frame, "LF");
    }



    if (strcmp(par1->name, "INT") == 0)
    {
        strcpy(param1, "int");
        print1 = malloc(sizeof(char) * (strlen(par1->info.string) + 1));
        strcpy(print1, par1->info.string);
        sprintf(name, "INT%lluNULL", label_n);
        info.ptr = symtab_elem_add(symtab, name);
        des = createToken("INT_ID", info);
    }

    else if (strcmp(par1->name, "DBL") == 0)
    {
        strcpy(param1, "float");
        print1 = malloc(sizeof(char) * (strlen(par1->info.string) + 1));
        strcpy(print1, par1->info.string);
        sprintf(name, "DBL%lluNULL", label_n);
        info.ptr = symtab_elem_add(symtab, name);
        des = createToken("DBL_ID", info);
    }

    else if (strcmp(par1->name, "STR") == 0)
    {
        strcpy(param1, "string");
        print1 = malloc(sizeof(char) * (strlen(par1->info.string) + 1));
        strcpy(print1, par1->info.string);
        sprintf(name, "STR%lluNULL", label_n);
        info.ptr = symtab_elem_add(symtab, name);
        des = createToken("STR_ID", info);
    }


    printf("DEFVAR %s@%s\n"
           "MOVE %s@%s %s@%s\n",

           frame,
           des->info.ptr->var.key,
           frame,
           des->info.ptr->var.key,
           param1,
           print1//
          );

    label_n++;
    destroyToken(par1);
    free(print1);
    return des;
}
*/
token_t *int_int(token_t *op, token_t *par1, token_t *par2, symtable_t *symtab)
{
    if (operator(op->name, 0) == NULL)
    {
        return NULL;
    }
    static unsigned long long label_n = 0;

    char param1[10];
    char param2[10];
    char frame[3];
    char name[20];
    char div[2] = "";
    token_info_t info;
    sprintf(name, "INT%lluINT", label_n);
    info.ptr = symtab_elem_add(symtab, name);
    token_t *des = createToken("BOOL_ID", info);

    char *print1;
    char *print2;

    if (strcmp(symtab->name, "$GT" ) == 0)
    {
        strcpy(frame, "GF");
    }
    else
    {
        strcpy(frame, "LF");
    }

    if (strcmp(par1->name, "INT_ID") == 0)
    {
        if (strcmp(symtab->name, "$GT" ) == 0)
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
            free(des->name);
            free(des);
            token_info_t info1;
            token_t *error = createToken("ERR_INT", info1);
            return error;
        }
        strcpy(print1, par1->info.ptr->var.key);
    }
    else if (strcmp(par1->name, "INT") == 0)
    {
        strcpy(param1, "int");

        print1 = malloc(sizeof(char) * (strlen(par1->info.string) + 1));
        if (print1 == NULL)
        {
            free(des->name);
            free(des);
            token_info_t info1;
            token_t *error = createToken("ERR_INT", info1);
            return error;
        }
        strcpy(print1, par1->info.string);
    }
    if (strcmp(par2->name, "INT_ID") == 0)
    {
        if (strcmp(symtab->name, "$GT" ) == 0)
        {
            strcpy(param2, "GF");
        }
        else
        {
            strcpy(param2, "LF");
        }

        print2 = malloc(sizeof(char) * (strlen(par2->info.ptr->var.key) + 1));
        if (print2 == NULL)
        {
            free(des->name);
            free(des);
            token_info_t info1;
            token_t *error = createToken("ERR_INT", info1);
            return error;
        }
        strcpy(print2, par2->info.ptr->var.key);
    }
    else if (strcmp(par2->name, "INT") == 0)
    {
        strcpy(param2, "int");

        print2 = malloc(sizeof(char) * (strlen(par1->info.string) + 1));
        if (print2 == NULL)
        {
            free(des->name);
            free(des);
            token_info_t info1;
            token_t *error = createToken("ERR_INT", info1);
            return error;
        }
        strcpy(print2, par2->info.string);
    }

    if (strcmp(op->name, "/") == 0)
    {
        strcpy(div, "I");
    }
    printf("DEFVAR %s@%s\n", frame, name);
    printf("%s%s %s@%s %s@%s %s@%s\n", div, operator(op->name, 0), frame, des->info.ptr->var.key,
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

    if (strcmp(op->name, "+") == 0 || strcmp(op->name, "-") == 0 || strcmp(op->name, "*") == 0 ||
            strcmp(op->name, "/") == 0)
    {
        strncpy(des->name, "INT_ID", 8);
    }
    //printf("DEFVAR %s@%s\n", name, des->info.ptr->var.key);


    label_n++;
    /*destroyToken(par1);
    destroyToken(par2);
    destroyToken(op);*/
    free(print1);
    free(print2);
    return des;
}

token_t *dbl_dbl(token_t *op, token_t *par1, token_t *par2, symtable_t *symtab)
{
    if (operator(op->name, 0) == NULL)
    {
        return NULL;
    }
    static unsigned long long label_n = 0;

    char param1[10];
    char param2[10];
    char frame[3];
    char name[20];
    token_info_t info;
    sprintf(name, "DBL%lluDBL", label_n);
    info.ptr = symtab_elem_add(symtab, name);
    token_t *des = createToken("DBL_ID", info);

    char *print1;
    char *print2;

    if (strcmp(symtab->name, "$GT" ) == 0)
    {
        strcpy(frame, "GF");
    }
    else
    {
        strcpy(frame, "LF");
    }

    if (strcmp(par1->name, "DBL_ID") == 0)
    {
        if (strcmp(symtab->name, "$GT" ) == 0)
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
            free(des->name);
            free(des);
            token_info_t info1;
            token_t *error = createToken("ERR_INT", info1);
            return error;
        }
        strcpy(print1, par1->info.ptr->var.key);
    }
    else if (strcmp(par1->name, "DBL") == 0)
    {
        strcpy(param1, "float");

        print1 = malloc(sizeof(char) * (strlen(par1->info.string) + 1));
        if (print1 == NULL)
        {
            free(des->name);
            free(des);
            token_info_t info1;
            token_t *error = createToken("ERR_INT", info1);
            return error;
        }
        strcpy(print1, par1->info.string);
    }
    if (strcmp(par2->name, "DBL_ID") == 0)
    {
        if (strcmp(symtab->name, "$GT" ) == 0)
        {
            strcpy(param2, "GF");
        }
        else
        {
            strcpy(param2, "LF");
        }

        print2 = malloc(sizeof(char) * (strlen(par2->info.ptr->var.key) + 1));
        if (print2 == NULL)
        {
            free(des->name);
            free(des);
            token_info_t info1;
            token_t *error = createToken("ERR_INT", info1);
            return error;
        }
        strcpy(print2, par2->info.ptr->var.key);
    }
    else if (strcmp(par2->name, "DBL") == 0)
    {
        strcpy(param2, "float");

        print2 = malloc(sizeof(char) * (strlen(par1->info.string) + 1));
        if (print2 == NULL)
        {
            free(des->name);
            free(des);
            token_info_t info1;
            token_t *error = createToken("ERR_INT", info1);
            return error;
        }
        strcpy(print2, par2->info.string);
    }

    printf("%s %s@%s %s@%s %s@%s\n", operator(op->name, 0), frame, des->info.ptr->var.key,
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
    destroyToken(par1);
    destroyToken(par2);
    destroyToken(op);
    free(print1);
    free(print2);
    return des;
}

token_t *int_dbl(token_t *op, token_t *par1, token_t *par2, symtable_t *symtab, bool switched)
{
    if (operator(op->name, 0) == NULL)
    {
        return NULL;
    }
    static unsigned long long label_n = 0;
    char param1[10];
    char param2[10];
    char frame[3];
    char name[20];
    token_info_t info;
    sprintf(name, "INT%lluDBL", label_n);
    info.ptr = symtab_elem_add(symtab, name);
    token_t *des = createToken("DBL_ID", info);

    char *print1;
    char *print2;

    if (strcmp(symtab->name, "$GT" ) == 0)
    {
        strcpy(frame, "GF");
    }
    else
    {
        strcpy(frame, "LF");
    }

    if (strcmp(par1->name, "INT_ID") == 0 || strcmp(par1->name, "DBL_ID") == 0)
    {
        if (strcmp(symtab->name, "$GT" ) == 0)
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
            free(des->name);
            free(des);
            token_info_t info1;
            token_t *error = createToken("ERR_INT", info1);
            return error;
        }
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
        if (print1 == NULL)
        {
            free(des->name);
            free(des);
            token_info_t info1;
            token_t *error = createToken("ERR_INT", info1);
            return error;
        }
        strcpy(print1, par1->info.string);
    }
    if (strcmp(par2->name, "INT_ID") == 0 || strcmp(par2->name, "DBL_ID") == 0)
    {
        if (strcmp(symtab->name, "$GT" ) == 0)
        {
            strcpy(param2, "GF");
        }
        else
        {
            strcpy(param2, "LF");
        }

        print2 = malloc(sizeof(char) * (strlen(par2->info.ptr->var.key) + 1));
        if (print2 == NULL)
        {
            free(des->name);
            free(des);
            token_info_t info1;
            token_t *error = createToken("ERR_INT", info1);
            return error;
        }
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
        if (print2 == NULL)
        {
            free(des->name);
            free(des);
            token_info_t info1;
            token_t *error = createToken("ERR_INT", info1);
            return error;
        }
        strcpy(print2, par2->info.string);
    }


    printf("DEFVAR %s@%llu$tmp$1\n"
           "INT2FLOAT %s@%llu$tmp$1 %s@%s\n",

           frame,
           label_n,//
           frame,
           label_n,
           param1,
           print1//
          );
    if (switched)
    {
        printf("%s %s@%s %s@%s %s@%llu$tmp$1\n", operator(op->name, 0),
               frame,
               des->info.ptr->var.key,
               param2,
               print2,
               frame,
               label_n//
              );

    }
    else
    {
        printf("%s %s@%s %s@%llu$tmp$1 %s@%s\n", operator(op->name, 0),
               frame,
               des->info.ptr->var.key,
               frame,
               label_n,
               param2,
               print2
              );
    }

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
    destroyToken(par1);
    destroyToken(par2);
    destroyToken(op);
    free(print1);
    free(print2);
    return des;

}

token_t *int_str(token_t *op, token_t *par1, token_t *par2, symtable_t *symtab)
{
    if (operator(op->name, 0) == NULL)
    {
        return NULL;
    }
    static unsigned long long label_n = 0;

    char param1[10];
    char param2[10];
    char frame[3];
    char name[20];
    token_info_t info;
    sprintf(name, "INT%lluSTR", label_n);
    info.ptr = symtab_elem_add(symtab, name);
    token_t *des = createToken("BOOL_ID", info);

    char *print1;
    char *print2;

    if (strcmp(symtab->name, "$GT" ) == 0)
    {
        strcpy(frame, "GF");
    }
    else
    {
        strcpy(frame, "LF");
    }

    if (strcmp(par1->name, "INT_ID") == 0)
    {
        if (strcmp(symtab->name, "$GT" ) == 0)
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
            free(des->name);
            free(des);
            token_info_t info1;
            token_t *error = createToken("ERR_INT", info1);
            return error;
        }
        strcpy(print1, par1->info.ptr->var.key);
    }
    else if (strcmp(par1->name, "INT") == 0)
    {
        strcpy(param1, "int");

        print1 = malloc(sizeof(char) * (strlen(par1->info.string) + 1));
        if (print1 == NULL)
        {
            free(des->name);
            free(des);
            token_info_t info1;
            token_t *error = createToken("ERR_INT", info1);
            return error;
        }
        strcpy(print1, par1->info.string);
    }
    if (strcmp(par2->name, "STR_ID") == 0)
    {
        if (strcmp(symtab->name, "$GT" ) == 0)
        {
            strcpy(param2, "GF");
        }
        else
        {
            strcpy(param2, "LF");
        }

        print2 = malloc(sizeof(char) * (strlen(par2->info.ptr->var.key) + 1));
        if (print2 == NULL)
        {
            free(des->name);
            free(des);
            token_info_t info1;
            token_t *error = createToken("ERR_INT", info1);
            return error;
        }
        strcpy(print2, par2->info.ptr->var.key);
    }
    else if (strcmp(par2->name, "STR") == 0)
    {
        strcpy(param2, "string");

        print2 = malloc(sizeof(char) * (strlen(par1->info.string) + 1));
        if (print2 == NULL)
        {
            free(des->name);
            free(des);
            token_info_t info1;
            token_t *error = createToken("ERR_INT", info1);
            return error;
        }
        strcpy(print2, par2->info.string);
    }

    if (strcmp(op->name, "==") == 0)
    {
        printf("%s %s@%s %s@%s %s@%s\n", operator(op->name, 0), frame, des->info.ptr->var.key,
               param1, print1, param2, print2);
    }
    else if (strcmp(op->name, "!=") == 0)
    {
        printf("%s %s@%s %s@%s %s@%s\n", operator(op->name, 0), frame, des->info.ptr->var.key,
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
        label_n++;
        destroyToken(par1);
        destroyToken(par2);
        destroyToken(op);
        free(print1);
        free(print2);
        free(des);
        token_info_t info1;
        token_t *error = createToken("ERR_SEM", info1);
        return error;
    }
    label_n++;
    destroyToken(par1);
    destroyToken(par2);
    destroyToken(op);
    free(print1);
    free(print2);
    return des;
}

token_t *dbl_str(token_t *op, token_t *par1, token_t *par2, symtable_t *symtab)
{

    if (operator(op->name, 0) == NULL)
    {
        return NULL;
    }
    static unsigned long long label_n = 0;

    char param1[10];
    char param2[10];
    char frame[3];
    char name[20];
    token_info_t info;
    sprintf(name, "DBL%lluSTR", label_n);
    info.ptr = symtab_elem_add(symtab, name);
    token_t *des = createToken("BOOL_ID", info);

    char *print1;
    char *print2;

    if (strcmp(symtab->name, "$GT" ) == 0)
    {
        strcpy(frame, "GF");
    }
    else
    {
        strcpy(frame, "LF");
    }

    if (strcmp(par1->name, "DBL_ID") == 0)
    {
        if (strcmp(symtab->name, "$GT" ) == 0)
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
            free(des->name);
            free(des);
            token_info_t info1;
            token_t *error = createToken("ERR_INT", info1);
            return error;
        }
        strcpy(print1, par1->info.ptr->var.key);
    }
    else if (strcmp(par1->name, "DBL") == 0)
    {
        strcpy(param1, "float");

        print1 = malloc(sizeof(char) * (strlen(par1->info.string) + 1));
        if (print1 == NULL)
        {
            free(des->name);
            free(des);
            token_info_t info1;
            token_t *error = createToken("ERR_INT", info1);
            return error;
        }
        strcpy(print1, par1->info.string);
    }
    if (strcmp(par2->name, "STR_ID") == 0)
    {
        if (strcmp(symtab->name, "$GT" ) == 0)
        {
            strcpy(param2, "GF");
        }
        else
        {
            strcpy(param2, "LF");
        }

        print2 = malloc(sizeof(char) * (strlen(par2->info.ptr->var.key) + 1));
        if (print2 == NULL)
        {
            free(des->name);
            free(des);
            token_info_t info1;
            token_t *error = createToken("ERR_INT", info1);
            return error;
        }
        strcpy(print2, par2->info.ptr->var.key);
    }
    else if (strcmp(par2->name, "STR") == 0)
    {
        strcpy(param2, "string");

        print2 = malloc(sizeof(char) * (strlen(par1->info.string) + 1));
        if (print2 == NULL)
        {
            free(des->name);
            free(des);
            token_info_t info1;
            token_t *error = createToken("ERR_INT", info1);
            return error;
        }
        strcpy(print2, par2->info.string);
    }

    if (strcmp(op->name, "==") == 0)
    {
        printf("%s %s@%s %s@%s %s@%s\n", operator(op->name, 0), frame, des->info.ptr->var.key,
               param1, print1, param2, print2);
    }
    else if (strcmp(op->name, "!=") == 0)
    {
        printf("%s %s@%s %s@%s %s@%s\n", operator(op->name, 0), frame, des->info.ptr->var.key,
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
        label_n++;
        destroyToken(par1);
        destroyToken(par2);
        destroyToken(op);
        free(print1);
        free(print2);
        free(des);
        token_info_t info1;
        token_t *error = createToken("ERR_SEM", info1);
        return error;
    }

    label_n++;
    destroyToken(par1);
    destroyToken(par2);
    destroyToken(op);
    free(print1);
    free(print2);
    return des;
}


token_t *int_id(token_t *op, token_t *par1, token_t *par2, symtable_t *symtab, bool switched)
{
    if (operator(op->name, 0) == NULL)
    {
        return NULL;
    }
    static unsigned long long label_n = 0;

    char param1[10];
    char frame[3];
    char name[20];
    token_info_t info;
    sprintf(name, "INT%lluID", label_n);
    info.ptr = symtab_elem_add(symtab, name);
    token_t *des = createToken("ID", info);

    char *print1;
    char *print2;

    if (strcmp(symtab->name, "$GT" ) == 0)
    {
        strcpy(frame, "GF");
    }
    else
    {
        strcpy(frame, "LF");
    }

    if (strcmp(par1->name, "INT_ID") == 0)
    {
        if (strcmp(symtab->name, "$GT" ) == 0)
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
            free(des->name);
            free(des);
            token_info_t info1;
            token_t *error = createToken("ERR_INT", info1);
            return error;
        }
        strcpy(print1, par1->info.ptr->var.key);
    }
    else if (strcmp(par1->name, "INT" ) == 0)
    {

        strcpy(param1, "int");

        print1 = malloc(sizeof(char) * (strlen(par1->info.string) + 1));
        if (print1 == NULL)
        {
            free(des->name);
            free(des);
            token_info_t info1;
            token_t *error = createToken("ERR_INT", info1);
            return error;
        }
        strcpy(print1, par1->info.string);
    }

    print2 = malloc(sizeof(char) * (strlen(par2->info.ptr->var.key) + 1));
    if (print2 == NULL)
    {
        free(des->name);
        free(des);
        printf("%d\n", 99);
        token_info_t info1;
        token_t *error = createToken("ERR_INT", info1);
        return error;
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
               operator(op->name, 0),
               frame,
               des->info.ptr->var.key,
               frame,
               print2,
               label_n,
               frame,
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
               "LABEL $%s$%llu$int\n",
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
               label_n//
              );

        if (switched)
        {
            printf("%s %s@%s %s@%s%llu$tmp %s@%llu$tmp$int\n", operator(op->name, 0),
                   frame,
                   des->info.ptr->var.key,
                   frame,
                   print2,
                   label_n,
                   frame,
                   label_n
                  );

        }
        else
        {
            printf("%s %s@%s %s@%llu$tmp$int %s@%s%llu$tmp \n", operator(op->name, 0),
                   frame,
                   des->info.ptr->var.key,
                   frame,
                   label_n,
                   frame,
                   print2,
                   label_n
                  );
        }
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
    destroyToken(par1);
    destroyToken(par2);
    destroyToken(op);
    free(print1);
    free(print2);
    return des;
}

token_t *dbl_id(token_t *op, token_t *par1, token_t *par2, symtable_t *symtab, bool switched)
{
    if (operator(op->name, 0) == NULL)
    {
        return NULL;
    }
    static unsigned long long label_n = 0;

    char param1[10];
    char frame[3];
    char name[20];
    token_info_t info;
    sprintf(name, "DBL%lluID", label_n);
    info.ptr = symtab_elem_add(symtab, name);
    token_t *des = createToken("ID", info);

    char *print1;
    char *print2;

    if (strcmp(symtab->name, "$GT" ) == 0)
    {
        strcpy(frame, "GF");
    }
    else
    {
        strcpy(frame, "LF");
    }

    if (strcmp(par1->name, "DBL_ID") == 0)
    {
        if (strcmp(symtab->name, "$GT" ) == 0)
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
            free(des->name);
            free(des);
            token_info_t info1;
            token_t *error = createToken("ERR_INT", info1);
            return error;
        }
        strcpy(print1, par1->info.ptr->var.key);
    }
    else if (strcmp(par1->name, "DBL" ) == 0)
    {

        strcpy(param1, "float");

        print1 = malloc(sizeof(char) * (strlen(par1->info.string) + 1));
        if (print1 == NULL)
        {
            free(des->name);
            free(des);
            token_info_t info1;
            token_t *error = createToken("ERR_INT", info1);
            return error;
        }
        strcpy(print1, par1->info.string);
    }

    print2 = malloc(sizeof(char) * (strlen(par2->info.ptr->var.key) + 1));
    if (print2 == NULL)
    {
        free(des->name);
        free(des);
        token_info_t info1;
        token_t *error = createToken("ERR_INT", info1);
        return error;
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
               frame,
               print2,
               label_n,
               frame,
               print2,//
               symtab->name,
               label_n,//
               operator(op->name, 0),
               frame,
               des->info.ptr->var.key,
               frame,
               print2,
               label_n,
               param1,
               print1//
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
               "LABEL $%s$%llu$float\n",
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
               label_n//
              );

        if (switched)
        {
            printf("%s %s@%s %s@%s%llu$tmp %s@%s\n", operator(op->name, 0),
                   frame,
                   des->info.ptr->var.key,
                   frame,
                   print2,
                   label_n,
                   param1,
                   print1//
                  );

        }
        else
        {
            printf("%s %s@%s %s@%s %s@%s%llu$tmp \n", operator(op->name, 0),
                   frame,
                   des->info.ptr->var.key,
                   param1,
                   print1,
                   frame,
                   print2,
                   label_n

                  );
        }
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
    destroyToken(par1);
    destroyToken(par2);
    destroyToken(op);
    free(print1);
    free(print2);
    return des;
}

token_t *str_str(token_t *op, token_t *par1, token_t *par2, symtable_t *symtab)
{
    if (operator(op->name, 0) == NULL)
    {
        return NULL;
    }
    static unsigned long long label_n = 0;

    char param1[10];
    char param2[10];
    char frame[3];
    char name[20];
    token_info_t info;
    sprintf(name, "STR%lluSTR", label_n);
    info.ptr = symtab_elem_add(symtab, name);
    token_t *des = createToken("STR_ID", info);

    char *print1;
    char *print2;

    if (strcmp(symtab->name, "$GT" ) == 0)
    {
        strcpy(frame, "GF");
    }
    else
    {
        strcpy(frame, "LF");
    }

    if (strcmp(par1->name, "STR_ID") == 0)
    {
        if (strcmp(symtab->name, "$GT" ) == 0)
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
            free(des->name);
            free(des);
            token_info_t info1;
            token_t *error = createToken("ERR_INT", info1);
            return error;
        }
        strcpy(print1, par1->info.ptr->var.key);
    }
    else if (strcmp(par1->name, "STR") == 0)
    {
        strcpy(param1, "string");

        print1 = malloc(sizeof(char) * (strlen(par1->info.string) + 1));
        if (print1 == NULL)
        {
            free(des->name);
            free(des);
            token_info_t info1;
            token_t *error = createToken("ERR_INT", info1);
            return error;
        }
        strcpy(print1, par1->info.string);
    }
    if (strcmp(par2->name, "STR_ID") == 0)
    {
        if (strcmp(symtab->name, "$GT" ) == 0)
        {
            strcpy(param2, "GF");
        }
        else
        {
            strcpy(param2, "LF");
        }

        print2 = malloc(sizeof(char) * (strlen(par2->info.ptr->var.key) + 1));
        if (print2 == NULL)
        {
            free(des->name);
            free(des);
            token_info_t info1;
            token_t *error = createToken("ERR_INT", info1);
            return error;
        }
        strcpy(print2, par2->info.ptr->var.key);
    }
    else if (strcmp(par2->name, "STR") == 0)
    {
        strcpy(param2, "string");

        print2 = malloc(sizeof(char) * (strlen(par1->info.string) + 1));
        if (print2 == NULL)
        {
            free(des->name);
            free(des);
            token_info_t info1;
            token_t *error = createToken("ERR_INT", info1);
            return error;
        }
        strcpy(print2, par2->info.string);
    }

    if (strcmp(op->name, "==") == 0)
    {
        printf("%s %s@%s %s@%s %s@%s\n", operator(op->name, 0), frame, des->info.ptr->var.key,
               param1, print1, param2, print2);
    }
    else if (strcmp(op->name, "!=") == 0)
    {
        printf("%s %s@%s %s@%s %s@%s\n", operator(op->name, 0), frame, des->info.ptr->var.key,
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
    else if (strcmp(op->name, "+") == 0)
    {
        printf("%s %s@%s %s@%s %s@%s\n", operator(op->name, 1), frame, des->info.ptr->var.key,
               param1, print1, param2, print2);
    }
    else
    {
        label_n++;
        destroyToken(par1);
        destroyToken(par2);
        destroyToken(op);
        free(print1);
        free(print2);
        free(des);
        token_info_t info1;
        token_t *error = createToken("ERR_SEM", info1);
        return error;
    }
    label_n++;
    destroyToken(par1);
    destroyToken(par2);
    destroyToken(op);
    free(print1);
    free(print2);
    return des;
}

token_t *str_id(token_t *op, token_t *par1, token_t *par2, symtable_t *symtab, bool switched)
{
    if (operator(op->name, 0) == NULL)
    {
        return NULL;
    }
    static unsigned long long label_n = 0;

    char param1[10];
    char frame[3];
    char name[20];
    token_info_t info;
    sprintf(name, "STR%lluID", label_n);
    info.ptr = symtab_elem_add(symtab, name);
    token_t *des = createToken("ID", info);

    char *print1;
    char *print2;

    if (strcmp(symtab->name, "$GT" ) == 0)
    {
        strcpy(frame, "GF");
    }
    else
    {
        strcpy(frame, "LF");
    }

    if (strcmp(par1->name, "STR_ID") == 0)
    {
        if (strcmp(symtab->name, "$GT" ) == 0)
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
            free(des->name);
            free(des);
            token_info_t info1;
            token_t *error = createToken("ERR_INT", info1);
            return error;
        }
        strcpy(print1, par1->info.ptr->var.key);
    }
    else if (strcmp(par1->name, "STR" ) == 0)
    {

        strcpy(param1, "string");

        print1 = malloc(sizeof(char) * (strlen(par1->info.string) + 1));
        if (print1 == NULL)
        {
            free(des->name);
            free(des);
            token_info_t info1;
            token_t *error = createToken("ERR_INT", info1);
            return error;
        }
        strcpy(print1, par1->info.string);
    }

    print2 = malloc(sizeof(char) * (strlen(par2->info.ptr->var.key) + 1));
    if (print2 == NULL)
    {
        free(des->name);
        free(des);
        token_info_t info1;
        token_t *error = createToken("ERR_INT", info1);
        return error;
    }
    strcpy(print2, par2->info.ptr->var.key);


    if (strcmp(op->name, "==") == 0 || strcmp(op->name, "!=") == 0)
    {
        printf("%s %s@%s %s@%s %s@%s\n", operator(op->name, 0),
               frame,
               des->info.ptr->var.key,
               frame,
               print2,
               param1,
               print1//
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
    }

    else if (strcmp(op->name, "+") == 0)
    {
        printf("DEFVAR %s@%s%llu$type\n"
               "TYPE %s@%s%llu$type %s@%s\n"
               "JUMPIFEQ $%s$%llu$string %s@%s%llu$type string@string\n"
               "EXIT int4\n"
               "LABEL $%s$%llu$string\n",
               frame,
               print2,
               label_n,//
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
               label_n//

              );

        if (switched)
        {
            printf("%s %s@%s %s@%s %s@%s\n", operator(op->name, 1),
                   frame,
                   des->info.ptr->var.key,
                   frame,
                   print2,//
                   param1,
                   print1//
                  );
        }
        else
        {
            printf("%s %s@%s %s@%s %s@%s\n", operator(op->name, 1),
                   frame,
                   des->info.ptr->var.key,
                   param1,
                   print1,//
                   frame,
                   print2//
                  );
        }
    }
    else
    {
        label_n++;
        destroyToken(par1);
        destroyToken(par2);
        destroyToken(op);
        free(print1);
        free(print2);
        free(des);
        token_info_t info1;
        token_t *error = createToken("ERR_SEM", info1);
        return error;
    }

    label_n++;
    destroyToken(par1);
    destroyToken(par2);
    destroyToken(op);
    free(print1);
    free(print2);
    return des;
}

token_t *id_id(token_t *op, token_t *par1, token_t *par2, symtable_t *symtab, bool switched)
{
    if (operator(op->name, 0) == NULL)
    {
        return NULL;
    }
    static unsigned long long label_n = 0;

    char frame[3];
    char name[20];
    token_info_t info;
    sprintf(name, "ID%lluID", label_n);
    info.ptr = symtab_elem_add(symtab, name);
    token_t *des = createToken("ID", info);

    char *print1;
    char *print2;

    if (strcmp(symtab->name, "$GT" ) == 0)
    {
        strcpy(frame, "GF");
    }
    else
    {
        strcpy(frame, "LF");
    }

    print1 = malloc(sizeof(char) * (strlen(par1->info.ptr->var.key) + 1));
    if (print1 == NULL)
    {
        free(des->name);
        free(des);
        token_info_t info1;
        token_t *error = createToken("ERR_INT", info1);
        return error;
    }
    strcpy(print1, par1->info.ptr->var.key);


    print2 = malloc(sizeof(char) * (strlen(par2->info.ptr->var.key) + 1));
    if (print2 == NULL)
    {
        free(des->name);
        free(des);
        token_info_t info1;
        token_t *error = createToken("ERR_INT", info1);
        return error;
    }
    strcpy(print2, par2->info.ptr->var.key);



    printf("DEFVAR %s@%s%llu$type0\n"
           "TYPE %s@%s%llu$type0 %s@%s\n"
           "JUMPIFEQ $%s$%llu$int0 %s@%s%llu$type0 string@int\n",
           frame, print1, label_n, frame, print1, label_n, frame, print1,
           symtab->name, label_n, frame, print1, label_n);

    printf("JUMPIFEQ $%s$%llu$float0 %s@%s%llu$type0 string@float\n", symtab->name,
           label_n, frame, print1, label_n );

    printf("JUMPIFEQ $%s$%llu$string0 %s@%s%llu$type0 string@string\n", symtab->name,
           label_n, frame, print1, label_n );

    printf("LABEL $%s$%llu$int0\n", symtab->name,
           label_n);

    if (strcmp(op->name, "==") == 0 || strcmp(op->name, "!=") == 0)
    {
        printf("DEFVAR %s@%s%llu$type1\n"
               "DEFVAR %s@%s%llu$tmp1\n"
               "DEFVAR %s@%llu$tmp$int1\n"
               "TYPE %s@%s%llu$type1 %s@%s\n"
               "MOVE %s@%s%llu$tmp1 %s@%s\n"
               "MOVE %s@%llu$tmp$int1 %s@%s\n"
               "JUMPIFEQ $%s$%llu$int1 %s@%s%llu$type1 string@int\n"
               "JUMPIFEQ $%s$%llu$int1 %s@%s%llu$type1 string@string\n"
               "INT2FLOAT %s@%llu$tmp$int1 %s@%s\n"
               "LABEL $%s$%llu$int1\n"
               "%s %s@%s %s@%s%llu$tmp1 %s@%llu$tmp$int1\n",
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
               frame,
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
               frame,
               print1,//
               symtab->name,
               label_n,//
               operator(op->name, 0),
               frame,
               des->info.ptr->var.key,
               frame,
               print2,
               label_n,
               frame,
               label_n//

              );

        if (strcmp(op->name, "!=") == 0)
        {
            printf("DEFVAR %s@%s$not1\n"
                   "MOVE %s@%s$not1 %s@%s\n"
                   "NOT %s@%s %s@%s$not1\n",
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
    }
    else
    {
        printf("DEFVAR %s@%s%llu$type1\n"
               "DEFVAR %s@%s%llu$tmp1\n"
               "DEFVAR %s@%llu$tmp$int1\n"
               "TYPE %s@%s%llu$type1 %s@%s\n"
               "MOVE %s@%s%llu$tmp1 %s@%s\n"
               "MOVE %s@%llu$tmp$int1 %s@%s\n"
               "JUMPIFEQ $%s$%llu$int1 %s@%s%llu$type1 string@int\n"
               "JUMPIFEQ $%s$%llu$float1 %s@%s%llu$type1 string@float\n"
               "EXIT int@4\n"
               "LABEL $%s$%llu$float1\n"
               "INT2FLOAT %s@%llu$tmp$int1 %s@%s\n"
               "LABEL $%s$%llu$int1\n",
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
               frame,
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
               frame,
               print1,//
               symtab->name,
               label_n//
              );

        if (switched)
        {
            printf("%s %s@%s %s@%s%llu$tmp1 %s@%llu$tmp$int1\n", operator(op->name, 0),
                   frame,
                   des->info.ptr->var.key,
                   frame,
                   print2,
                   label_n,
                   frame,
                   label_n
                  );

        }
        else
        {
            printf("%s %s@%s %s@%llu$tmp$int1 %s@%s%llu$tmp1 \n", operator(op->name, 0),
                   frame,
                   des->info.ptr->var.key,
                   frame,
                   label_n,
                   frame,
                   print2,
                   label_n
                  );
        }
        if (strcmp(op->name, "<=") == 0 || strcmp(op->name, ">=") == 0)
        {
            printf("DEFVAR %s@%s$eq1\n"
                   "DEFVAR %s@%s$or1\n"
                   "MOVE %s@%s$or1 %s@%s\n"
                   "EQ %s@%s$eq1 %s@%s%llu$tmp1 %s@%llu$tmp$int1\n"
                   "OR %s@%s %s@%s$eq1 %s@%s$or\n",
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


    printf("JUMP ENDTHISMADNESS%llu\n", label_n);

    printf("LABEL $%s$%llu$float0\n", symtab->name,
           label_n);


    if (strcmp(op->name, "==") == 0 || strcmp(op->name, "!=") == 0)
    {
        printf("DEFVAR %s@%s%llu$type2\n"
               "DEFVAR %s@%s%llu$tmp2\n"
               "TYPE %s@%s%llu$type2 %s@%s\n"
               "MOVE %s@%s%llu$tmp2 %s@%s\n"
               "JUMPIFEQ $%s$%llu$float2 %s@%s%llu$type2 string@float\n"
               "JUMPIFEQ $%s$%llu$float2 %s@%s%llu$type2 string@string\n"
               "INT2FLOAT %s@%s%llu$tmp2 %s@%s\n"
               "LABEL $%s$%llu$float2\n"
               "%s %s@%s %s@%s%llu$tmp2 %s@%s\n",
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
               operator(op->name, 0),
               frame,
               des->info.ptr->var.key,
               frame,
               print2,
               label_n,
               frame,
               print1//
              );
        if (strcmp(op->name, "!=") == 0)
        {
            printf("DEFVAR %s@%s$not2\n"
                   "MOVE %s@%s$not2 %s@%s\n"
                   "NOT %s@%s %s@%s$not2\n",
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
    }


    else
    {
        printf("DEFVAR %s@%s%llu$type2\n"
               "DEFVAR %s@%s%llu$tmp2\n"
               "TYPE %s@%s%llu$type2 %s@%s\n"
               "MOVE %s@%s%llu$tmp2 %s@%s\n"
               "JUMPIFEQ $%s$%llu$int2 %s@%s%llu$type2 string@int\n"
               "JUMPIFEQ $%s$%llu$float2 %s@%s%llu$type2 string@float\n"
               "EXIT int@4\n"
               "LABEL $%s$%llu$int2\n"
               "INT2FLOAT %s@%s%llu$tmp2 %s@%s\n"
               "LABEL $%s$%llu$float2\n",
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
               label_n//
              );

        if (switched)
        {
            printf("%s %s@%s %s@%s%llu$tmp2 %s@%s\n", operator(op->name, 0),
                   frame,
                   des->info.ptr->var.key,
                   frame,
                   print2,
                   label_n,
                   frame,
                   print1//
                  );

        }
        else
        {
            printf("%s %s@%s %s@%s %s@%s%llu$tmp2 \n", operator(op->name, 0),
                   frame,
                   des->info.ptr->var.key,
                   frame,
                   print1,
                   frame,
                   print2,
                   label_n

                  );
        }
        if (strcmp(op->name, "<=") == 0 || strcmp(op->name, ">=") == 0)
        {
            printf("DEFVAR %s@%s$eq2\n"
                   "DEFVAR %s@%s$or2\n"
                   "MOVE %s@%s$or2 %s@%s\n"
                   "EQ %s@%s$eq2 %s@%s%llu$tmp2 %s@%s\n"
                   "OR %s@%s %s@%s$eq2 %s@%s$or2\n",
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
    printf("JUMP ENDTHISMADNESS%llu\n", label_n);

    printf("LABEL $%s$%llu$string0\n", symtab->name,
           label_n);


    if (strcmp(op->name, "==") == 0 || strcmp(op->name, "!=") == 0)
    {
        printf("%s %s@%s %s@%s %s@%s\n", operator(op->name, 0),
               frame,
               des->info.ptr->var.key,
               frame,
               print2,
               frame,
               print1//
              );
        if (strcmp(op->name, "!=") == 0)
        {
            printf("DEFVAR %s@%s$not3\n"
                   "MOVE %s@%s$not3 %s@%s\n"
                   "NOT %s@%s %s@%s$not3\n",
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
    }

    else if (strcmp(op->name, "+") == 0)
    {
        printf("DEFVAR %s@%s%llu$type3\n"
               "TYPE %s@%s%llu$type3 %s@%s\n"
               "JUMPIFEQ $%s$%llu$string3 %s@%s%llu$type3 string@string\n"
               "EXIT int4\n"
               "LABEL $%s$%llu$string3\n",
               frame,
               print2,
               label_n,//
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
               label_n//

              );

        if (switched)
        {
            printf("%s %s@%s %s@%s %s@%s\n", operator(op->name, 1),
                   frame,
                   des->info.ptr->var.key,
                   frame,
                   print2,//
                   frame,
                   print1//
                  );
        }
        else
        {
            printf("%s %s@%s %s@%s %s@%s\n", operator(op->name, 1),
                   frame,
                   des->info.ptr->var.key,
                   frame,
                   print1,//
                   frame,
                   print2//
                  );
        }
    }

    printf("LABEL ENDTHISMADNESS%llu\n", label_n);

    label_n++;
    destroyToken(par1);
    destroyToken(par2);
    destroyToken(op);
    free(print1);
    free(print2);
    return des;
}

token_t *gen_expr(token_t *op, token_t *param1, token_t *param2, symtable_t *symtab)
{

    switch (type(param1, param2))
    {
        case INT_INT:
            return int_int(op, param1, param2, symtab);
            break;
        case INT_DBL:
            return int_dbl(op, param1, param2, symtab, 0);
            break;
        case INT_STR:
            return int_str(op, param1, param2, symtab);
            break;
        case INT_ID:
            return int_id(op, param1, param2, symtab, 0);
            break;
        case DBL_INT:
            return int_dbl(op, param2, param1, symtab, 1);
            break;
        case DBL_DBL:
            return dbl_dbl(op, param1, param2, symtab);
            break;
        case DBL_STR:
            return dbl_str(op, param1, param2, symtab);
            break;
        case DBL_ID:
            return dbl_id(op, param1, param2, symtab, 0);
            break;
        case STR_INT:
            return int_str(op, param2, param1, symtab);
            break;
        case STR_DBL:
            return dbl_str(op, param2, param1, symtab);
            break;
        case STR_STR:
            return str_str(op, param1, param2, symtab);
            break;
        case STR_ID:
            return str_id(op, param1, param2, symtab, 0);
            break;
        case ID_INT:
            return int_id(op, param2, param1, symtab, 1);
            break;
        case ID_DBL:
            return dbl_id(op, param2, param1, symtab, 1);
            break;
        case ID_STR:
            return str_id(op, param2, param1, symtab, 1);
            break;
        case ID_ID:
            return id_id(op, param1, param2, symtab, 0);
            break;
        /*case NULL_NULL:
            return null_null(param1, symtab);
            break;*/

        default: break;
    }

    token_info_t info1;
    token_t *error = createToken("ERR_INT", info1);
    return error;
}

#if 1


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
    info3.string = "1";
    token_info_t info4;
    info4.string = "7.1";
    token_t *token2 = createToken("INT", info3);
    token_t *token3 = createToken("DBL", info4);
    token_t *tmp12 = gen_expr(op,  token1, token2, symtable);
    printf("---%s---\n", tmp12->info.ptr->var.key);
    printf("---%s---\n", tmp12->name);
    //token_t *tmp13 = gen_expr(op, tmp12, token2, symtable);

    if (tmp12 == NULL)
    {
        printf("\nC\nH\nY\nB\nA\n");
    }

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

