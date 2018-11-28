/**CFile****************************************************************

  FileName    [list.c]

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
#include "list.h"
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>

////////////////////////////////////////////////////////////////////////
///                       GLOBAL VARIABLES                           ///
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
////////////////////////////////////////////////////////////////////////


bool print_or_add(list_t *list, bool add, const char *fmt, ...)
{
    va_list arg;
    va_start(arg, fmt);

    if (add == true)
    {
        size_t needed = vsnprintf(NULL, 0, fmt, arg) + 1;
        char *buffer = malloc(needed * sizeof(char));
        if (buffer == NULL)
        {
            va_end(arg);
            return false;
        }
        va_end(arg);

        va_start(arg, fmt);
        vsprintf(buffer, fmt, arg); 

        if (! list_append(list, buffer))
            return false;
    }
    else
    {
        vprintf(fmt, arg);
    }

    va_end(arg);
    return true;
}


list_t *list_create()
{
    list_t *list = malloc(sizeof(list_t));
    if (list == NULL)
        return NULL;
    
    list->last = NULL;
    list->first = NULL;
    return list;
}


void list_clean(list_t *list) 
{
    list_elem_t *tmp = list->first;
    list_elem_t *tmp_del;
    while (tmp != NULL)
    {
        tmp_del = tmp;
        tmp = tmp->next;
        free(tmp_del->string);
        free(tmp_del); 
    }

    list->last = NULL;
    list->first = NULL;
}


void list_destroy(list_t *list)
{
    list_clean(list);
    free(list);
}


bool list_append(list_t *list, char *buffer)
{
    if (buffer == NULL)
        return false;

    list_elem_t *new_elem = malloc(sizeof(list_elem_t));
    if (new_elem == NULL)
        return false;
    new_elem->string = buffer;
    new_elem->next = NULL;

    if (list->last != NULL)
        list->last->next = new_elem;

    if (list->first == NULL)
        list->first = new_elem; 

    list->last = new_elem;
    return true;
}


void list_print_clean(list_t *list)
{
    list_elem_t *tmp = list->first;
    list_elem_t *tmp_del;
    while (tmp != NULL)
    {
        printf("%s", tmp->string);

        tmp_del = tmp;
        tmp = tmp->next;
        free(tmp_del->string);
        free(tmp_del); 
    }

    list->last = NULL;
    list->first = NULL;
}

#if 0
int main()
{
    list_t *list = list_create();

    for (int i = 0; i < 10; i++)
    {
        print_or_add(list, true, "ok ta ty si frajer %s %d\n", "fkc", i);
    }

    list_print_clean(list);
    print_or_add(list, true, "ok ta ty si frajer %s %d\n", "fkc", 99);
    list_print_clean(list);
    list_print_clean(list);
    list_destroy(list);
    return 0;
}
#endif

////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////

