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



#include "code_gen.h"





////////////////////////////////////////////////////////////////////////
///                       GLOBAL VARIABLES                           ///
////////////////////////////////////////////////////////////////////////
/*
GF@$des\n"
GF@$eq\n"
GF@$jump\n"
GF@$tmp\n"
GF@$type\n\n"
*/

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

bool isNIL(token_t *token)
{
    if (strcmp(token->name, "nil") == 0)
    {
        return true;
    }
    return false;
}


void which_frame(symtable_t *symtab, char frame[3]) {
    if (strcmp(symtab->name, "$GT" ) == 0)
    {
        strcpy(frame, "GF");
    }
    else
    {
        strcpy(frame, "LF");
    }
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
    else if (strcmp(param1->name, "nil") == 0 )
    {
        return NIL_ID;
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
    else if (strcmp(param2->name, "nil") == 0 )
    {
        return ID_NIL;
    }

    return matrix[index_param1][index_param2];
}


char *params(symtable_t *symtab, char* param, token_t *par)
{
    char *print;
    if (strcmp(par->name, "INT_ID") == 0 || strcmp(par->name, "DBL_ID") == 0 || strcmp(par->name, "STR_ID") == 0 || strcmp(par->name, "BOOL_ID") == 0)
    {
        which_frame(symtab, param);

        print = malloc(sizeof(char) * (strlen("$des") + 1));
        if (print == NULL)
        {
            return "ERR_INT";
        }
        strcpy(print, "$des");
    }
    else if (strcmp(par->name, "ID") == 0)
    {
        which_frame(symtab, param);

        print = malloc(sizeof(char) * (strlen(par->info.ptr->var.key) + 1));
        if (print == NULL)
        {
            return "ERR_INT";
        }
        strcpy(print, par->info.ptr->var.key);
    }
    else if (strcmp(par->name, "INT" ) == 0 || strcmp(par->name, "DBL") == 0 || strcmp(par->name, "STR") == 0 || strcmp(par->name, "BOOL") == 0)
    {
        if (strcmp(par->name, "INT") == 0)
        {
            strcpy(param, "int");
        }
        else if (strcmp(par->name, "DBL") == 0)
        {
            strcpy(param, "float");
        }
        else if (strcmp(par->name, "STR") == 0)
        {
            strcpy(param, "string");
        }
        else if (strcmp(par->name, "BOOL") == 0)
        {
            strcpy(param, "bool");
        }

        print = malloc(sizeof(char) * (strlen(par->info.string) + 1));
        if (print == NULL)
        {
            return "ERR_INT";
        }
        strcpy(print, par->info.string);
    }
    return print;
}
token_t *nil(token_t *op, token_t *par1, token_t *par2, list_t *code_buffer, bool in_stat)
{

    if (strcmp(op->name, "==") != 0 && strcmp(op->name, "!=") != 0)
    {
        token_info_t info1;
        token_t *error = createToken("ERR_SEM", info1);
        return error;
    }

    static unsigned long long label_n = 0;


    token_info_t info = {.ptr = NULL};


    token_t *des = createToken("BOOL_ID", info);


    if (strcmp(par2->name, "nil") == 0)
    {
        if (strcmp(op->name, "==") == 0)
        {
            print_or_append(code_buffer, in_stat, "MOVE GF@$des bool@true\n");
        }
        else
        {
            print_or_append(code_buffer, in_stat, "MOVE GF@$des bool@false\n");
        }
    }
    else
    {
        if (strcmp(op->name, "==") == 0)
        {
            print_or_append(code_buffer, in_stat, "MOVE GF@$des bool@false\n");
        }
        else
        {
            print_or_append(code_buffer, in_stat, "MOVE GF@$des bool@true\n");
        }
    }

    label_n++;
    destroyToken(par1);
    destroyToken(par2);
    destroyToken(op);
    return des;
}

token_t *int_int(token_t *op, token_t *par1, token_t *par2, symtable_t *symtab, bool variant, list_t *code_buffer, bool in_stat)
{
    if (operator(op->name, 0) == NULL)
    {
        return NULL;
    }
    static unsigned long long label_n = 0;

    char param1[10];
    char param2[10];
    char div_type[6] = "float";
    char div[2] = "";
    token_info_t info = {.ptr = NULL};

    token_t *des = createToken("BOOL_ID", info);

    char *print1 = params(symtab, param1, par1);

    char *print2 = params(symtab, param2, par2);

    if (strcmp(print1, "ERR_INT") == 0 || strcmp(print2, "ERR_INT") == 0)
    {
        destroyToken(des);
        destroyToken(par1);
        destroyToken(par2);
        destroyToken(op);
        token_info_t info1;
        token_t *error = createToken("ERR_SEM", info1);
        return error;
    }

    if (variant == 0)
    {
        strncpy(div_type, "int", 6);
        if (strcmp(op->name, "/") == 0)
        {
            strcpy(div, "I");

            print_or_append(code_buffer, in_stat, "JUMPIFNEQ $%s$%llu$notZero %s@%s %s@0\n"
                            "EXIT int@9\n"
                            "LABEL $%s$%llu$notZero\n", symtab->name, label_n, param2, print2, div_type, symtab->name, label_n);
        }
    }
    else
    {
        if (strcmp(op->name, "/") == 0)
        {
            print_or_append(code_buffer, in_stat, "JUMPIFNEQ $%s$%llu$notZero %s@%s %s@0x0p+0\n"
                            "EXIT int@9\n"
                            "LABEL $%s$%llu$notZero\n", symtab->name, label_n, param2, print2, div_type, symtab->name, label_n);
        }
    }

    print_or_append(code_buffer, in_stat, "%s%s GF@$des %s@%s %s@%s\n", div, operator(op->name, 0), param1, print1, param2, print2);

    if (strcmp(op->name, "<=") == 0 || strcmp(op->name, ">=") == 0)
    {
        print_or_append(code_buffer, in_stat, "EQ GF@$tmp %s@%s %s@%s\n"
                        "OR GF@$des GF@$tmp GF@$des\n",
                        param2,
                        print2,
                        param1,
                        print1
                       );
    }

    if (strcmp(op->name, "+") == 0 || strcmp(op->name, "-") == 0 || strcmp(op->name, "*") == 0 || strcmp(op->name, "/") == 0)
    {
        if (variant == 0)
        {
            strncpy(des->name, "INT_ID", 8);
        }
        else {
            strncpy(des->name, "DBL_ID", 8);
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


token_t *int_dbl(token_t *op, token_t *par1, token_t *par2, symtable_t *symtab, bool switched, list_t *code_buffer, bool in_stat)
{
    if (operator(op->name, 0) == NULL)
    {
        return NULL;
    }
    static unsigned long long label_n = 0;
    char param1[10];
    char param2[10];
    token_info_t info = {.ptr = NULL};

    token_t *des = createToken("BOOL_ID", info);

    char *print1 = params(symtab, param1, par1);

    char *print2 = params(symtab, param2, par2);


    print_or_append(code_buffer, in_stat, "INT2FLOAT GF@$tmp %s@%s\n", param1, print1);

    if (switched)
    {
        if (strcmp(op->name, "/") == 0)
        {
            print_or_append(code_buffer, in_stat, "JUMPIFNEQ $%s$%llu$notZero %s@%s int@0\n"
                            "EXIT int@9\n"
                            "LABEL $%s$%llu$notZero\n", symtab->name, label_n, param1, print1, symtab->name, label_n);
        }
        print_or_append(code_buffer, in_stat, "%s GF@$des %s@%s GF@$tmp\n", operator(op->name, 0), param2, print2);

    }
    else
    {
        if (strcmp(op->name, "/") == 0)
        {
            print_or_append(code_buffer, in_stat, "JUMPIFNEQ $%s$%llu$notZero %s@%s float@0x0p+0\n"
                            "EXIT int@9\n"
                            "LABEL $%s$%llu$notZero\n", symtab->name, label_n, param2, print2, symtab->name, label_n);
        }
        print_or_append(code_buffer, in_stat, "%s GF@$des GF@$tmp %s@%s\n", operator(op->name, 0), param2, print2);
    }

    if (strcmp(op->name, "<=") == 0 || strcmp(op->name, ">=") == 0)
    {
        //udelat == do temp promenne, OR temp promenne a vysledku operace predtim

        print_or_append(code_buffer, in_stat, "EQ GF@$eq %s@%s GF@$tmp\n"
                        "OR GF@$des GF@$eq GF@$des\n", param2, print2);
    }
    if (strcmp(op->name, "+") == 0 || strcmp(op->name, "-") == 0 || strcmp(op->name, "*") == 0 ||
            strcmp(op->name, "/") == 0)
    {
        strncpy(des->name, "DBL_ID", 8);
    }


    label_n++;
    destroyToken(par1);
    destroyToken(par2);
    destroyToken(op);
    free(print1);
    free(print2);
    return des;

}



token_t *int_str(token_t *op, token_t *par1, token_t *par2, list_t *code_buffer, bool in_stat)
{
    if (operator(op->name, 0) == NULL)
    {
        return NULL;
    }
    static unsigned long long label_n = 0;


    token_info_t info = {.ptr = NULL};
    token_t *des = createToken("BOOL_ID", info);

    if (strcmp(op->name, "!=") == 0)
    {
        print_or_append(code_buffer, in_stat, "MOVE GF@$des bool@true\n");
    }
    else if (strcmp(op->name, "==") == 0)
    {
        print_or_append(code_buffer, in_stat, "MOVE GF@$des bool@false\n");
    }
    else
    {
        label_n++;
        destroyToken(par1);
        destroyToken(par2);
        destroyToken(op);
        free(des->name);
        free(des);
        token_info_t info1;
        token_t *error = createToken("ERR_SEM", info1);
        return error;
    }
    label_n++;
    destroyToken(par1);
    destroyToken(par2);
    destroyToken(op);
    return des;
}


token_t *dbl_str(token_t *op, token_t *par1, token_t *par2, list_t *code_buffer, bool in_stat)
{
    if (operator(op->name, 0) == NULL)
    {
        return NULL;
    }
    static unsigned long long label_n = 0;

    token_info_t info = {.ptr = NULL};
    token_t *des = createToken("BOOL_ID", info);

    if (strcmp(op->name, "!=") == 0)
    {
        print_or_append(code_buffer, in_stat, "MOVE GF@$des bool@true\n");
    }
    else if (strcmp(op->name, "==") == 0)
    {
        print_or_append(code_buffer, in_stat, "MOVE GF@$des bool@false\n");
    }
    else
    {
        label_n++;
        destroyToken(par1);
        destroyToken(par2);
        destroyToken(op);
        free(des->name);
        free(des);
        token_info_t info1 = {.ptr = NULL};
        token_t *error = createToken("ERR_SEM", info1);
        return error;
    }
    label_n++;
    destroyToken(par1);
    destroyToken(par2);
    destroyToken(op);
    return des;
}

token_t *int_id(token_t *op, token_t *par1, token_t *par2, symtable_t *symtab, bool switched, list_t *code_buffer, bool in_stat)
{
    if (operator(op->name, 0) == NULL)
    {
        return NULL;
    }
    static unsigned long long label_n = 0;

    char param1[10];
    char param2[10];
    token_info_t info = {.ptr = NULL};

    token_t *des = createToken("BOOL_ID", info);

    char *print1 = params(symtab, param1, par1);

    char *print2 = params(symtab, param2, par2);


    if (strcmp(op->name, "==") == 0 || strcmp(op->name, "!=") == 0)
    {
        print_or_append(code_buffer, in_stat, "TYPE GF@$type %s@%s\n"
                        "JUMPIFNEQ $%s$%llu$string GF@$type string@string\n", param2, print2, symtab->name, label_n);
        if (strcmp(op->name, "!=") == 0)
        {
            print_or_append(code_buffer, in_stat, "MOVE GF@$des bool@true\n");
        }
        else if (strcmp(op->name, "==") == 0)
        {
            print_or_append(code_buffer, in_stat, "MOVE GF@$des bool@false\n");
        }

        print_or_append(code_buffer, in_stat,
                        "JUMP END$%llu\n"
                        "LABEL $%s$%llu$string\n"
                        "MOVE GF@$des %s@%s\n"
                        "JUMPIFNEQ $%s$%llu$float GF@$type string@float\n"
                        "INT2FLOAT GF@$des %s@%s\n"
                        "LABEL $%s$%llu$float\n"
                        "%s GF@$des GF@$des %s@%s\n", label_n, symtab->name, label_n, param1, print1, symtab->name, label_n, param1, print1,
                        symtab->name, label_n, operator(op->name, 0), param2, print2);

        if (strcmp(op->name, "!=") == 0)
        {
            print_or_append(code_buffer, in_stat, "NOT GF@$des GF@$des\n");
        }
        print_or_append(code_buffer, in_stat, "LABEL END$%llu\n", label_n);
    }
    else
    {
        print_or_append(code_buffer, in_stat,
                        "TYPE GF@$type %s@%s\n"
                        "JUMPIFNEQ $%s$%llu$string GF@$type string@string\n"
                        "EXIT int@4\n"
                        "LABEL $%s$%llu$string\n"
                        "MOVE GF@$des %s@%s\n"
                        "JUMPIFNEQ $%s$%llu$float GF@$type string@float\n"
                        "INT2FLOAT GF@$des %s@%s\n"
                        "LABEL $%s$%llu$float\n",  param2, print2, symtab->name, label_n, symtab->name, label_n, param1,
                        print1, symtab->name, label_n, param1, print1, symtab->name, label_n);

        if (!switched)
        {
            if (strcmp(op->name, "/") == 0)
            {
                print_or_append(code_buffer, in_stat,
                                "TYPE GF@$tmp %s@%s\n"
                                "JUMPIFNEQ $%s$%llu$int1 GF@$tmp string@int\n"
                                "JUMPIFNEQ $%s$%llu$notZero1 %s@%s int@0\n"
                                "EXIT int@9\n"
                                "LABEL $%s$%llu$notZero1\n"
                                "JUMP $%s$%llu$notZero\n"
                                "LABEL $%s$%llu$int1\n", param2, print2, symtab->name, label_n, symtab->name, label_n, param2, print2,
                                symtab->name, label_n, symtab->name, label_n, symtab->name, label_n);

                print_or_append(code_buffer, in_stat, "JUMPIFNEQ $%s$%llu$notZero %s@%s float@0x0p+0\n"
                                "EXIT int@9\n"
                                "LABEL $%s$%llu$notZero\n", symtab->name, label_n, param2, print2, symtab->name, label_n);
            }

            print_or_append(code_buffer, in_stat,
                            "MOVE GF@$eq GF@$des\n"
                            "%s GF@$des GF@$des %s@%s\n", operator(op->name, 0), param2, print2);

            if (strcmp(op->name, "<=") == 0 || strcmp(op->name, ">=") == 0)
            {
                //udelat == do temp promenne, OR temp promenne a vysledku operace predtim

                print_or_append(code_buffer, in_stat, "EQ GF@$eq %s@%s GF@$eq\n"
                                "OR GF@$des GF@$eq GF@$des\n", param2, print2);
            }

            if (strcmp(op->name, "/") == 0)
            {
                print_or_append(code_buffer, in_stat,
                                "JUMPIFNEQ $%s$%llu$int GF@$type string@int\n"
                                "FLOAT2INT GF@$des GF@$des\n"
                                "LABEL $%s$%llu$int\n", symtab->name, label_n, symtab->name, label_n);
            }

        }
        else
        {
            if (strcmp(op->name, "/") == 0)
            {
                print_or_append(code_buffer, in_stat,
                                "TYPE GF@$tmp %s@%s\n"
                                "JUMPIFNEQ $%s$%llu$int1 GF@$tmp string@int\n"
                                "JUMPIFNEQ $%s$%llu$notZero1 GF@$des int@0\n"
                                "EXIT int@9\n"
                                "LABEL $%s$%llu$notZero1\n"
                                "JUMP $%s$%llu$notZero\n"
                                "LABEL $%s$%llu$int1\n", param2, print2, symtab->name, label_n, symtab->name, label_n,
                                symtab->name, label_n, symtab->name, label_n, symtab->name, label_n);

                print_or_append(code_buffer, in_stat, "JUMPIFNEQ $%s$%llu$notZero GF@$des float@0x0p+0\n"
                                "EXIT int@9\n"
                                "LABEL $%s$%llu$notZero\n", symtab->name, label_n, symtab->name, label_n);
            }

            print_or_append(code_buffer, in_stat,
                            "MOVE GF@$eq GF@$des\n"
                            "%s GF@$des %s@%s GF@$des\n", operator(op->name, 0), param2, print2);

            if (strcmp(op->name, "<=") == 0 || strcmp(op->name, ">=") == 0)
            {
                //udelat == do temp promenne, OR temp promenne a vysledku operace predtim

                print_or_append(code_buffer, in_stat, "EQ GF@$eq %s@%s GF@$eq\n"
                                "OR GF@$des GF@$eq GF@$des\n", param2, print2);
            }
            if (strcmp(op->name, "/") == 0)
            {
                print_or_append(code_buffer, in_stat,
                                "JUMPIFNEQ $%s$%llu$int GF@$type string@int\n"
                                "FLOAT2INT GF@$des GF@$des\n"
                                "LABEL $%s$%llu$int\n", symtab->name, label_n, symtab->name, label_n);
            }

        }


    }

    if (strcmp(op->name, "+") == 0 || strcmp(op->name, "-") == 0 || strcmp(op->name, "*") == 0 ||
            strcmp(op->name, "/") == 0)
    {
        strncpy(des->name, "ID", 8);
    }

    label_n++;
    destroyToken(par1);
    destroyToken(par2);
    destroyToken(op);
    free(print1);
    free(print2);
    return des;
}

token_t *dbl_id(token_t *op, token_t *par1, token_t *par2, symtable_t *symtab, bool switched, list_t *code_buffer, bool in_stat)
{
    if (operator(op->name, 0) == NULL)
    {
        return NULL;
    }
    static unsigned long long label_n = 0;

    char param1[10];
    char param2[10];

    token_info_t info = {.ptr = NULL};

    token_t *des = createToken("BOOL_ID", info);

    char *print1 = params(symtab, param1, par1);

    char *print2 = params(symtab, param2, par2);

    if (strcmp(op->name, "==") == 0 || strcmp(op->name, "!=") == 0)
    {
        print_or_append(code_buffer, in_stat, "TYPE GF@$type %s@%s\n"
                        "JUMPIFNEQ $%s$%llu$string GF@$type string@string\n", param2, print2, symtab->name, label_n);
        if (strcmp(op->name, "!=") == 0)
        {
            print_or_append(code_buffer, in_stat, "MOVE GF@$des bool@true\n");
        }
        else if (strcmp(op->name, "==") == 0)
        {
            print_or_append(code_buffer, in_stat, "MOVE GF@$des bool@false\n");
        }

        print_or_append(code_buffer, in_stat,
                        "JUMP END$%llu\n"
                        "LABEL $%s$%llu$string\n"
                        "MOVE GF@$des %s@%s\n"
                        "JUMPIFEQ $%s$%llu$float GF@$type string@float\n"
                        "INT2FLOAT GF@$des %s@%s\n"
                        "LABEL $%s$%llu$float\n"
                        "%s GF@$des GF@$des %s@%s\n", label_n, symtab->name, label_n, param2, print2, symtab->name, label_n, param2, print2,
                        symtab->name, label_n, operator(op->name, 0), param1, print1);

        if (strcmp(op->name, "!=") == 0)
        {
            print_or_append(code_buffer, in_stat, "NOT GF@$des GF@$des\n");
        }
        print_or_append(code_buffer, in_stat, "LABEL END$%llu\n", label_n);
    }
    else
    {
        print_or_append(code_buffer, in_stat,
                        "TYPE GF@$type %s@%s\n"
                        "JUMPIFNEQ $%s$%llu$string GF@$type string@string\n"
                        "EXIT int@4\n"
                        "LABEL $%s$%llu$string\n"
                        "MOVE GF@$des %s@%s\n"
                        "JUMPIFEQ $%s$%llu$float GF@$type string@float\n"
                        "INT2FLOAT GF@$des %s@%s\n"
                        "LABEL $%s$%llu$float\n",  param2, print2, symtab->name, label_n, symtab->name, label_n, param2,
                        print2, symtab->name, label_n, param2, print2, symtab->name, label_n);

        if (switched)
        {
            if (strcmp(op->name, "/") == 0)
            {
                print_or_append(code_buffer, in_stat, "JUMPIFNEQ $%s$%llu$notZero GF@$des float@0x0p+0\n"
                                "EXIT int@9\n"
                                "LABEL $%s$%llu$notZero\n", symtab->name, label_n, symtab->name, label_n);
            }

            print_or_append(code_buffer, in_stat,
                            "MOVE GF@$eq GF@$des\n"
                            "%s GF@$des GF@$des %s@%s\n", operator(op->name, 0), param1, print1);

            if (strcmp(op->name, "<=") == 0 || strcmp(op->name, ">=") == 0)
            {
                //udelat == do temp promenne, OR temp promenne a vysledku operace predtim

                print_or_append(code_buffer, in_stat, "EQ GF@$eq %s@%s GF@$eq\n"
                                "OR GF@$des GF@$eq GF@$des\n", param1, print1);
            }


        }
        else
        {
            if (strcmp(op->name, "/") == 0)
            {
                print_or_append(code_buffer, in_stat, "JUMPIFNEQ $%s$%llu$notZero GF@$des float@0x0p+0\n"
                                "EXIT int@9\n"
                                "LABEL $%s$%llu$notZero\n", symtab->name, label_n, symtab->name, label_n);
            }

            print_or_append(code_buffer, in_stat,
                            "MOVE GF@$eq GF@$des\n"
                            "%s GF@$des %s@%s GF@$des\n", operator(op->name, 0), param1, print1);

            if (strcmp(op->name, "<=") == 0 || strcmp(op->name, ">=") == 0)
            {
                //udelat == do temp promenne, OR temp promenne a vysledku operace predtim

                print_or_append(code_buffer, in_stat, "EQ GF@$eq %s@%s GF@$eq\n"
                                "OR GF@$des GF@$eq GF@$des\n", param1, print1);
            }
        }
    }


    if (strcmp(op->name, "+") == 0 || strcmp(op->name, "-") == 0 || strcmp(op->name, "*") == 0 ||
            strcmp(op->name, "/") == 0)
    {
        strncpy(des->name, "ID", 8);
    }


    label_n++;
    destroyToken(par1);
    destroyToken(par2);
    destroyToken(op);
    free(print1);
    free(print2);
    return des;
}

token_t *str_str(token_t *op, token_t *par1, token_t *par2, symtable_t *symtab, list_t *code_buffer, bool in_stat)
{
    if (operator(op->name, 0) == NULL)
    {
        return NULL;
    }
    static unsigned long long label_n = 0;

    char param1[10];
    char param2[10];
    token_info_t info = {.ptr = NULL};

    token_t *des = createToken("BOOL_ID", info);

    char *print1 = params(symtab, param1, par1);

    char *print2 = params(symtab, param2, par2);


    if (strcmp(op->name, "==") == 0)
    {
        print_or_append(code_buffer, in_stat, "%s GF@$des %s@%s %s@%s\n", operator(op->name, 0), param1, print1, param2, print2);
    }
    else if (strcmp(op->name, "!=") == 0)
    {
        print_or_append(code_buffer, in_stat, "%s GF@$des %s@%s %s@%s\n", operator(op->name, 0), param1, print1, param2, print2);

        print_or_append(code_buffer, in_stat, "NOT GF@$des GF@$des\n");
    }
    else if (strcmp(op->name, "+") == 0)
    {
        strncpy(des->name, "STR_ID", 8);
        print_or_append(code_buffer, in_stat, "%s GF@$des %s@%s %s@%s\n", operator(op->name, 1), param1, print1, param2, print2);
    }
    else if (strcmp(operator(op->name, 0), "LT") == 0 || strcmp(operator(op->name, 0), "GT") == 0)
    {
        print_or_append(code_buffer, in_stat, "%s GF@$des %s@%s %s@%s\n", operator(op->name, 0), param1, print1, param2, print2);
        if ((strcmp(op->name, ">=") == 0) || (strcmp(op->name, "<=") == 0))
        {
            print_or_append(code_buffer, in_stat, "EQ GF@$eq %s@%s %s@%s\n"
                            "OR GF@$des GF@$eq GF@$des\n", param1, print1, param2, print2);
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
        destroyToken(des);
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

token_t *str_id(token_t *op, token_t *par1, token_t *par2, symtable_t *symtab, bool switched, list_t *code_buffer, bool in_stat)
{
    static unsigned long long label_n = 0;
    if (operator(op->name, 0) == NULL)
    {
        return NULL;
    }
    if (strcmp(op->name, "/") == 0 || strcmp(op->name, "-") == 0 || strcmp(op->name, "*") == 0)
    {
        label_n++;
        destroyToken(par1);
        destroyToken(par2);
        destroyToken(op);
        /*token_info_t info1;
        token_t *error = createToken("ERR_SEM", info1);
        return error;*/
        print_or_append(code_buffer, in_stat, "EXIT int@4\n");
        token_info_t info = {.ptr = NULL};

        token_t *des = createToken("STR_ID", info);
        return des;
    }

    char param1[10];
    char param2[10];
    char frame[3];
    token_info_t info = {.ptr = NULL};

    token_t *des = createToken("BOOL_ID", info);

    char *print1 = params(symtab, param1, par1);

    char *print2 = params(symtab, param2, par2);

    which_frame(symtab, frame);

    if (strcmp(op->name, "==") == 0 || strcmp(op->name, "!=") == 0)
    {
        print_or_append(code_buffer, in_stat, "TYPE GF@$type %s@%s\n"
                        "JUMPIFEQ $%s$%llu$string GF@$type string@string\n", param2, print2, symtab->name, label_n);
        if (strcmp(op->name, "!=") == 0)
        {
            print_or_append(code_buffer, in_stat, "MOVE GF@$des bool@true\n");
        }
        else if (strcmp(op->name, "==") == 0)
        {
            print_or_append(code_buffer, in_stat, "MOVE GF@$des bool@false\n");
        }

        print_or_append(code_buffer, in_stat,
                        "JUMP END$%llu\n"
                        "LABEL $%s$%llu$string\n"
                        "MOVE GF@$des %s@%s\n"
                        "%s GF@$des GF@$des %s@%s\n", label_n, symtab->name, label_n, param1, print1, operator(op->name, 0), param2, print2);

        if (strcmp(op->name, "!=") == 0)
        {
            print_or_append(code_buffer, in_stat, "NOT GF@$des GF@$des\n");
        }
        print_or_append(code_buffer, in_stat, "LABEL END$%llu\n", label_n);
    }
//TADY JSEM SKONCIL
    else
    {
        print_or_append(code_buffer, in_stat,
                        "TYPE GF@$type %s@%s\n"
                        "JUMPIFEQ $%s$%llu$string GF@$type string@string\n"
                        "EXIT int@4\n"
                        "LABEL $%s$%llu$string\n"
                        "MOVE GF@$des %s@%s\n", param2, print2, symtab->name, label_n, symtab->name, label_n, param1, print1);
        if (switched)
        {
            print_or_append(code_buffer, in_stat, "%s GF@$des %s@%s GF@$des\n", operator(op->name, 1), param2, print2);
        }
        else
        {
            print_or_append(code_buffer, in_stat, "%s GF@$des GF@$des %s@%s\n", operator(op->name, 1), param2, print2);
        }

        if ((strcmp(op->name, ">=") == 0) || (strcmp(op->name, "<=") == 0))
        {
            print_or_append(code_buffer, in_stat, "EQ GF@$eq %s@%s %s@%s\n"
                            "OR GF@$des GF@$eq GF@$des\n", param1, print1, param2, print2);
        }

    }
    if (strcmp(op->name, "+"))
    {
        strncpy(des->name, "STR_ID", 8);
    }
    label_n++;
    destroyToken(par1);
    destroyToken(par2);
    destroyToken(op);
    free(print1);
    free(print2);
    return des;
}

token_t *id_id(token_t *op, token_t *par1, token_t *par2, symtable_t *symtab, bool switched, list_t *code_buffer, bool in_stat)
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
    token_info_t info = {.ptr = NULL};

    token_t *des = createToken("BOOL_ID", info);

    char *print1 = params(symtab, param1, par1);

    char *print2 = params(symtab, param2, par2);



    print_or_append(code_buffer, in_stat, "DEFVAR %s@%s\n", frame, name);

    print_or_append(code_buffer, in_stat, "DEFVAR %s@%s %llu$type0\n"
                    "TYPE %s@%s %llu$type0 %s@%s\n"
                    "JUMPIFEQ $%s$%llu$int0 %s@%s %llu$type0 string@int\n",
                    frame, print1, label_n, frame, print1, label_n, frame, print1,
                    symtab->name, label_n, frame, print1, label_n);

    print_or_append(code_buffer, in_stat, "JUMPIFEQ $%s$%llu$float0 %s@%s %llu$type0 string@float\n", symtab->name,
                    label_n, frame, print1, label_n );

    print_or_append(code_buffer, in_stat, "JUMPIFEQ $%s$%llu$string0 %s@%s %llu$type0 string@string\n", symtab->name,
                    label_n, frame, print1, label_n );

    print_or_append(code_buffer, in_stat, "LABEL $%s$%llu$int0\n", symtab->name,
                    label_n);

    if (strcmp(op->name, " == ") == 0 || strcmp(op->name, " != ") == 0)
    {
        print_or_append(code_buffer, in_stat, "DEFVAR %s@%s %llu$type1\n"
                        "DEFVAR %s@%s %llu$tmp1\n"
                        "DEFVAR %s@$tmp %llu$int1\n"
                        "TYPE %s@%s %llu$type1 %s@%s\n"
                        "MOVE %s@%s %llu$tmp1 %s@%s\n"
                        "MOVE %s@$tmp %llu$int1 %s@%s\n"
                        "JUMPIFEQ $%s$%llu$int1 %s@%s %llu$type1 string@int\n"
                        "JUMPIFEQ $%s$%llu$int1 %s@%s %llu$type1 string@string\n"
                        "INT2FLOAT %s@$tmp %llu$int1 %s@%s\n"
                        "LABEL $%s$%llu$int1\n"
                        "%s %s@%s %s@%s %llu$tmp1 %s@$tmp %llu$int1\n",
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

        if (strcmp(op->name, " != ") == 0)
        {
            print_or_append(code_buffer, in_stat, "DEFVAR %s@%s$not1\n"
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
        print_or_append(code_buffer, in_stat, "DEFVAR %s@%s %llu$type1\n"
                        "DEFVAR %s@%s %llu$tmp1\n"
                        "DEFVAR %s@$tmp %llu$int1\n"
                        "TYPE %s@%s %llu$type1 %s@%s\n"
                        "MOVE %s@%s %llu$tmp1 %s@%s\n"
                        "MOVE %s@$tmp %llu$int1 %s@%s\n"
                        "JUMPIFEQ $%s$%llu$int1 %s@%s %llu$type1 string@int\n"
                        "JUMPIFEQ $%s$%llu$float1 %s@%s %llu$type1 string@float\n"
                        "EXIT int@4\n"
                        "LABEL $%s$%llu$float1\n"
                        "INT2FLOAT %s@$tmp %llu$int1 %s@%s\n"
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
            print_or_append(code_buffer, in_stat, "%s %s@%s %s@%s %llu$tmp1 %s@$tmp %llu$int1\n", operator(op->name, 0),
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
            print_or_append(code_buffer, in_stat, "%s %s@%s %s@$tmp %llu$int1 %s@%s %llu$tmp1 \n", operator(op->name, 0),
                            frame,
                            des->info.ptr->var.key,
                            frame,
                            label_n,
                            frame,
                            print2,
                            label_n
                           );
        }
        if (strcmp(op->name, " <= ") == 0 || strcmp(op->name, " >= ") == 0)
        {
            print_or_append(code_buffer, in_stat, "DEFVAR %s@%s$eq1\n"
                            "DEFVAR %s@%s$or1\n"
                            "MOVE %s@%s$or1 %s@%s\n"
                            "EQ %s@%s$eq1 %s@%s %llu$tmp1 %s@$tmp %llu$int1\n"
                            "OR %s@%s %s@%s$eq1 %s@%s$ or \n",
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


    print_or_append(code_buffer, in_stat, "JUMP ENDTHISMADNESS %llu\n", label_n);

    print_or_append(code_buffer, in_stat, "LABEL $%s$%llu$float0\n", symtab->name,
                    label_n);


    if (strcmp(op->name, " == ") == 0 || strcmp(op->name, " != ") == 0)
    {
        print_or_append(code_buffer, in_stat, "DEFVAR %s@%s %llu$type2\n"
                        "DEFVAR %s@%s %llu$tmp2\n"
                        "TYPE %s@%s %llu$type2 %s@%s\n"
                        "MOVE %s@%s %llu$tmp2 %s@%s\n"
                        "JUMPIFEQ $%s$%llu$float2 %s@%s %llu$type2 string@float\n"
                        "JUMPIFEQ $%s$%llu$float2 %s@%s %llu$type2 string@string\n"
                        "INT2FLOAT %s@%s %llu$tmp2 %s@%s\n"
                        "LABEL $%s$%llu$float2\n"
                        "%s %s@%s %s@%s %llu$tmp2 %s@%s\n",
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
        if (strcmp(op->name, " != ") == 0)
        {
            print_or_append(code_buffer, in_stat, "DEFVAR %s@%s$not2\n"
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
        print_or_append(code_buffer, in_stat, "DEFVAR %s@%s %llu$type2\n"
                        "DEFVAR %s@%s %llu$tmp2\n"
                        "TYPE %s@%s %llu$type2 %s@%s\n"
                        "MOVE %s@%s %llu$tmp2 %s@%s\n"
                        "JUMPIFEQ $%s$%llu$int2 %s@%s %llu$type2 string@int\n"
                        "JUMPIFEQ $%s$%llu$float2 %s@%s %llu$type2 string@float\n"
                        "EXIT int@4\n"
                        "LABEL $%s$%llu$int2\n"
                        "INT2FLOAT %s@%s %llu$tmp2 %s@%s\n"
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
            print_or_append(code_buffer, in_stat, "%s %s@%s %s@%s %llu$tmp2 %s@%s\n", operator(op->name, 0),
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
            print_or_append(code_buffer, in_stat, "%s %s@%s %s@%s %s@%s %llu$tmp2 \n", operator(op->name, 0),
                            frame,
                            des->info.ptr->var.key,
                            frame,
                            print1,
                            frame,
                            print2,
                            label_n

                           );
        }
        if (strcmp(op->name, " <= ") == 0 || strcmp(op->name, " >= ") == 0)
        {
            print_or_append(code_buffer, in_stat, "DEFVAR %s@%s$eq2\n"
                            "DEFVAR %s@%s$or2\n"
                            "MOVE %s@%s$or2 %s@%s\n"
                            "EQ %s@%s$eq2 %s@%s %llu$tmp2 %s@%s\n"
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
    print_or_append(code_buffer, in_stat, "JUMP ENDTHISMADNESS %llu\n", label_n);

    print_or_append(code_buffer, in_stat, "LABEL $%s$%llu$string0\n", symtab->name,
                    label_n);


    if (strcmp(op->name, " == ") == 0 || strcmp(op->name, " != ") == 0)
    {
        print_or_append(code_buffer, in_stat, "%s %s@%s %s@%s %s@%s\n", operator(op->name, 0),
                        frame,
                        des->info.ptr->var.key,
                        frame,
                        print2,
                        frame,
                        print1//
                       );
        if (strcmp(op->name, " != ") == 0)
        {
            print_or_append(code_buffer, in_stat, "DEFVAR %s@%s$not3\n"
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

    else if (strcmp(op->name, " + ") == 0)
    {
        print_or_append(code_buffer, in_stat, "DEFVAR %s@%s %llu$type3\n"
                        "TYPE %s@%s %llu$type3 %s@%s\n"
                        "JUMPIFEQ $%s$%llu$string3 %s@%s %llu$type3 string@string\n"
                        "EXIT int@4\n"
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
            print_or_append(code_buffer, in_stat, "%s %s@%s %s@%s %s@%s\n", operator(op->name, 1),
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
            print_or_append(code_buffer, in_stat, "%s %s@%s %s@%s %s@%s\n", operator(op->name, 1),
                            frame,
                            des->info.ptr->var.key,
                            frame,
                            print1,//
                            frame,
                            print2//
                           );
        }
    }

    print_or_append(code_buffer, in_stat, "LABEL ENDTHISMADNESS %llu\n", label_n);

    if (strcmp(op->name, " + ") == 0 || strcmp(op->name, " - ") == 0 || strcmp(op->name, "*") == 0 ||
            strcmp(op->name, " / ") == 0)
    {
        strncpy(des->name, "ID", 8);
    }

    label_n++;
    destroyToken(par1);
    destroyToken(par2);
    destroyToken(op);
    free(print1);
    free(print2);
    return des;
}

token_t *gen_expr(token_t *op, token_t *param1, token_t *param2, symtable_t *symtab, list_t *code_buffer, bool in_stat)
{

    switch (type(param1, param2))
    {
        case INT_INT:
            return int_int(op, param1, param2, symtab, false, code_buffer, in_stat);
            break;
        case INT_DBL:
            return int_dbl(op, param1, param2, symtab, 0, code_buffer, in_stat);
            break;
        case INT_STR:
            return int_str(op, param1, param2, code_buffer, in_stat);
            break;
        case INT_ID:
            return int_id(op, param1, param2, symtab, 0, code_buffer, in_stat);
            break;
        case DBL_INT:
            return int_dbl(op, param2, param1, symtab, 1, code_buffer, in_stat);
            break;
        case DBL_DBL:
            return int_int(op, param1, param2, symtab, true, code_buffer, in_stat);
            break;
        case DBL_STR:
            return dbl_str(op, param1, param2, code_buffer, in_stat);
            break;
        case DBL_ID:
            return dbl_id(op, param1, param2, symtab, 0, code_buffer, in_stat);
            break;
        case STR_INT:
            return int_str(op, param2, param1, code_buffer, in_stat);
            break;
        case STR_DBL:
            return dbl_str(op, param2, param1, code_buffer, in_stat);
            break;
        case STR_STR:
            return str_str(op, param1, param2, symtab, code_buffer, in_stat);
            break;
        case STR_ID:
            return str_id(op, param1, param2, symtab, 0, code_buffer, in_stat);
            break;
        case ID_INT:
            return int_id(op, param2, param1, symtab, 1, code_buffer, in_stat);
            break;
        case ID_DBL:
            return dbl_id(op, param2, param1, symtab, 1, code_buffer, in_stat);
            break;
        case ID_STR:
            return str_id(op, param2, param1, symtab, 1, code_buffer, in_stat);
            break;
        case ID_ID:
            return id_id(op, param1, param2, symtab, 0, code_buffer, in_stat);
            break;
        /*case NULL_NULL:
        return null_null(param1, symtab);
        break;*/
        case ID_NIL:
            return nil(op, param2, param1, code_buffer, in_stat);
            break;
        case NIL_ID:
            return nil(op, param1, param2, code_buffer, in_stat);
            break;

        default: break;
    }

    token_info_t info1;
    token_t *error = createToken("ERR_INT", info1);
    return error;
}

#if 0


int main()
{
    printf("GF@$des\nGF@$eq\nGF@$jump\nGF@$tmp\nGF@$type\n");
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


    token_info_t info2;
    info2.string = "2";
    token_t *token1 = createToken("INT", info2);

    elem_t elem2;
    elem2.var.key = "jsem0STR_ID";
    token_info_t info5;
    info5.ptr = &elem2;
    token_t *token4 = createToken("STR_ID", info5);


    symtable_t *symtable = symtab_init("foo", VARIABLES);

    token_info_t tmp;
    tmp.ptr = NULL;

    token_t *op = createToken(" == ", tmp);
    token_info_t info3;
    info3.string = "1";
    token_info_t info4;
    info4.string = "1.1";
    token_t *token2 = createToken("INT", info3);
    token_t *token3 = createToken("DBL", info4);

    token_t *tmp12 = gen_expr(op,  token, token3, symtable, NULL, 0);

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

