/**CFile****************************************************************

  FileName    [scanner.h]

  SystemName  [IFJ - PROJECT]

  PackageName [Scanner]

  Synopsis    [Lexical analysis - reads input and create tokens]

  Author      [Jindrich Sestak]

  Login       [xsesta05]

  Affiliation []

  Date        [19/10/2018]

  Revision    []

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                          INCLUDES                                ///
////////////////////////////////////////////////////////////////////////
#ifndef SCANNER_H
#define SCANNER_H

#include "symtable.h"
#include "queue.h"
#include "dynamic_str.h"
#include "token.h"

////////////////////////////////////////////////////////////////////////
///                         BASIC TYPES                              ///
////////////////////////////////////////////////////////////////////////
typedef enum
{
    State_S,        ///Starting state
	State_ID, /// Identifier
	State_IDEXL, //When "ID!"
	State_FUNC, //Function

	State_INT, /// Integer number
    State_INT0, //INT = 0
	State_FLOAT, /// Double number
	State_HEX,	//BEgining og hex number '\x' in sting
	State_HEX_NUM,	//Hex number with one number '\xA' in string
	
	State_QUATATION1, //Starting a string
	State_QUATATION2,	//Ending a string
	State_STR, /// String
	State_SPEC_CHAR, //Special symbols like '\n', '\s'....

	State_LEQ, /// Less or equal <=
	State_LTN, /// Less than <
	State_MEQ, /// More or equal >=
	State_MTN, /// More than >

	State_ASSIGN, /// Assign =
    State_EQ, /// Equal ==
    State_EXLAMATION, // Exlamation Mark '!'
    State_NEQ, /// Not equal !=
	State_PLUS, /// Plus +
	State_MINUS, /// Minus -
	State_MUL, /// Multiplication *
	State_DIV, /// Division / result always double

	State_LEFT_BRACKET, /// Left bracket (
	State_RIGHT_BRACKET, /// Right bracket )
    State_DOT, // Dot . (for float numbers)
    State_EXP,  //Mid-state of FLOAT ( 0e???)
    State_EXPS, //Mid-state of FLOAT with sign (0e-???)
	State_COMMA, /// Comma ,
	State_SEMICOLON, /// Semicolon ;

	State_EOF, /// End of file
	State_EOL, /// End of line
	State_COMM, //Starting of comment block EOL-> '='
	State_COMMB,	//Starting block of comments with EOL-> '=b'
	State_COMMBE,
	State_COMMBEG,
	State_COMMBEGI,
	State_COMMBEGIN,
	State_MAY_END,		//Maybe end of comments
	State_COMM_LINE, 	//Comments #
	State_END_COMM,		//Ending of comments
	State_END_COMME,	//Ending like "=e"
	State_END_COMMEN,
	State_END_COMMEND,
	State_LCOMM, 		//One line comment

//////////BASE extention///////////////////
	State_Binary, 	//Binary number to be transformed into normal integer
	State_Hexidecimal,	//Hexidecimal number to be transformed into normal integer
	State_Octal,		//Octal number to be transformed into normal integer
	State_Backslash,
///////////////////////////////////////////////////

	State_ERR,
} State;

////////////////////////////////////////////////////////////////////////
///                      MACRO DEFINITIONS                           ///
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
///                    FUNCTION DECLARATIONS                         ///
////////////////////////////////////////////////////////////////////////

/**
 * @brief	Function reads from input and generates tokens according to State automat
 * 
 * @param sc_str 		Dynamic string
 * @param que 			Queue of tokens
 * @return token_t* 	Token from input
 */
token_t* scanner_get(dynamicStr_t *sc_str, queue_t *que);


/**
 * @brief Returns token from queue
 * 
 * @param que 		Queue of tokens
 * @param token 	destined token
 * @param name 		atribute of token
 * @return true 	If succes
 * @return false 	if error
 */
bool scanner_unget(queue_t *que, token_t *token, char *name);

/**
 * @brief Function returns char and set state to default (State_S)
 * 
 * @param c character to be ungetc (returning character)
 */
void sc_unget(int c);

/**
 * @brief Converts str from input to str according to task to format of int
 * 
 * @param s 		destined string with needed format
 * @param sc_str 	input string
 * @return true 		If succes
 * @return false 		if internal error
 */
bool InttoStr (char *s, dynamicStr_t *sc_str);

/**
 * @brief Converts str from input to str according to task to format of double
 * 
 * @param s 		destined str with needed format
 * @param sc_str 	input string
 * @return true 		If succes
 * @return false 		if internal error
 */
bool DobtoStr (char *s, dynamicStr_t *sc_str);

/**
 * @brief Check if ID is keywords
 * 
 * @param str 			string to be compared with keywords
 * @param keywords 		array of names of keywords
 * @return char* 		return name of keyword or "ID"
 */
char* inKeyword(char *str, char **keywords);

/**
 * @brief  Converts hexadecimal number in string from input to str according to task to format of "\int"
 * 
 * @param sc_str 		input string
 * @param position 		position in sc_str where hexadecimal nuber begins
 * @return true 		If succes
 * @return false 		if internal error
 */
bool Str_with_hex(dynamicStr_t *sc_str, unsigned int position);

/**
 * @brief Check if ID is builtin_function
 * 
 * @param str 				string to be compared
 * @param builtin_func 		array of names of builtin_functions
 * @return char* 			return name of builtin_function or "ID"
 */
char *inBuiltin_Func(char *str, char **builtin_func);

#endif
////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////