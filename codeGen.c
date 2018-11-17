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



token_t *generate(token_t *op, token_t *par1, token_t *par2, token_t *par3)
{
	if (isINT(par2))
	{
		if (isINT(par3))
		{
			if (op->name[0] == '+')
			{
				printf("ADD LF@%s int@%s int@%s\n", par1->info.ptr->var.key, par2->info.string, par3->info.string);
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
	elem.var.key = "promena";
	token_info_t info;
	info.ptr = &elem;

	token_info_t tmp;
	tmp.ptr = NULL;

	token_t *token = createToken("ID", info);
	token_t *op = createToken("+", tmp);
	printf("DEFVAR LF@%s$type\n", token->info.ptr->var.key);
	token_info_t info2;
	info2.string = "5";
	token_t *token2 = createToken("INT", info2);
	token_t *token3 = createToken("INT", info2);
	generate(op, token, token2, token3);
	return 0;
}



////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////

