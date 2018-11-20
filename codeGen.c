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

token_t *gen_expr(token_t *op, token_t *par1, token_t *par2, symtable_t *symtab)
{
    switch (type(par1->name, par2->name))
    {

        case INT_INT:
            //int_int(op, dest, param1, param2);
            printf("ahoj jsem tady\n");
            break;
        case INT_DBL:
            //int_int(op, dest, param1, param2);
            break;
        case INT_STR:
            //int_int(op, dest, param1, param2);
            break;
        case INT_ID:
            //int_int(op, dest, param1, param2);
            break;
        case DBL_INT:
            //int_int(op, dest, param1, param2);
            break;
        case DBL_DBL:
            //int_int(op, dest, param1, param2);
            break;
        case DBL_STR:
            //int_int(op, dest, param1, param2);
            break;
        case DBL_ID:
            //int_int(op, dest, param1, param2);
            break;
        case STR_INT:
            //int_int(op, dest, param1, param2);
            break;
        case STR_DBL:
            //int_int(op, dest, param1, param2);
            break;
        case STR_STR:
            //int_int(op, dest, param1, param2);
            break;
        case STR_ID:
            //int_int(op, dest, param1, param2);
            break;
        case ID_INT:
            //int_int(op, dest, param1, param2);
            break;
        case ID_DBL:
            //int_int(op, dest, param1, param2);
            break;
        case ID_STR:
            //int_int(op, dest, param1, param2);
            break;
        case ID_ID:
            //int_int(op, dest, param1, param2);
            break;

        default: break;
    }
    return (token_t *) NULL;
}



#if 0

printf("ADD LF@%s int@%s int@%s\n", des->info.ptr->var.key, par2->info.string, par3->info.string);
/*destroyToken(par2);
destroyToken(par3);*/
destroyToken(op);
token_t *tmp = createToken("INT_ID", des->info);
tmp->info.ptr = des->info.ptr;
destroyToken(des);
return tmp;


if (isID(par3))
{
    if (op->name[0] == '+')
    {
        static unsigned long long label_n = 0;
        printf("DEFVAR LF@%s$type\n"
               "DEFVAR LF@%s$tmp\n"
               "TYPE LF@%s$type LF@%s\n"
               "MOVE LF@%s$tmp LF@a\n"
               "JUMPIFEQ $%s$%llu$int LF@%s$type string@int\n"
               "JUMPIFEQ $%s$%llu$float LF@%s$type string@float\n"
               "EXIT int@4\n"
               "LABEL $%s$%llu$float\n"
               "DEFVAR LF@%llu$tmp$int\n"
               "INT2FLOAT LF@%llu$tmp$int int@%s\n"
               "ADD LF@%s LF@%s$tmp LF@%llu$tmp$int\n"
               "LABEL $%s$%llu$int\n"
               "ADD LF@%s LF@%s$tmp int@%s\n",
               par3->info.ptr->var.key,//
               par3->info.ptr->var.key,//
               par3->info.ptr->var.key,
               par3->info.ptr->var.key,//
               par3->info.ptr->var.key,//
               funName,
               label_n,
               par3->info.ptr->var.key,//
               funName,
               label_n,
               par3->info.ptr->var.key,//
               funName,
               label_n,//
               label_n,//
               label_n,
               par2->info.string,//
               des->info.ptr->var.key,
               par3->info.ptr->var.key,
               label_n,//
               funName,
               label_n,//
               des->info.ptr->var.key,
               par3->info.ptr->var.key,
               par2->info.string//
              );
        label_n++;
        /*destroyToken(par2);
        destroyToken(par3);*/
        destroyToken(op);
        token_t *tmp = createToken("INT_ID", des->info);
        tmp->info.ptr = des->info.ptr;
        destroyToken(des);
        return tmp;

#endif

// a + 1

        int main()
        {
            elem_t elem;
            elem.var.key = "a";
            token_info_t info;
            info.ptr = &elem;



            token_info_t tmp;
            tmp.ptr = NULL;

            token_t *token = createToken("ID", info);
            token_t *op = createToken("+", tmp);
            //printf("DEFVAR LF@%s$type\n", token->info.ptr->var.key);
            token_info_t info2;
            info2.string = "5";
            token_t *token2 = createToken("INT", info2);
            token_t *token3 = createToken("INT", info2);
            token_t *tmp12 = generateExp(op, token2, token3, "foo");


            //free(tmp12);
            free(token);
            free(op);
            free(token2);
            free(token3);
            return 0;
        }



////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////

