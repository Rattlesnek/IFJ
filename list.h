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
    list_elem_t *act;
    list_elem_t *first;
} list_t;


////////////////////////////////////////////////////////////////////////
///                      MACRO DEFINITIONS                           ///
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
///                    FUNCTION DECLARATIONS                         ///
////////////////////////////////////////////////////////////////////////

char *sprintf_alloc(const char *fmt, ...);

list_t *list_create();

void print_from_last(list_elem_t *element);


void list_clean(list_t *L);


void list_destroy(list_t *L);


bool list_insertFirst (list_t *L, char *buffer);


void list_actFirst(list_t *L);


bool list_insertPost (list_t *L, char *buffer);


void list_actSucc (list_t *L);


int list_isActive(list_t *L);






#endif
////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////