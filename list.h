/**CFile****************************************************************

  FileName    [list.h]

  SystemName  [IFJ - PROJECT]

  PackageName []

  Synopsis    []

  Author      [Adam Pankuch]

  Affiliation []

  Date        [28/11/2018]

  Revision    []

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                          INCLUDES                                ///
////////////////////////////////////////////////////////////////////////
#ifndef LIST_H
#define LIST_H

#include <stdarg.h>
#include <stdbool.h>

////////////////////////////////////////////////////////////////////////
///                         BASIC TYPES                              ///
////////////////////////////////////////////////////////////////////////

/**
 * @brief Element of list
 * 
 */
typedef struct list_element {
    char *string;
    struct list_element *next;
} list_elem_t;


/**
 * @brief List of strings structure
 * 
 */
typedef struct {
    list_elem_t *first;  // pointer to first element
    list_elem_t *last;   // pointer to last element
} list_t;


////////////////////////////////////////////////////////////////////////
///                      MACRO DEFINITIONS                           ///
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
///                    FUNCTION DECLARATIONS                         ///
////////////////////////////////////////////////////////////////////////

/**
 * @brief Prints to stdout or appends to list according to 'append'
 * 
 * @param list      list -- buffer of generated code
 * @param append    if true appends to list else prints to stdout
 * @param fmt       format string
 * @param ...       arguments
 * @return true     if success
 * @return false    if internal error
 */
bool print_or_append(list_t *list, bool append, const char *fmt, ...);


/**
 * @brief Create list
 * 
 * @return list_t* if success
 *         NULL    if internal error
 */
list_t *list_create();


/**
 * @brief Clean list
 * 
 * @param list list to be cleaned
 */
void list_clean(list_t *list);


/**
 * @brief Destroy list
 * 
 * @param list list to be destroyed
 */
void list_destroy(list_t *list);


/**
 * @brief Append string to list
 * 
 * @param list      list where string will be appended
 * @param buffer    string which should be appended
 * @return true     if success
 * @return false    if internal error
 */
bool list_append(list_t *list, char *buffer);


/**
 * @brief Print whole list to stdout from first to last and clean it
 * 
 * @param list list to be printed and cleaned
 */
void list_print_clean(list_t *list);


#endif
////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////