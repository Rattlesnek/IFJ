/**CFile****************************************************************

  FileName    [list.h]

  SystemName  [IFJ - PROJECT]

  PackageName []

  Synopsis    []

  Author      [Adam Pankuch]

  Affiliation []

  Date        [--/--/2018]

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


typedef struct list_element {
    char *string;
    struct list_element *next;
} list_elem_t;


typedef struct {
    list_elem_t *last;
    list_elem_t *first;
} list_t;


////////////////////////////////////////////////////////////////////////
///                      MACRO DEFINITIONS                           ///
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
///                    FUNCTION DECLARATIONS                         ///
////////////////////////////////////////////////////////////////////////

bool print_or_append(list_t *list, bool append, const char *fmt, ...);


list_t *list_create();


void list_clean(list_t *list);


void list_destroy(list_t *list);


bool list_append(list_t *list, char *buffer);


void list_print_clean(list_t *list);




#endif
////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////