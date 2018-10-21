/**CFile****************************************************************
  FileName    [token.h]
  SystemName  [IFJ - PROJECT]
  PackageName [Lexical analysis]
  Synopsis    [Token manipulation]
  Author      [Adam Pankuch]
  Affiliation []
  Date        [18/10/2018]
  Revision    []
***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                          INCLUDES                                ///
////////////////////////////////////////////////////////////////////////
#ifndef TOKEN_H
#define TOKEN_H
#include <stdio.h>
#include <stdbool.h>

////////////////////////////////////////////////////////////////////////
///                         BASIC TYPES                              ///
////////////////////////////////////////////////////////////////////////

/****************************************************/
<<<<<<< HEAD


typedef enum{unknown, intg, dbl, string, id, id_f, eol, eof, bol, keyword, op} type_t; 

=======


typedef enum{unknown, intg, dbl, string, id, id_f, eol, eof, bol, keyword, op} type_t; 

>>>>>>> 599c0d76a50513793cad11942ca89d1ffc4e9c7f
typedef union token_info {
    char *string;   // either ptr to info-element in hash table (use in all cases except latter two)
    int intg;       // or integer (use only when token name is "INT")
    double dbl;     // or double (use only when token name is "DBL")
} token_info_t;


/**
 * @brief item structure
 * @param *key dynamically allocated array of chars -> identifier
 * @param data info about the token
 * @param *next pointer to next item
 */
typedef struct element
{
    char *key;
    token_info_t data;
    type_t token_type;
    type_t value_type;
    //struct  element *first_param; // if element is function, this points to first parameter
    //struct  element *next_param; // if element is parameter, this points to next parameter
    bool is_init;
    bool is_global; //?
    struct element *next;
} elem_t;

/**
 * @brief hash table structure
 * @param size number of items in the table
 * @param array_size number of indexes in the array of pointers
 * @param *ptr[] array of pointers to items
 */
typedef struct symtable
{
    size_t size;
    size_t arr_size;
    elem_t *ptr[];
} symtable_t;



typedef struct token {
    char *name;         // token name eg. "ID", "STR", "+", "==", "EOL", ...
    token_info_t info;  // token info
   // char type;        
} token_t;


////////////////////////////////////////////////////////////////////////
///                      MACRO DEFINITIONS                           ///
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
///                    FUNCTION DECLARATIONS                         ///
////////////////////////////////////////////////////////////////////////

/**
 * @brief Creates token and initializes it
 * 
 * @param name token name eg. "ID", "STR", "+", "==", "EOL", ...
 * @param info either info or ptr to corresponding info-element in hashtable
 * @return token_t* initialized token or NULL if not succesful
 */
token_t *tk_CreateToken(char *name, token_info_t info);


/**
 * @brief Destroys token
 * 
 * @param token token to be destroyed
 */
void tk_DestroyToken(token_t *token);


#endif
////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////