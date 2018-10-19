/**CFile****************************************************************

  FileName    []

  SystemName  [IFJ - PROJECT]

  PackageName []

  Synopsis    []

  Author      []

  Affiliation []

  Date        [--/--/2018]

  Revision    []

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                          INCLUDES                                ///
////////////////////////////////////////////////////////////////////////
#ifndef TEMPLATE_H
#define TEMPLATE_H

#include <stdio.h>

////////////////////////////////////////////////////////////////////////
///                         BASIC TYPES                              ///
////////////////////////////////////////////////////////////////////////
typedef enum
{
    State_S,        ///Starting state
	State_EOF, /// End of file
	State_EOL, /// End of line
	State_EMPTY, /// Empty
	State_ID, /// Identifier
	State_KEYWORD, /// Keyword

	State_INT, /// Integer number
	State_DOUBLE, /// Double number
	State_STR, /// String

	State_NEQ, /// Not equal <>
	State_LEQ, /// Less or equal <=
	State_LTN, /// Less than <
	State_MEQ, /// More or equal >=
	State_MTN, /// More than >

	// operators
	State_ASSIGN, /// Assign =
	State_PLUS, /// Plus +
	State_MINUS, /// Minus -
	State_MUL, /// Multiplication *
	State_DIV, /// Division / result always double
	State_IDIV, /// Integer division \ only works with integers

	State_LEFT_BRACKET, /// Left bracket (
	State_RIGHT_BRACKET, /// Right bracket )
	State_COMMA, /// Comma ,
	State_SEMICOLON, /// Semicolon ;
} State;

////////////////////////////////////////////////////////////////////////
///                      MACRO DEFINITIONS                           ///
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
///                    FUNCTION DECLARATIONS                         ///
////////////////////////////////////////////////////////////////////////






#endif
////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////