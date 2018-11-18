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



token_t *generateExp(token_t *op, token_t *des, token_t *par2, token_t *par3, char *funName)
{
    if (isINT(par2))
    {
        if (isINT(par3))
        {
            if (op->name[0] == '+')
            {
                printf("ADD LF@%s int@%s int@%s\n", des->info.ptr->var.key, par2->info.string, par3->info.string);
                /*destroyToken(par2);
                destroyToken(par3);*/
                destroyToken(op);
                token_t *tmp = createToken("INT_ID", des->info);
                tmp->info.ptr = des->info.ptr;
                destroyToken(des);
                return tmp;
            }
            else if (op->name[0] == '-')
            {
                printf("SUB LF@%s int@%s int@%s\n", des->info.ptr->var.key, par2->info.string, par3->info.string);
                /*destroyToken(par2);
                destroyToken(par3);*/
                destroyToken(op);
                token_t *tmp = createToken("INT_ID", des->info);
                tmp->info.ptr = des->info.ptr;
                destroyToken(des);
                return tmp;

            }
            else if (op->name[0] == '*')
            {
                printf("MUL LF@%s int@%s int@%s\n", des->info.ptr->var.key, par2->info.string, par3->info.string);
                /*destroyToken(par2);
                destroyToken(par3);*/
                destroyToken(op);
                token_t *tmp = createToken("INT_ID", des->info);
                tmp->info.ptr = des->info.ptr;
                destroyToken(des);
                return tmp;
            }
            else if (op->name[0] == '/')
            {
                printf("IDIV LF@%s int@%s int@%s\n", des->info.ptr->var.key, par2->info.string, par3->info.string);
                /*destroyToken(par2);
                destroyToken(par3);*/
                destroyToken(op);
                token_t *tmp = createToken("INT_ID", des->info);
                tmp->info.ptr = des->info.ptr;
                destroyToken(des);
                return tmp;
            }
            else if (strcmp(op->name, "==") == 0)
            {
            }
            else if (op->name[0] == '=')
            {
            }
            else if (strcmp(op->name, "<=") == 0)
            {
            }
            else if (strcmp(op->name, ">=") == 0)
            {
            }
            else if (strcmp(op->name, "!=") == 0)
            {
            }
            else if (op->name[0] == '<')
            {
            }
            else if (op->name[0] == '>')
            {
            }
            else
            {
                fprintf(stderr, "Wrong operand\n");
            }
        }
        if (isDBL(par3))
        {
            if (op->name[0] == '+')
            {

            }
            else if (op->name[0] == '-')
            {

            }
            else if (op->name[0] == '*')
            {

            }
            else if (op->name[0] == '/')
            {
            }
            else if (strcmp(op->name, "==") == 0)
            {
            }
            else if (op->name[0] == '=')
            {
            }
            else if (strcmp(op->name, "<=") == 0)
            {
            }
            else if (strcmp(op->name, ">=") == 0)
            {
            }
            else if (strcmp(op->name, "!=") == 0)
            {
            }
            else if (op->name[0] == '<')
            {
            }
            else if (op->name[0] == '>')
            {
            }
            else
            {
                fprintf(stderr, "Wrong operand\n");
            }
        }
        if (isSTR(par3))
        {
            if (op->name[0] == '+')
            {

            }
            else if (op->name[0] == '-')
            {

            }
            else if (op->name[0] == '*')
            {

            }
            else if (op->name[0] == '/')
            {
            }
            else if (strcmp(op->name, "==") == 0)
            {
            }
            else if (op->name[0] == '=')
            {
            }
            else if (strcmp(op->name, "<=") == 0)
            {
            }
            else if (strcmp(op->name, ">=") == 0)
            {
            }
            else if (strcmp(op->name, "!=") == 0)
            {
            }
            else if (op->name[0] == '<')
            {
            }
            else if (op->name[0] == '>')
            {
            }
            else
            {
                fprintf(stderr, "Wrong operand\n");
            }
        }
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
            }
            else if (op->name[0] == '-')
            {

            }
            else if (op->name[0] == '*')
            {

            }
            else if (op->name[0] == '/')
            {
            }
            else if (strcmp(op->name, "==") == 0)
            {
            }
            else if (op->name[0] == '=')
            {
            }
            else if (strcmp(op->name, "<=") == 0)
            {
            }
            else if (strcmp(op->name, ">=") == 0)
            {
            }
            else if (strcmp(op->name, "!=") == 0)
            {
            }
            else if (op->name[0] == '<')
            {
            }
            else if (op->name[0] == '>')
            {
            }
            else
            {
                fprintf(stderr, "Wrong operand\n");
            }
        }
        if (isINT_ID(par3))
        {
            if (op->name[0] == '+')
            {

            }
            else if (op->name[0] == '-')
            {

            }
            else if (op->name[0] == '*')
            {

            }
            else if (op->name[0] == '/')
            {
            }
            else if (strcmp(op->name, "==") == 0)
            {
            }
            else if (op->name[0] == '=')
            {
            }
            else if (strcmp(op->name, "<=") == 0)
            {
            }
            else if (strcmp(op->name, ">=") == 0)
            {
            }
            else if (strcmp(op->name, "!=") == 0)
            {
            }
            else if (op->name[0] == '<')
            {
            }
            else if (op->name[0] == '>')
            {
            }
            else
            {
                fprintf(stderr, "Wrong operand\n");
            }
        }

        if (isDBL_ID(par3))
        {
            if (op->name[0] == '+')
            {

            }
            else if (op->name[0] == '-')
            {

            }
            else if (op->name[0] == '*')
            {

            }
            else if (op->name[0] == '/')
            {
            }
            else if (strcmp(op->name, "==") == 0)
            {
            }
            else if (op->name[0] == '=')
            {
            }
            else if (strcmp(op->name, "<=") == 0)
            {
            }
            else if (strcmp(op->name, ">=") == 0)
            {
            }
            else if (strcmp(op->name, "!=") == 0)
            {
            }
            else if (op->name[0] == '<')
            {
            }
            else if (op->name[0] == '>')
            {
            }
            else
            {
                fprintf(stderr, "Wrong operand\n");
            }
        }
    }
    else if (isDBL(par2))
    {
        if (isINT(par3))
        {
            if (op->name[0] == '+')
            {

            }
            else if (op->name[0] == '-')
            {

            }
            else if (op->name[0] == '*')
            {

            }
            else if (op->name[0] == '/')
            {
            }
            else if (strcmp(op->name, "==") == 0)
            {
            }
            else if (op->name[0] == '=')
            {
            }
            else if (strcmp(op->name, "<=") == 0)
            {
            }
            else if (strcmp(op->name, ">=") == 0)
            {
            }
            else if (strcmp(op->name, "!=") == 0)
            {
            }
            else if (op->name[0] == '<')
            {
            }
            else if (op->name[0] == '>')
            {
            }
            else
            {
                fprintf(stderr, "Wrong operand\n");
            }
        }
        if (isDBL(par3))
        {
            if (op->name[0] == '+')
            {

            }
            else if (op->name[0] == '-')
            {

            }
            else if (op->name[0] == '*')
            {

            }
            else if (op->name[0] == '/')
            {
            }
            else if (strcmp(op->name, "==") == 0)
            {
            }
            else if (op->name[0] == '=')
            {
            }
            else if (strcmp(op->name, "<=") == 0)
            {
            }
            else if (strcmp(op->name, ">=") == 0)
            {
            }
            else if (strcmp(op->name, "!=") == 0)
            {
            }
            else if (op->name[0] == '<')
            {
            }
            else if (op->name[0] == '>')
            {
            }
            else
            {
                fprintf(stderr, "Wrong operand\n");
            }
        }
        if (isSTR(par3))
        {
            if (op->name[0] == '+')
            {

            }
            else if (op->name[0] == '-')
            {

            }
            else if (op->name[0] == '*')
            {

            }
            else if (op->name[0] == '/')
            {
            }
            else if (strcmp(op->name, "==") == 0)
            {
            }
            else if (op->name[0] == '=')
            {
            }
            else if (strcmp(op->name, "<=") == 0)
            {
            }
            else if (strcmp(op->name, ">=") == 0)
            {
            }
            else if (strcmp(op->name, "!=") == 0)
            {
            }
            else if (op->name[0] == '<')
            {
            }
            else if (op->name[0] == '>')
            {
            }
            else
            {
                fprintf(stderr, "Wrong operand\n");
            }
        }
        if (isID(par3))
        {
            if (op->name[0] == '+')
            {

            }
            else if (op->name[0] == '-')
            {

            }
            else if (op->name[0] == '*')
            {

            }
            else if (op->name[0] == '/')
            {
            }
            else if (strcmp(op->name, "==") == 0)
            {
            }
            else if (op->name[0] == '=')
            {
            }
            else if (strcmp(op->name, "<=") == 0)
            {
            }
            else if (strcmp(op->name, ">=") == 0)
            {
            }
            else if (strcmp(op->name, "!=") == 0)
            {
            }
            else if (op->name[0] == '<')
            {
            }
            else if (op->name[0] == '>')
            {
            }
            else
            {
                fprintf(stderr, "Wrong operand\n");
            }
        }
        if (isINT_ID(par3))
        {
            if (op->name[0] == '+')
            {

            }
            else if (op->name[0] == '-')
            {

            }
            else if (op->name[0] == '*')
            {

            }
            else if (op->name[0] == '/')
            {
            }
            else if (strcmp(op->name, "==") == 0)
            {
            }
            else if (op->name[0] == '=')
            {
            }
            else if (strcmp(op->name, "<=") == 0)
            {
            }
            else if (strcmp(op->name, ">=") == 0)
            {
            }
            else if (strcmp(op->name, "!=") == 0)
            {
            }
            else if (op->name[0] == '<')
            {
            }
            else if (op->name[0] == '>')
            {
            }
            else
            {
                fprintf(stderr, "Wrong operand\n");
            }
        }
        if (isDBL_ID(par3))
        {
            if (op->name[0] == '+')
            {

            }
            else if (op->name[0] == '-')
            {

            }
            else if (op->name[0] == '*')
            {

            }
            else if (op->name[0] == '/')
            {
            }
            else if (strcmp(op->name, "==") == 0)
            {
            }
            else if (op->name[0] == '=')
            {
            }
            else if (strcmp(op->name, "<=") == 0)
            {
            }
            else if (strcmp(op->name, ">=") == 0)
            {
            }
            else if (strcmp(op->name, "!=") == 0)
            {
            }
            else if (op->name[0] == '<')
            {
            }
            else if (op->name[0] == '>')
            {
            }
            else
            {
                fprintf(stderr, "Wrong operand\n");
            }
        }
    }
    else if (isSTR(par2))
    {
        if (isINT(par3))
        {
            if (op->name[0] == '+')
            {

            }
            else if (op->name[0] == '-')
            {

            }
            else if (op->name[0] == '*')
            {

            }
            else if (op->name[0] == '/')
            {
            }
            else if (strcmp(op->name, "==") == 0)
            {
            }
            else if (op->name[0] == '=')
            {
            }
            else if (strcmp(op->name, "<=") == 0)
            {
            }
            else if (strcmp(op->name, ">=") == 0)
            {
            }
            else if (strcmp(op->name, "!=") == 0)
            {
            }
            else if (op->name[0] == '<')
            {
            }
            else if (op->name[0] == '>')
            {
            }
            else
            {
                fprintf(stderr, "Wrong operand\n");
            }
        }
        if (isDBL(par3))
        {
            if (op->name[0] == '+')
            {

            }
            else if (op->name[0] == '-')
            {

            }
            else if (op->name[0] == '*')
            {

            }
            else if (op->name[0] == '/')
            {
            }
            else if (strcmp(op->name, "==") == 0)
            {
            }
            else if (op->name[0] == '=')
            {
            }
            else if (strcmp(op->name, "<=") == 0)
            {
            }
            else if (strcmp(op->name, ">=") == 0)
            {
            }
            else if (strcmp(op->name, "!=") == 0)
            {
            }
            else if (op->name[0] == '<')
            {
            }
            else if (op->name[0] == '>')
            {
            }
            else
            {
                fprintf(stderr, "Wrong operand\n");
            }
        }
        if (isSTR(par3))
        {
            if (op->name[0] == '+')
            {

            }
            else if (op->name[0] == '-')
            {

            }
            else if (op->name[0] == '*')
            {

            }
            else if (op->name[0] == '/')
            {
            }
            else if (strcmp(op->name, "==") == 0)
            {
            }
            else if (op->name[0] == '=')
            {
            }
            else if (strcmp(op->name, "<=") == 0)
            {
            }
            else if (strcmp(op->name, ">=") == 0)
            {
            }
            else if (strcmp(op->name, "!=") == 0)
            {
            }
            else if (op->name[0] == '<')
            {
            }
            else if (op->name[0] == '>')
            {
            }
            else
            {
                fprintf(stderr, "Wrong operand\n");
            }
        }
        if (isID(par3))
        {
            if (op->name[0] == '+')
            {

            }
            else if (op->name[0] == '-')
            {

            }
            else if (op->name[0] == '*')
            {

            }
            else if (op->name[0] == '/')
            {
            }
            else if (strcmp(op->name, "==") == 0)
            {
            }
            else if (op->name[0] == '=')
            {
            }
            else if (strcmp(op->name, "<=") == 0)
            {
            }
            else if (strcmp(op->name, ">=") == 0)
            {
            }
            else if (strcmp(op->name, "!=") == 0)
            {
            }
            else if (op->name[0] == '<')
            {
            }
            else if (op->name[0] == '>')
            {
            }
            else
            {
                fprintf(stderr, "Wrong operand\n");
            }
        }
        if (isINT_ID(par3))
        {
            if (op->name[0] == '+')
            {

            }
            else if (op->name[0] == '-')
            {

            }
            else if (op->name[0] == '*')
            {

            }
            else if (op->name[0] == '/')
            {
            }
            else if (strcmp(op->name, "==") == 0)
            {
            }
            else if (op->name[0] == '=')
            {
            }
            else if (strcmp(op->name, "<=") == 0)
            {
            }
            else if (strcmp(op->name, ">=") == 0)
            {
            }
            else if (strcmp(op->name, "!=") == 0)
            {
            }
            else if (op->name[0] == '<')
            {
            }
            else if (op->name[0] == '>')
            {
            }
            else
            {
                fprintf(stderr, "Wrong operand\n");
            }
        }
        if (isDBL_ID(par3))
        {
            if (op->name[0] == '+')
            {

            }
            else if (op->name[0] == '-')
            {

            }
            else if (op->name[0] == '*')
            {

            }
            else if (op->name[0] == '/')
            {
            }
            else if (strcmp(op->name, "==") == 0)
            {
            }
            else if (op->name[0] == '=')
            {
            }
            else if (strcmp(op->name, "<=") == 0)
            {
            }
            else if (strcmp(op->name, ">=") == 0)
            {
            }
            else if (strcmp(op->name, "!=") == 0)
            {
            }
            else if (op->name[0] == '<')
            {
            }
            else if (op->name[0] == '>')
            {
            }
            else
            {
                fprintf(stderr, "Wrong operand\n");
            }
        }
    }
    else if (isID(par2))
    {
        if (isINT(par3))
        {
            if (op->name[0] == '+')
            {

            }
            else if (op->name[0] == '-')
            {

            }
            else if (op->name[0] == '*')
            {

            }
            else if (op->name[0] == '/')
            {
            }
            else if (strcmp(op->name, "==") == 0)
            {
            }
            else if (op->name[0] == '=')
            {
            }
            else if (strcmp(op->name, "<=") == 0)
            {
            }
            else if (strcmp(op->name, ">=") == 0)
            {
            }
            else if (strcmp(op->name, "!=") == 0)
            {
            }
            else if (op->name[0] == '<')
            {
            }
            else if (op->name[0] == '>')
            {
            }
            else
            {
                fprintf(stderr, "Wrong operand\n");
            }
        }
        if (isDBL(par3))
        {
            if (op->name[0] == '+')
            {

            }
            else if (op->name[0] == '-')
            {

            }
            else if (op->name[0] == '*')
            {

            }
            else if (op->name[0] == '/')
            {
            }
            else if (strcmp(op->name, "==") == 0)
            {
            }
            else if (op->name[0] == '=')
            {
            }
            else if (strcmp(op->name, "<=") == 0)
            {
            }
            else if (strcmp(op->name, ">=") == 0)
            {
            }
            else if (strcmp(op->name, "!=") == 0)
            {
            }
            else if (op->name[0] == '<')
            {
            }
            else if (op->name[0] == '>')
            {
            }
            else
            {
                fprintf(stderr, "Wrong operand\n");
            }
        }
        if (isSTR(par3))
        {
            if (op->name[0] == '+')
            {

            }
            else if (op->name[0] == '-')
            {

            }
            else if (op->name[0] == '*')
            {

            }
            else if (op->name[0] == '/')
            {
            }
            else if (strcmp(op->name, "==") == 0)
            {
            }
            else if (op->name[0] == '=')
            {
            }
            else if (strcmp(op->name, "<=") == 0)
            {
            }
            else if (strcmp(op->name, ">=") == 0)
            {
            }
            else if (strcmp(op->name, "!=") == 0)
            {
            }
            else if (op->name[0] == '<')
            {
            }
            else if (op->name[0] == '>')
            {
            }
            else
            {
                fprintf(stderr, "Wrong operand\n");
            }
        }
        if (isID(par3))
        {
            if (op->name[0] == '+')
            {

            }
            else if (op->name[0] == '-')
            {

            }
            else if (op->name[0] == '*')
            {

            }
            else if (op->name[0] == '/')
            {
            }
            else if (strcmp(op->name, "==") == 0)
            {
            }
            else if (op->name[0] == '=')
            {
            }
            else if (strcmp(op->name, "<=") == 0)
            {
            }
            else if (strcmp(op->name, ">=") == 0)
            {
            }
            else if (strcmp(op->name, "!=") == 0)
            {
            }
            else if (op->name[0] == '<')
            {
            }
            else if (op->name[0] == '>')
            {
            }
            else
            {
                fprintf(stderr, "Wrong operand\n");
            }
        }
        if (isINT_ID(par3))
        {
            if (op->name[0] == '+')
            {

            }
            else if (op->name[0] == '-')
            {

            }
            else if (op->name[0] == '*')
            {

            }
            else if (op->name[0] == '/')
            {
            }
            else if (strcmp(op->name, "==") == 0)
            {
            }
            else if (op->name[0] == '=')
            {
            }
            else if (strcmp(op->name, "<=") == 0)
            {
            }
            else if (strcmp(op->name, ">=") == 0)
            {
            }
            else if (strcmp(op->name, "!=") == 0)
            {
            }
            else if (op->name[0] == '<')
            {
            }
            else if (op->name[0] == '>')
            {
            }
            else
            {
                fprintf(stderr, "Wrong operand\n");
            }
        }
        if (isDBL_ID(par3))
        {
            if (op->name[0] == '+')
            {

            }
            else if (op->name[0] == '-')
            {

            }
            else if (op->name[0] == '*')
            {

            }
            else if (op->name[0] == '/')
            {
            }
            else if (strcmp(op->name, "==") == 0)
            {
            }
            else if (op->name[0] == '=')
            {
            }
            else if (strcmp(op->name, "<=") == 0)
            {
            }
            else if (strcmp(op->name, ">=") == 0)
            {
            }
            else if (strcmp(op->name, "!=") == 0)
            {
            }
            else if (op->name[0] == '<')
            {
            }
            else if (op->name[0] == '>')
            {
            }
            else
            {
                fprintf(stderr, "Wrong operand\n");
            }
        }
    }
    else if (isINT_ID(par2))
    {
        if (isINT(par3))
        {
            if (op->name[0] == '+')
            {

            }
            else if (op->name[0] == '-')
            {

            }
            else if (op->name[0] == '*')
            {

            }
            else if (op->name[0] == '/')
            {
            }
            else if (strcmp(op->name, "==") == 0)
            {
            }
            else if (op->name[0] == '=')
            {
            }
            else if (strcmp(op->name, "<=") == 0)
            {
            }
            else if (strcmp(op->name, ">=") == 0)
            {
            }
            else if (strcmp(op->name, "!=") == 0)
            {
            }
            else if (op->name[0] == '<')
            {
            }
            else if (op->name[0] == '>')
            {
            }
            else
            {
                fprintf(stderr, "Wrong operand\n");
            }
        }
        if (isDBL(par3))
        {
            if (op->name[0] == '+')
            {

            }
            else if (op->name[0] == '-')
            {

            }
            else if (op->name[0] == '*')
            {

            }
            else if (op->name[0] == '/')
            {
            }
            else if (strcmp(op->name, "==") == 0)
            {
            }
            else if (op->name[0] == '=')
            {
            }
            else if (strcmp(op->name, "<=") == 0)
            {
            }
            else if (strcmp(op->name, ">=") == 0)
            {
            }
            else if (strcmp(op->name, "!=") == 0)
            {
            }
            else if (op->name[0] == '<')
            {
            }
            else if (op->name[0] == '>')
            {
            }
            else
            {
                fprintf(stderr, "Wrong operand\n");
            }
        }
        if (isSTR(par3))
        {
            if (op->name[0] == '+')
            {

            }
            else if (op->name[0] == '-')
            {

            }
            else if (op->name[0] == '*')
            {

            }
            else if (op->name[0] == '/')
            {
            }
            else if (strcmp(op->name, "==") == 0)
            {
            }
            else if (op->name[0] == '=')
            {
            }
            else if (strcmp(op->name, "<=") == 0)
            {
            }
            else if (strcmp(op->name, ">=") == 0)
            {
            }
            else if (strcmp(op->name, "!=") == 0)
            {
            }
            else if (op->name[0] == '<')
            {
            }
            else if (op->name[0] == '>')
            {
            }
            else
            {
                fprintf(stderr, "Wrong operand\n");
            }
        }
        if (isID(par3))
        {
            if (op->name[0] == '+')
            {

            }
            else if (op->name[0] == '-')
            {

            }
            else if (op->name[0] == '*')
            {

            }
            else if (op->name[0] == '/')
            {
            }
            else if (strcmp(op->name, "==") == 0)
            {
            }
            else if (op->name[0] == '=')
            {
            }
            else if (strcmp(op->name, "<=") == 0)
            {
            }
            else if (strcmp(op->name, ">=") == 0)
            {
            }
            else if (strcmp(op->name, "!=") == 0)
            {
            }
            else if (op->name[0] == '<')
            {
            }
            else if (op->name[0] == '>')
            {
            }
            else
            {
                fprintf(stderr, "Wrong operand\n");
            }
        }
        if (isINT_ID(par3))
        {
            if (op->name[0] == '+')
            {

            }
            else if (op->name[0] == '-')
            {

            }
            else if (op->name[0] == '*')
            {

            }
            else if (op->name[0] == '/')
            {
            }
            else if (strcmp(op->name, "==") == 0)
            {
            }
            else if (op->name[0] == '=')
            {
            }
            else if (strcmp(op->name, "<=") == 0)
            {
            }
            else if (strcmp(op->name, ">=") == 0)
            {
            }
            else if (strcmp(op->name, "!=") == 0)
            {
            }
            else if (op->name[0] == '<')
            {
            }
            else if (op->name[0] == '>')
            {
            }
            else
            {
                fprintf(stderr, "Wrong operand\n");
            }
        }
        if (isDBL_ID(par3))
        {
            if (op->name[0] == '+')
            {

            }
            else if (op->name[0] == '-')
            {

            }
            else if (op->name[0] == '*')
            {

            }
            else if (op->name[0] == '/')
            {
            }
            else if (strcmp(op->name, "==") == 0)
            {
            }
            else if (op->name[0] == '=')
            {
            }
            else if (strcmp(op->name, "<=") == 0)
            {
            }
            else if (strcmp(op->name, ">=") == 0)
            {
            }
            else if (strcmp(op->name, "!=") == 0)
            {
            }
            else if (op->name[0] == '<')
            {
            }
            else if (op->name[0] == '>')
            {
            }
            else
            {
                fprintf(stderr, "Wrong operand\n");
            }
        }
    }
    else if (isDBL_ID(par2))
    {
        if (isINT(par3))
        {
            if (op->name[0] == '+')
            {

            }
            else if (op->name[0] == '-')
            {

            }
            else if (op->name[0] == '*')
            {

            }
            else if (op->name[0] == '/')
            {
            }
            else if (strcmp(op->name, "==") == 0)
            {
            }
            else if (op->name[0] == '=')
            {
            }
            else if (strcmp(op->name, "<=") == 0)
            {
            }
            else if (strcmp(op->name, ">=") == 0)
            {
            }
            else if (strcmp(op->name, "!=") == 0)
            {
            }
            else if (op->name[0] == '<')
            {
            }
            else if (op->name[0] == '>')
            {
            }
            else
            {
                fprintf(stderr, "Wrong operand\n");
            }
        }
        if (isDBL(par3))
        {
            if (op->name[0] == '+')
            {

            }
            else if (op->name[0] == '-')
            {

            }
            else if (op->name[0] == '*')
            {

            }
            else if (op->name[0] == '/')
            {
            }
            else if (strcmp(op->name, "==") == 0)
            {
            }
            else if (op->name[0] == '=')
            {
            }
            else if (strcmp(op->name, "<=") == 0)
            {
            }
            else if (strcmp(op->name, ">=") == 0)
            {
            }
            else if (strcmp(op->name, "!=") == 0)
            {
            }
            else if (op->name[0] == '<')
            {
            }
            else if (op->name[0] == '>')
            {
            }
            else
            {
                fprintf(stderr, "Wrong operand\n");
            }
        }
        if (isSTR(par3))
        {
            if (op->name[0] == '+')
            {

            }
            else if (op->name[0] == '-')
            {

            }
            else if (op->name[0] == '*')
            {

            }
            else if (op->name[0] == '/')
            {
            }
            else if (strcmp(op->name, "==") == 0)
            {
            }
            else if (op->name[0] == '=')
            {
            }
            else if (strcmp(op->name, "<=") == 0)
            {
            }
            else if (strcmp(op->name, ">=") == 0)
            {
            }
            else if (strcmp(op->name, "!=") == 0)
            {
            }
            else if (op->name[0] == '<')
            {
            }
            else if (op->name[0] == '>')
            {
            }
            else
            {
                fprintf(stderr, "Wrong operand\n");
            }
        }
        if (isID(par3))
        {
            if (op->name[0] == '+')
            {

            }
            else if (op->name[0] == '-')
            {

            }
            else if (op->name[0] == '*')
            {

            }
            else if (op->name[0] == '/')
            {
            }
            else if (strcmp(op->name, "==") == 0)
            {
            }
            else if (op->name[0] == '=')
            {
            }
            else if (strcmp(op->name, "<=") == 0)
            {
            }
            else if (strcmp(op->name, ">=") == 0)
            {
            }
            else if (strcmp(op->name, "!=") == 0)
            {
            }
            else if (op->name[0] == '<')
            {
            }
            else if (op->name[0] == '>')
            {
            }
            else
            {
                fprintf(stderr, "Wrong operand\n");
            }
        }
        if (isINT_ID(par3))
        {
            if (op->name[0] == '+')
            {

            }
            else if (op->name[0] == '-')
            {

            }
            else if (op->name[0] == '*')
            {

            }
            else if (op->name[0] == '/')
            {
            }
            else if (strcmp(op->name, "==") == 0)
            {
            }
            else if (op->name[0] == '=')
            {
            }
            else if (strcmp(op->name, "<=") == 0)
            {
            }
            else if (strcmp(op->name, ">=") == 0)
            {
            }
            else if (strcmp(op->name, "!=") == 0)
            {
            }
            else if (op->name[0] == '<')
            {
            }
            else if (op->name[0] == '>')
            {
            }
            else
            {
                fprintf(stderr, "Wrong operand\n");
            }
        }
        if (isDBL_ID(par3))
        {
            if (op->name[0] == '+')
            {

            }
            else if (op->name[0] == '-')
            {

            }
            else if (op->name[0] == '*')
            {

            }
            else if (op->name[0] == '/')
            {
            }
            else if (strcmp(op->name, "==") == 0)
            {
            }
            else if (op->name[0] == '=')
            {
            }
            else if (strcmp(op->name, "<=") == 0)
            {
            }
            else if (strcmp(op->name, ">=") == 0)
            {
            }
            else if (strcmp(op->name, "!=") == 0)
            {
            }
            else if (op->name[0] == '<')
            {
            }
            else if (op->name[0] == '>')
            {
            }
            else
            {
                fprintf(stderr, "Wrong operand\n");
            }
        }
    }

    /*
    else if (op->name[0] == '+')
    {

    }
    else if (op->name[0] == '-')
    {

    }
    else if (op->name[0] == '*')
    {

    }
    else if (op->name[0] == '/')
    {
    }
    else if (strcmp(op->name, "==") == 0)
    {
    }
    else if (op->name[0] == '=')
    {
    }
    else if (strcmp(op->name, "<=") == 0)
    {
    }
    else if (strcmp(op->name, ">=") == 0)
    {
    }
    else if (op->name[0] == '<')
    {
    }
    else if (op->name[0] == '>')
    {
    }
    else if (op->name[0] == '!=')
    {
    }
    else
    {
        fprintf(stderr, "Wrong operand\n");
    }
    */

    return NULL;
}

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
    token_t *token3 = createToken("ID", info);
    token_t *tmp12 = generateExp(op, token, token2, token3, "foo");


    free(tmp12);
    free(token);
    free(op);
    free(token2);
    free(token3);
    return 0;
}



////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////

