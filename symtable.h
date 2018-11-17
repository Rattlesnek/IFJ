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
#ifndef SYMTAB_H
#define SYMTAB_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>



////////////////////////////////////////////////////////////////////////
///                         BASIC TYPES                              ///
////////////////////////////////////////////////////////////////////////

typedef enum {VARIABLES, FUNCTIONS} table_type_t;


typedef struct function {
    char *key;
    bool is_defined;
    int n_params;
} func_t;

typedef struct variable {
    char *key;
    // parameters of functions are variables ???
} var_t;

/**
 * @brief item structure
 * @param *key dynamically allocated array of chars -> identifier
 * @param data info about the token
 * @param *next pointer to next item
 */

typedef struct element
{
    union {
        var_t var;
        func_t func;
    };
 
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
    table_type_t type;
    size_t size;
    size_t arr_size;
    elem_t *ptr[];
} symtable_t;

////////////////////////////////////////////////////////////////////////
///                      MACRO DEFINITIONS                           ///
////////////////////////////////////////////////////////////////////////

#define UNDEF_NO_PARAMS -1

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
 * @brief      adds element into the symtable
 * @param      symtab  pointer to the target symtab
 * @param      name    type of the ID
 * @param      key     Real value of the variable
 * @return     returns a pointer to the element
 */
elem_t *symtab_elem_add(symtable_t *symtab, char *key);

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
symtable_t * symtab_init(size_t size, table_type_t type);

/**
 * @brief calls a function for each item in the table
 * @param *t pointer to the hash table
 * @param func function that will be called for each item
 */
bool symtab_foreach(symtable_t *t, bool (*func) (elem_t *elem));

/**
 * @brief finds an item based on the match of keys
 * @param *key pointer to the string
 * @param *t pointer to the hash table
 * @return NULL if the item was not found, pointer to he item if the item was found
 */
elem_t *symtab_find(symtable_t *t, const char *key);

/**
 * @brief removes the item from the linked list based on the match of keys
 * @param *key pointer to the string
 * @param *t pointer to the hash table
 * @param false if the item doesn't exist, true if succesfully removed
 */
bool symtab_remove(symtable_t *t, const char *key);

/**
 * @brief      updates the variable "is_defined" in the element
 *
 * @param[in]      t           Pointer to the hash table
 * @param[in]  is_defined  Indicates a function is if defined
 * @param[in]      key         The key used as in the hash function
 */

//elem_t *symtab_elem_add(symtable_t *symtab, char *name /*(ID, FUNC)*/, char *key /*(realne jmeno promenne).*/) 



#endif
////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////

