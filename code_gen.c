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


char *params(symtable_t *symtab, char* param, token_t *par, bool id_variant)
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

        if (par->info.ptr == NULL)
        {
            print = malloc(sizeof(char) * 6);
            if (print == NULL)
            {
                return "ERR_INT";
            }
            if (id_variant)
            {
                strcpy(print, "$des1");
            }
            else
            {
                strcpy(print, "$des2");
            }

        }
        else
        {
            print = malloc(sizeof(char) * (strlen(par->info.ptr->var.key) + 1));
            if (print == NULL)
            {
                return "ERR_INT";
            }
            strcpy(print, par->info.ptr->var.key);
        }

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

    char *print1 = params(symtab, param1, par1, 0);

    char *print2 = params(symtab, param2, par2, 0);

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

            print_or_append(code_buffer, in_stat, "JUMPIFNEQ $%s$%llu$notZero0 %s@%s %s@0\n"
                            "EXIT int@9\n"
                            "LABEL $%s$%llu$notZero0\n", symtab->name, label_n, param2, print2, div_type, symtab->name, label_n);
        }
    }
    else
    {
        if (strcmp(op->name, "/") == 0)
        {
            print_or_append(code_buffer, in_stat, "JUMPIFNEQ $%s$%llu$notZero1 %s@%s %s@0x0p+0\n"
                            "EXIT int@9\n"
                            "LABEL $%s$%llu$notZero1\n", symtab->name, label_n, param2, print2, div_type, symtab->name, label_n);
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

    char *print1 = params(symtab, param1, par1, 0);

    char *print2 = params(symtab, param2, par2, 0);


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

    char *print1 = params(symtab, param1, par1, 1);
    if (strcmp(print1, "$des1") == 0)
    {
        print_or_append(code_buffer, in_stat, "MOVE GF@$des1 GF@$des\n");
    }
    char *print2 = params(symtab, param2, par2, 0);
    if (strcmp(print1, "$des2") == 0)
    {
        print_or_append(code_buffer, in_stat, "MOVE GF@$des2 GF@$des\n");
    }

    if (strcmp(op->name, "==") == 0 || strcmp(op->name, "!=") == 0)
    {
        print_or_append(code_buffer, in_stat, "TYPE GF@$type %s@%s\n"
                        "JUMPIFNEQ $%s$%llu$int$string GF@$type string@string\n", param2, print2, symtab->name, label_n);
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
                        "LABEL $%s$%llu$int$string\n"
                        "MOVE GF@$des %s@%s\n"
                        "JUMPIFNEQ $%s$%llu$int$float GF@$type string@float\n"
                        "INT2FLOAT GF@$des %s@%s\n"
                        "LABEL $%s$%llu$int$float\n"
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
                        "JUMPIFNEQ $%s$%llu$int$string GF@$type string@string\n"
                        "EXIT int@4\n"
                        "LABEL $%s$%llu$int$string\n"
                        "MOVE GF@$des %s@%s\n"
                        "JUMPIFNEQ $%s$%llu$int$float GF@$type string@float\n"
                        "INT2FLOAT GF@$des %s@%s\n"
                        "LABEL $%s$%llu$int$float\n",  param2, print2, symtab->name, label_n, symtab->name, label_n, param1,
                        print1, symtab->name, label_n, param1, print1, symtab->name, label_n);

        if (!switched)
        {
            if (strcmp(op->name, "/") == 0)
            {
                print_or_append(code_buffer, in_stat,
                                "TYPE GF@$tmp %s@%s\n"
                                "JUMPIFNEQ $%s$%llu$int$int1 GF@$tmp string@int\n"
                                "JUMPIFNEQ $%s$%llu$int$notZero1 %s@%s int@0\n"
                                "EXIT int@9\n"
                                "LABEL $%s$%llu$int$notZero1\n"
                                "JUMP $%s$%llu$int$notZero\n"
                                "LABEL $%s$%llu$int$int1\n", param2, print2, symtab->name, label_n, symtab->name, label_n, param2, print2,
                                symtab->name, label_n, symtab->name, label_n, symtab->name, label_n);

                print_or_append(code_buffer, in_stat, "JUMPIFNEQ $%s$%llu$int$notZero %s@%s float@0x0p+0\n"
                                "EXIT int@9\n"
                                "LABEL $%s$%llu$int$notZero\n", symtab->name, label_n, param2, print2, symtab->name, label_n);
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
                                "JUMPIFNEQ $%s$%llu$int$int1 GF@$tmp string@int\n"
                                "JUMPIFNEQ $%s$%llu$int$notZero1 GF@$des int@0\n"
                                "EXIT int@9\n"
                                "LABEL $%s$%llu$int$notZero1\n"
                                "JUMP $%s$%llu$int$notZero\n"
                                "LABEL $%s$%llu$int$int1\n", param2, print2, symtab->name, label_n, symtab->name, label_n,
                                symtab->name, label_n, symtab->name, label_n, symtab->name, label_n);

                print_or_append(code_buffer, in_stat, "JUMPIFNEQ $%s$%llu$int$notZero GF@$des float@0x0p+0\n"
                                "EXIT int@9\n"
                                "LABEL $%s$%llu$int$notZero\n", symtab->name, label_n, symtab->name, label_n);
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

    char *print1 = params(symtab, param1, par1, 1);
    if (strcmp(print1, "$des1") == 0)
    {
        print_or_append(code_buffer, in_stat, "MOVE GF@$des1 GF@$des\n");
    }
    char *print2 = params(symtab, param2, par2, 0);
    if (strcmp(print1, "$des2") == 0)
    {
        print_or_append(code_buffer, in_stat, "MOVE GF@$des2 GF@$des\n");
    }

    if (strcmp(op->name, "==") == 0 || strcmp(op->name, "!=") == 0)
    {
        print_or_append(code_buffer, in_stat, "TYPE GF@$type %s@%s\n"
                        "JUMPIFNEQ $%s$%llu$float$string GF@$type string@string\n", param2, print2, symtab->name, label_n);
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
                        "LABEL $%s$%llu$float$string\n"
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
                        "JUMPIFNEQ $%s$%llu$float$string GF@$type string@string\n"
                        "EXIT int@4\n"
                        "LABEL $%s$%llu$float$string\n"
                        "MOVE GF@$des %s@%s\n"
                        "JUMPIFEQ $%s$%llu$float GF@$type string@float\n"
                        "INT2FLOAT GF@$des %s@%s\n"
                        "LABEL $%s$%llu$float\n",  param2, print2, symtab->name, label_n, symtab->name, label_n, param2,
                        print2, symtab->name, label_n, param2, print2, symtab->name, label_n);

        if (switched)
        {
            if (strcmp(op->name, "/") == 0)
            {
                print_or_append(code_buffer, in_stat, "JUMPIFNEQ $%s$%llu$float$notZero GF@$des float@0x0p+0\n"
                                "EXIT int@9\n"
                                "LABEL $%s$%llu$float$notZero\n", symtab->name, label_n, symtab->name, label_n);
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
                print_or_append(code_buffer, in_stat, "JUMPIFNEQ $%s$%llu$float$notZero GF@$des float@0x0p+0\n"
                                "EXIT int@9\n"
                                "LABEL $%s$%llu$float$notZero\n", symtab->name, label_n, symtab->name, label_n);
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

    char *print1 = params(symtab, param1, par1, 0);

    char *print2 = params(symtab, param2, par2, 0);


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

    char *print1 = params(symtab, param1, par1, 1);
    if (strcmp(print1, "$des1") == 0)
    {
        print_or_append(code_buffer, in_stat, "MOVE GF@$des1 GF@$des\n");
    }
    char *print2 = params(symtab, param2, par2, 0);
    if (strcmp(print1, "$des2") == 0)
    {
        print_or_append(code_buffer, in_stat, "MOVE GF@$des2 GF@$des\n");
    }

    which_frame(symtab, frame);

    if (strcmp(op->name, "==") == 0 || strcmp(op->name, "!=") == 0)
    {
        print_or_append(code_buffer, in_stat, "TYPE GF@$type %s@%s\n"
                        "JUMPIFEQ $%s$%llu$string$string GF@$type string@string\n", param2, print2, symtab->name, label_n);
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
                        "LABEL $%s$%llu$string$string\n"
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
                        "JUMPIFEQ $%s$%llu$string$string GF@$type string@string\n"
                        "EXIT int@4\n"
                        "LABEL $%s$%llu$string$string\n"
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
    label_n++;
    label_n++;
    label_n++;
    char param1[10];
    char param2[10];

    token_info_t info = {.ptr = NULL};

    token_t *des = createToken("BOOL_ID", info);

    char *print1 = params(symtab, param1, par1, 1);
    if (strcmp(print1, "$des1") == 0)
    {
        print_or_append(code_buffer, in_stat, "MOVE GF@$des1 GF@$des\n");
    }
    char *print2 = params(symtab, param2, par2, 0);
    if (strcmp(print1, "$des2") == 0)
    {
        print_or_append(code_buffer, in_stat, "MOVE GF@$des2 GF@$des\n");
    }



    print_or_append(code_buffer, in_stat,
                    "TYPE GF@$type %s@%s\n"
                    "JUMPIFEQ $%s$%llu$int0 GF@$type string@int\n",
                    param1, print1, symtab->name, label_n);

    print_or_append(code_buffer, in_stat, "JUMPIFEQ $%s$%llu$float0 GF@$type string@float\n", symtab->name, label_n);

    print_or_append(code_buffer, in_stat, "JUMPIFEQ $%s$%llu$string0 GF@$type string@string\n", symtab->name, label_n );

    print_or_append(code_buffer, in_stat, "LABEL $%s$%llu$int0\n", symtab->name, label_n);


    if (strcmp(op->name, "==") == 0 || strcmp(op->name, "!=") == 0)
    {
        print_or_append(code_buffer, in_stat, "TYPE GF@$type %s@%s\n"
                        "JUMPIFNEQ $%s$%llu$int$string GF@$type string@string\n", param2, print2, symtab->name, label_n);
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
                        "LABEL $%s$%llu$int$string\n"
                        "MOVE GF@$des %s@%s\n"
                        "JUMPIFNEQ $%s$%llu$int$float GF@$type string@float\n"
                        "INT2FLOAT GF@$des %s@%s\n"
                        "LABEL $%s$%llu$int$float\n"
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
                        "JUMPIFNEQ $%s$%llu$int$string GF@$type string@string\n"
                        "EXIT int@4\n"
                        "LABEL $%s$%llu$int$string\n"
                        "MOVE GF@$des %s@%s\n"
                        "JUMPIFNEQ $%s$%llu$int$float GF@$type string@float\n"
                        "INT2FLOAT GF@$des %s@%s\n"
                        "LABEL $%s$%llu$int$float\n",  param2, print2, symtab->name, label_n, symtab->name, label_n, param1,
                        print1, symtab->name, label_n, param1, print1, symtab->name, label_n);

        if (!switched)
        {
            if (strcmp(op->name, "/") == 0)
            {
                print_or_append(code_buffer, in_stat,
                                "TYPE GF@$tmp %s@%s\n"
                                "JUMPIFNEQ $%s$%llu$int$int1 GF@$tmp string@int\n"
                                "JUMPIFNEQ $%s$%llu$int$notZero1 %s@%s int@0\n"
                                "EXIT int@9\n"
                                "LABEL $%s$%llu$int$notZero1\n"
                                "JUMP $%s$%llu$int$notZero\n"
                                "LABEL $%s$%llu$int$int1\n", param2, print2, symtab->name, label_n, symtab->name, label_n, param2, print2,
                                symtab->name, label_n, symtab->name, label_n, symtab->name, label_n);

                print_or_append(code_buffer, in_stat, "JUMPIFNEQ $%s$%llu$int$notZero %s@%s float@0x0p+0\n"
                                "EXIT int@9\n"
                                "LABEL $%s$%llu$int$notZero\n", symtab->name, label_n, param2, print2, symtab->name, label_n);
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
                                "JUMPIFNEQ $%s$%llu$int$int1 GF@$tmp string@int\n"
                                "JUMPIFNEQ $%s$%llu$int$notZero1 GF@$des int@0\n"
                                "EXIT int@9\n"
                                "LABEL $%s$%llu$int$notZero1\n"
                                "JUMP $%s$%llu$int$notZero\n"
                                "LABEL $%s$%llu$int$int1\n", param2, print2, symtab->name, label_n, symtab->name, label_n,
                                symtab->name, label_n, symtab->name, label_n, symtab->name, label_n);

                print_or_append(code_buffer, in_stat, "JUMPIFNEQ $%s$%llu$int$notZero GF@$des float@0x0p+0\n"
                                "EXIT int@9\n"
                                "LABEL $%s$%llu$int$notZero\n", symtab->name, label_n, symtab->name, label_n);
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

    print_or_append(code_buffer, in_stat, "JUMP ENDTHISMADNESS%llu\n", label_n);

    print_or_append(code_buffer, in_stat, "LABEL $%s$%llu$float0\n", symtab->name,
                    label_n);


    if (strcmp(op->name, "==") == 0 || strcmp(op->name, "!=") == 0)
    {
        print_or_append(code_buffer, in_stat, "TYPE GF@$type %s@%s\n"
                        "JUMPIFNEQ $%s$%llu$float$string GF@$type string@string\n", param2, print2, symtab->name, label_n);
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
                        "LABEL $%s$%llu$float$string\n"
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
                        "JUMPIFNEQ $%s$%llu$float$string GF@$type string@string\n"
                        "EXIT int@4\n"
                        "LABEL $%s$%llu$float$string\n"
                        "MOVE GF@$des %s@%s\n"
                        "JUMPIFEQ $%s$%llu$float GF@$type string@float\n"
                        "INT2FLOAT GF@$des %s@%s\n"
                        "LABEL $%s$%llu$float\n",  param2, print2, symtab->name, label_n, symtab->name, label_n, param2,
                        print2, symtab->name, label_n, param2, print2, symtab->name, label_n);

        if (switched)
        {
            if (strcmp(op->name, "/") == 0)
            {
                print_or_append(code_buffer, in_stat, "JUMPIFNEQ $%s$%llu$float$notZero GF@$des float@0x0p+0\n"
                                "EXIT int@9\n"
                                "LABEL $%s$%llu$float$notZero\n", symtab->name, label_n, symtab->name, label_n);
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
                print_or_append(code_buffer, in_stat, "JUMPIFNEQ $%s$%llu$float$notZero GF@$des float@0x0p+0\n"
                                "EXIT int@9\n"
                                "LABEL $%s$%llu$float$notZero\n", symtab->name, label_n, symtab->name, label_n);
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

    print_or_append(code_buffer, in_stat, "JUMP ENDTHISMADNESS%llu\n", label_n);

    print_or_append(code_buffer, in_stat, "LABEL $%s$%llu$string0\n", symtab->name,
                    label_n);


    if (strcmp(op->name, "==") == 0 || strcmp(op->name, "!=") == 0)
    {
        print_or_append(code_buffer, in_stat, "TYPE GF@$type %s@%s\n"
                        "JUMPIFEQ $%s$%llu$string$string GF@$type string@string\n", param2, print2, symtab->name, label_n);
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
                        "LABEL $%s$%llu$string$string\n"
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
                        "JUMPIFEQ $%s$%llu$string$string GF@$type string@string\n"
                        "EXIT int@4\n"
                        "LABEL $%s$%llu$string$string\n"
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
    print_or_append(code_buffer, in_stat, "LABEL ENDTHISMADNESS%llu\n", label_n);

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


token_t *length(list_t *code_buffer, bool in_stat, symtable_t *symtab, token_t *par)
{
	if (strcmp(par->name, "nil") == 0)
	{
		token_info_t info1;
		token_t *err = createToken("ERR_SEM", info1);
		return err;
	}

	static unsigned long long label_n = 0;
	char name[20];
	token_info_t info;
	sprintf(name, "LEN%lluSTR", label_n);
	info.ptr = symtab_elem_add(symtab, name);
	token_t *des = createToken("INT_ID", info);

	char frame_act [3] = "LF";
	if (strcmp(symtab->name, "$GT" ) == 0)
		strcpy(frame_act, "GF");

	if (! print_or_append(code_buffer, in_stat,"MOVE GF@$des int@0\n"))
		goto err_internal;

	if (strcmp(par->name, "ID") == 0)
	{
		if (symtab_find(symtab, par->info.ptr->var.key) == NULL)	//ID has not been declareted
			goto err_sem; 	// undefined ID

		if (! print_or_append(code_buffer, in_stat,
				"MOVE GF@$tmp %s@%s\n"
				"TYPE GF@$type %s@%s\n"
				"JUMPIFEQ $%s$%llu$string GF@$type string@string\n"
				"EXIT int@4\n"
				"LABEL $%s$%llu$string\n",
				frame_act, par->info.ptr->var.key,
				frame_act, par->info.ptr->var.key,
				label_n,
				label_n))
			goto err_internal;
	}
	else if (strcmp(par->name, "STR") == 0)
	{
		if (! print_or_append(code_buffer, in_stat, "MOVE GF@$tmp string@%s\n ", par->info.string))
			goto err_internal;
	}
	else 
		goto err_sem_type;

	if (! print_or_append(code_buffer, in_stat,"STRLEN GF@$des GF@$tmp\n"))
		goto err_internal;

	label_n++;
	destroyToken(par);
	return des;

err_internal:
	label_n++;
	destroyToken(par);
	destroyToken(des);
	info.ptr = NULL;
	des = createToken("ERR_INTERNAL", info);
	return des;

err_sem:
	label_n++;
	destroyToken(par);
	destroyToken(des);
	info.ptr = NULL;
	des = createToken("ERR_SEM_UNDEF", info);
	return des;

err_sem_type:
	label_n++;
	destroyToken(par);
	destroyToken(des);
	info.ptr = NULL;
	des = createToken("ERR_SEM_TYPE", info);
	return des;
}

//RETURN ascii char of value par<0,255> "chr(42)"
token_t *chr(list_t *code_buffer, bool in_stat, symtable_t *symtab, token_t *par)
{
	static unsigned long long label_n = 0;
	char name[24];
	sprintf(name, "CHR%llu", label_n);
	token_info_t info;
	info.ptr = symtab_elem_add(symtab, name);
	token_t *des = createToken("STR_ID", info);

	char frame[3] = "LF";

	if (strcmp(symtab->name, "$GT" ) == 0)
		strcpy(frame, "GF");

	if (! print_or_append(code_buffer, in_stat, "MOVE GF@$des string@\n"))
		goto err_internal;

	if (strcmp(par->name, "ID") == 0)
	{
		if (symtab_find(symtab, par->info.ptr->var.key) == NULL)	//ID has not been declareted
			goto err_sem; 	// undefined ID
			
		if (! print_or_append(code_buffer, in_stat, 
				"MOVE GF@$tmp %s@%s\n"
				"TYPE GF@$type %s@%s\n"
				"JUMPIFEQ $chr$tmp%llu$int$true GF@$type string@int\n"
				"EXIT int@4\n"
				"LABEL $chr$tmp%llu$int$true\n",
				frame, par->info.ptr->var.key,
				frame, par->info.ptr->var.key,
				label_n,
				label_n))
			goto err_internal;
	}
	else if (strcmp(par->name, "INT") == 0)
	{
		if (! print_or_append(code_buffer, in_stat,
				"MOVE GF@$tmp int@%s\n",
		      	par->info.string))
			goto err_internal;
	}
	else
		goto err_sem_type;

	if (! print_or_append(code_buffer, in_stat, "INT2CHAR GF@$des GF@$tmp\n"))        //INT2CHAR takes value <0,255>
		goto err_internal;

	destroyToken(par);
	label_n++;
	return des;

err_internal:
	label_n++;
	destroyToken(par);
	destroyToken(des);
	info.ptr = NULL;
	des = createToken("ERR_INTERNAL", info);
	return des;

err_sem:
	label_n++;
	destroyToken(par);
	destroyToken(des);
	info.ptr = NULL;
	des = createToken("ERR_SEM_UNDEF", info);
	return des;

err_sem_type:
	label_n++;
	destroyToken(par);
	destroyToken(des);
	info.ptr = NULL;
	des = createToken("ERR_SEM_TYPE", info);
	return des;
}

token_t *ord(list_t *code_buffer, bool in_stat, symtable_t *symtab, token_t *par1, token_t *par2)      //par1 == string, par2 == position(INT)
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

	if (! print_or_append(code_buffer, in_stat, "MOVE GF@$des int@0\n"))
		goto err_internal;

	/*
	GF@$tmp = par1
	GF@$eq = par2
	GF@$des = return
	*/
//////////////////////**FIRST PARAMETR**////////////////////////////////
	if (strcmp(par1->name, "ID") == 0)
	{
		if (symtab_find(symtab, par1->info.ptr->var.key) == NULL)
			goto err_sem; // undefined ID

		if (! print_or_append(code_buffer, in_stat, 
				"MOVE GF@$tmp %s@%s\n"
				"TYPE GF@$type %s@%s\n"
				"JUMPIFEQ $ord$string%llu$string$true GF@$type string@string\n"
				"EXIT int@4\n"
				"LABEL $ord$string%llu$string$true\n",
				frame, par1->info.ptr->var.key,
				frame, par1->info.ptr->var.key,
				label_n,
				label_n))
			goto err_internal;
	}
	else if (strcmp(par1->name, "STR") == 0)
	{
		if (! print_or_append(code_buffer, in_stat, "MOVE GF@$tmp string@%s\n", par1->info.string))
			goto err_internal;
	}
	else
		goto err_sem_type;
//////////////////////**SECOND PARAMETR**////////////////////////////////
	if (strcmp(par2->name, "ID") == 0)
	{
		if (symtab_find(symtab, par2->info.ptr->var.key) == NULL)
			goto err_sem;	//undefined ID

		if (! print_or_append(code_buffer, in_stat,
				"MOVE GF@$eq %s@%s\n"
				"TYPE GF@$type %s@%s\n"
				"JUMPIFEQ $ord$position%llu$int$true GF@$type string@int\n"
				"EXIT int@4\n"
				"LABEL $ord$position%llu$int$true\n",
				frame, par2->info.ptr->var.key,
				frame, par2->info.ptr->var.key,
				label_n,
				label_n))
			goto err_internal;
	}
	else if (strcmp(par2->name, "INT") == 0)
	{
		if (! print_or_append(code_buffer, in_stat, 
				"MOVE GF@$eq int@%s\n", par2->info.string))
			goto err_internal;
	}
	else
		goto err_sem_type;

	if (! print_or_append(code_buffer, in_stat,
			"STRLEN GF@$des GF@$tmp\n"
			"LT GF@$type GF@$eq GF@$des\n"      //position < strlen(string)
			"JUMPIFEQ $ord%llu GF@$type bool@true\n"
			"MOVE GF@$des nil@nil\n"
			"JUMP $ord$end%llu\n"
			"LABEL $ord%llu\n"
			"STRI2INT GF@$des GF@$tmp GF@$eq\n"
			"LABEL $ord$end%llu\n",
			label_n,
			label_n,
			label_n,
			label_n))
		goto err_internal;

	destroyToken(par1);
	destroyToken(par2);
	label_n++;
	return des;

err_internal:
	label_n++;
	destroyToken(par1);
	destroyToken(par2);
	destroyToken(des);
	info.ptr = NULL;
	des = createToken("ERR_INTERNAL", info);
	return des;

err_sem:
	label_n++;
	destroyToken(par1);
	destroyToken(par2);
	destroyToken(des);
	info.ptr = NULL;
	des = createToken("ERR_SEM", info);
	return des;

err_sem_type:
	label_n++;
	destroyToken(par1);
	destroyToken(par2);
	destroyToken(des);
	info.ptr = NULL;
	des = createToken("ERR_SEM_TYPE", info);
	return des;
}

token_t *substr(list_t *code_buffer, bool in_stat, symtable_t *symtab, token_t *string, token_t *begin, token_t *end)
{
	static unsigned long long label_n = 0;
	char name[24];
	sprintf(name, "SUBS%llu", label_n);
	token_info_t info;
	info.ptr = symtab_elem_add(symtab, name);
	token_t *des = createToken("STR_ID", info);

	char frame[3] = "LF";

	if (strcmp(symtab->name, "$GT" ) == 0)
		strcpy(frame, "GF");

	if (! print_or_append(code_buffer, in_stat, 
			"MOVE GF@$des string@\n"))
		goto err_internal;

	/*
	GF@$jump = string
	GF@$eq = begin
	GF@$tmp = end
	*/

	//////////////////////**FIRST PARAMETR**////////////////////////////////
	if (strcmp(string->name, "ID") == 0)
	{
		if (symtab_find(symtab, string->info.ptr->var.key) == NULL)
			goto err_sem;		//undefined ID

		if (! print_or_append(code_buffer, in_stat, 
				"MOVE GF@$jump %s@%s\n"
				"TYPE GF@$type %s@%s\n"
				"JUMPIFEQ $substr$string%llu$string$true GF@$type string@string\n"
				"EXIT int@4\n"
				"LABEL $substr$string%llu$string$true\n",
				frame, string->info.ptr->var.key,
				frame, string->info.ptr->var.key,
				label_n,
				label_n))
			goto err_internal;
	}
	else if (strcmp(string->name, "STR") == 0)
	{
		if (! print_or_append(code_buffer, in_stat, "MOVE GF@$jump string@%s\n",
		       string->info.string))
			goto err_internal;
	}
	else
		goto err_sem_type;

	//////////////////////**SECOND PARAMETR**////////////////////////////////
	if (strcmp(begin->name, "ID") == 0)
	{
		if (symtab_find(symtab, begin->info.ptr->var.key) == NULL)
			goto err_sem; 		//Undefined ID

		if (! print_or_append(code_buffer, in_stat, 
				"MOVE GF@$eq %s@%s\n"
				"TYPE GF@$type %s@%s\n"
				"JUMPIFEQ $substr$begin%llu$int$true GF@$type string@int\n"
				"EXIT int@4\n"
				"LABEL $substr$begin%llu$int$true\n",
				frame, begin->info.ptr->var.key,
				frame, begin->info.ptr->var.key,
				label_n,
				label_n))
			goto err_internal;
	}
	else if (strcmp(begin->name, "INT") == 0)
	{
		printf("MOVE GF@$eq int@%s\n",
		       begin->info.string);
	}
	else
		goto err_sem_type;

	//////////////////////**THIRD PARAMETR**////////////////////////////////
	if (strcmp(end->name, "ID") == 0)
	{
		if (symtab_find(symtab, end->info.ptr->var.key) == NULL)
			goto err_sem; // Undefined ID

		if (! print_or_append(code_buffer, in_stat, 
				"MOVE GF@$tmp %s@%s\n"
				"TYPE GF@$type %s@%s\n"
				"JUMPIFEQ $substr$end%llu$int$true GF@$type string@int\n"
				"EXIT int@4\n"
				"LABEL $substr$end%llu$int$true\n",
				frame, end->info.ptr->var.key,
				frame, end->info.ptr->var.key,
				label_n,
				label_n))
			goto err_internal;
	}
	else if (strcmp(end->name, "INT") == 0)
	{
		if (! print_or_append(code_buffer, in_stat, 
				"MOVE GF@$tmp int@%s\n",
		       	end->info.string))
			goto err_internal;
	}
	else
		goto err_sem_type;
/////////////////////////////**FINAL**////////////////////////////////////////

	/*
	GF@$jump = string
	GF@$eq = begin
	GF@$tmp = end
	*/
	if (! print_or_append(code_buffer, in_stat,
			"STRLEN GF@$des GF@$jump\n"							//GF@$des = len(Str)
			"LT GF@$type GF@$eq int@0\n"                     	//I < 0
			"JUMPIFEQ $substr$nil%llu GF@$type bool@true\n"
			"GT GF@$type GF@$eq GF@$des\n"     				//I > lens(STR)
			"JUMPIFEQ $substr$nil%llu GF@$type bool@true\n"
			"EQ GF@$type GF@$eq GF@$des\n" 				    //I = lens(STR)
			"JUMPIFEQ $substr$nil%llu GF@$type bool@true\n"

			"LT GF@$type GF@$tmp int@0\n"                       //N < 0
			"JUMPIFEQ $substr$nil%llu GF@$type bool@true\n"
			"ADD GF@$tmp GF@$eq GF@$tmp\n"                	//GF@$tmp = begin + end == the last index
			"LT GF@$type GF@$des GF@$tmp\n"      			// LEN(STR) < IDX-max ==> do till end of STR
			"JUMPIFEQ $substr$whole%llu GF@$type bool@true\n"

			
/////////////////////////**ONLY REQUIRED SIZE**////////////////////////////////
			"MOVE GF@$des string@\n"									//GF@$des = retval
			"LABEL $substr$reqsize%llu\n"
			"LT GF@$type GF@$eq GF@$tmp\n"       						//BEGIN < IDX-max ==> again
			"JUMPIFEQ $substr$nextchar%llu GF@$type bool@true\n"
			//    "EQ %s@$substr$cmp%llu %s@$substr$begin%llu %s@$substr$end%llu\n"    //BEGIN == IDX-max ==> once more
			//   "JUMPIFEQ $substr$nextchar%llu %s@$substr$cmp%llu bool@true\n"
			"JUMP $substr$end%llu\n"

			"LABEL $substr$nextchar%llu\n"
			"GETCHAR GF@$type GF@$jump GF@$eq\n"
			"CONCAT GF@$des GF@$des GF@$type\n"
			"ADD GF@$eq GF@$eq int@1\n"
			"JUMP $substr$reqsize%llu\n"

/////////////////////////**TILL THE END OF STRING**////////////////////////////////
			"LABEL $substr$whole%llu\n"
			"MOVE GF@$tmp GF@$des\n"				//GF@$tmp = len(str)
			"MOVE GF@$des string@\n"									//GF@$des = retval
			"LABEL $substr$wholestr%llu\n"
			"LT GF@$type GF@$eq GF@$tmp\n"    //BEGIN < LEN(STR) ==> again
			"JUMPIFEQ $substr$nextcharwhole%llu GF@$type bool@true\n"
			//    "EQ %s@$substr$cmp%llu %s@$substr$begin%llu %s@$substr$lenstr%llu\n"    //BEGIN == LEN(STR) ==> once more
			//    "JUMPIFEQ $substr$nextcharwhole%llu %s@$substr$cmp%llu bool@true\n"
			"JUMP $substr$end%llu\n"

			"LABEL $substr$nextcharwhole%llu\n"
			"GETCHAR GF@$type GF@$jump GF@$eq\n"
			"CONCAT GF@$des GF@$des GF@$type\n"
			"ADD GF@$eq GF@$eq int@1\n"
			"JUMP $substr$wholestr%llu\n"

			"LABEL $substr$nil%llu\n"
			"MOVE GF@$des nil@nil\n"
			"LABEL $substr$end%llu\n",

			label_n,label_n,label_n,label_n,label_n,
			label_n,label_n,label_n,label_n,label_n,
			label_n,label_n,label_n,label_n,label_n, label_n, label_n, label_n))
		goto err_internal;

	destroyToken(string);
	destroyToken(begin);
	destroyToken(end);
	label_n++;
	return des;

err_internal:
	label_n++;
	destroyToken(string);
	destroyToken(begin);
	destroyToken(end);
	destroyToken(des);
	info.ptr = NULL;
	des = createToken("ERR_INTERNAL", info);
	return des;

err_sem:
	label_n++;
	destroyToken(string);
	destroyToken(begin);
	destroyToken(end);
	destroyToken(des);
	info.ptr = NULL;
	des = createToken("ERR_SEM", info);
	return des;

err_sem_type:
	label_n++;
	destroyToken(string);
	destroyToken(begin);
	destroyToken(end);
	destroyToken(des);
	info.ptr = NULL;
	des = createToken("ERR_SEM_TYPE", info);
	return des;
}

token_t *input(list_t *code_buffer, bool in_stat, symtable_t *symtab, int type)
{
	static unsigned long long label_n = 0;
	char name[20];
	token_info_t info;
	token_t *des;
	char param[7];
	char frame[3] = "LF";
	if (strcmp(symtab->name, "$GT" ) == 0)
		strcpy(frame, "GF");

	if (type == 0)
	{
		sprintf(name, "IN%lluINT", label_n);
		info.ptr = symtab_elem_add(symtab, name);
		des = createToken("INT_ID", info);
		strcpy(param, "int");

	}
	else if (type == 1)
	{
		sprintf(name, "IN%lluDBL", label_n);
		info.ptr = symtab_elem_add(symtab, name);
		des = createToken("DBL_ID", info);
		strcpy(param, "float");
	}
	else if (type == 2)
	{
		sprintf(name, "IN%lluSTR", label_n);
		info.ptr = symtab_elem_add(symtab, name);
		des = createToken("STR_ID", info);
		strcpy(param, "string");
	}
	else
	{
		return NULL;
	}

	if (! print_or_append(code_buffer, in_stat, "READ GF@$des %s\n", param))
	{
		label_n++;
		destroyToken(des);
		info.ptr = NULL;
		des = createToken("ERR_INTERNAL", info);
		return des;
	}

	label_n++;
	return des;
}

token_t *print(symtable_t *symtab, stack_tkn_t *stack, list_t *code_buffer, bool in_stat)
{
	static unsigned long long label_n = 0;
	token_info_t info;
	info.string = "nil";
	token_t *des = createToken("nil", info);
	char param[7];      //from which frame is variable ID
	char frame[3] = "LF";
	char *print;
	if (strcmp(symtab->name, "$GT" ) == 0)
		strcpy(frame, "GF");

	token_t *tmp;
	int iterator = stack->top + 1;

	for (int i = 0; i < iterator; ++i)
	{
		tmp = stcTkn_pop(stack);
		if (strcmp(tmp->name, "ID") == 0)
		{
			if (strcmp(symtab->name, "$GT" ) == 0)
			{
				strcpy(param, "GF");
			}
			else
			{
				strcpy(param, "LF");
			}

			print = malloc(sizeof(char) * (strlen(tmp->info.ptr->var.key) + 1));
			if (print == NULL)
			{
				free(des->name);
				free(des);
				token_info_t info1;
				token_t *error = createToken("ERR_INT", info1);
				return error;
			}
			strcpy(print, tmp->info.ptr->var.key);
		}
		else
		{
			print = malloc(sizeof(char) * (strlen(tmp->info.string) + 1));
			if (print == NULL)
			{
				free(des->name);
				free(des);
				token_info_t info1;
				token_t *error = createToken("ERR_INT", info1);
				return error;
			}
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
			else if (strcmp(tmp->name, "nil") == 0)
			{
				strcpy(param, "string");
				strncpy(print, "", 3);
			}
		}

		if (! print_or_append(code_buffer, in_stat, "WRITE %s@%s\n", param, print))
			goto err_internal;
		destroyToken(tmp); // ????
	}

	label_n++;
	free(print);
	return des;

err_internal:
	destroyToken(tmp);
	destroyToken(des);
	info.ptr = NULL;
	des =  createToken("ERR_INT", info);
	label_n++;
	free(print);
	return des;
}

#if 0


//gcc builtin_gen.c symtable.c dynamicArrParam.c stackStr.c token.c stackTkn.c -o gen


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

	stack_tkn_t *stack = stcTkn_create();
	stcTkn_push(stack, token2);
	stcTkn_push(stack, token3);
	stcTkn_push(stack, token5);
	stcTkn_push(stack, token);


	token_t *tmp12 = print(symtable, stack);
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

