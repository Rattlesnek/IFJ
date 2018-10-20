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

////////////////////////////////////////////////////////////////////////
///                         BASIC TYPES                              ///
////////////////////////////////////////////////////////////////////////

/****************************************************/
/* NEW AND UPDATED*/
typedef struct element elem_t;

typedef union token_info {
    elem_t *ptr;    // either ptr to info-element in hash table (use in all cases except latter two)
    int intg;       // or integer (use only when token name is "INT")
    double dbl;     // or double (use only when token name is "DBL")
} token_info_t;

/* NEW AND UPDATED*/
/**
 * @brief item structure
 * @param *key dynamically allocated array of chars -> word
 * @param data number of occurences of the word
 * @param *next pointer to next item
 */
struct element
{
    char *key;
    token_info_t data;
    struct element *next;
};
/* NEW AND UPDATED*/
/**
 * @brief hash table structure
 * @param size number of items in the table
 * @param array_size number of indexes in the array of pointers
 * @param *array[] of pointers to items
 */
typedef struct symtable
{
    size_t size;
    size_t arr_size;
    elem_t *ptr[];
} symtable_t;



struct token {
    char *name;         // token name eg. "ID", "STR", "+", "==", "EOL", ...
    token_info_t info;  // token info 
};


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