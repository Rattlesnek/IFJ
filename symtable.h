/**CFile****************************************************************

  FileName    [symtable.h]

  SystemName  [IFJ - PROJECT]

  PackageName [Symbol table]

  Synopsis    [Table operations]

  Author      [Lukas Valek]

  Affiliation []

  Date        [20/10/2018]

  Revision    []

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                          INCLUDES                                ///
////////////////////////////////////////////////////////////////////////
#ifndef symtab_h
#define symtab_h

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


////////////////////////////////////////////////////////////////////////
///                         BASIC TYPES                              ///
////////////////////////////////////////////////////////////////////////

typedef enum{unknown, intg, dbl, string, bol} value_t;
typedef enum{id, var, id_f} type_t;


typedef struct element elem_t; 
typedef union token_info {
    elem_t *ptr;   // either ptr to info-element in hash table (use in all cases except latter two)
    int intg;       // or integer (use only when token name is "INT")
    double dbl;     // or double (use only when token name is "DBL")
    char *string;
} token_info_t;


/**
 * @brief item structure
 * @param *key dynamically allocated array of chars -> identifier
 * @param data info about the token
 * @param *next pointer to next item
 */
struct element
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
} ;

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
 * @brief creates hash number for given string
 * @param *str array of chars
 * @return value of the hashed word
 */
size_t symtab_hash_function(const char *str);

/**
 * @brief gives the number of items in the table
 * @param *t pointer to the hash table
 * @return number of items in the table
 */
size_t symtab_size(symtable_t *t);

/**
 * @brief gives the number of indexes of the table
 * @param *t pointer to the hash table
 * @return number of indexes in the table
 */
size_t symtab_bucket_count(symtable_t *t);

/**
 * @brief frees all items in the table, keeps the table allocated
 * @param *t pointer to the hash table
 */
void symtab_clear(symtable_t *t);

/**
 * @brief calls symtab_clear, frees the table
 * @param *t pointer to the hash table
 */
void symtab_free(symtable_t *t);

/**
 * @brief constructor: creates and initializes the table with given size
 * @param size number of indexes of the table
 * @return pointer to created table
 */
symtable_t * symtab_init(size_t size);

/**
 * @brief calls a function for each item in the table
 * @param *t pointer to the hash table
 * @param func function that will be called for each item
 */
void symtab_foreach(symtable_t *t, void (*func) (elem_t *elem));

/**
 * @brief finds an item based on the match of keys
 * @param *key pointer to the string
 * @param *t pointer to the hash table
 * @return NULL if the item was not found, pointer to he item if the item was found
 */
struct element *symtab_find(symtable_t *t, const char *key);

/**
 * @brief removes the item from the linked list based on the match of keys
 * @param *key pointer to the string
 * @param *t pointer to the hash table
 * @param false if the item doesn't exist, true if succesfully removed
 */
bool symtab_remove(symtable_t *t, const char *key);

/**
 * @brief looks up item in the table, if the item exists the funcstion 
   increments size_t data, if the item doesn't exist the function will create 
   it and add it to the end of the list
 * @param *key pointer to the string
 * @param *t pointer to the hash table
 * @param pointer to new or found item
 */
struct element *symtab_token_add(symtable_t *t, token_t *token);














#endif
////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////
