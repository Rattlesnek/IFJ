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

char *sprintf_alloc(const char *fmt, ...)
{
    va_list arg;

    va_start(arg, fmt);
    size_t needed = vsnprintf(NULL, 0, fmt, arg) + 1;
    char *buffer = malloc(needed * sizeof(char));
    if (buffer == NULL)
    {
        va_end(arg);
        return NULL;
    }
    va_end(arg);

    va_start(arg, fmt);
    vsprintf(buffer, fmt, arg); 
    va_end(arg);

    return buffer;
}


list_t *list_create()
{
    list_t *L = malloc(sizeof(list_t));
    if (L == NULL)
        return NULL;
    
    L->act = NULL;
    L->first = NULL;
    return L;
}


void list_clean(list_t *L) 
{
    list_elem_t *tmp = L->first;
    list_elem_t *tmp_del;
    while (tmp != NULL)
    {
        tmp_del = tmp;
        tmp = tmp->next;
        free(tmp_del->string);
        free(tmp_del); 
    }

    L->act = NULL;
    L->first = NULL;
}


void list_destroy(list_t *L)
{
    list_clean(L);
    free(L);
}


bool list_insertFirst (list_t *L, char *buffer)
{
    if (buffer == NULL)
        return false;

    list_elem_t *new_elem = malloc(sizeof(list_elem_t));
    if (new_elem == NULL)
        return false;

    // initialize new element
    new_elem->string = buffer;
    // insert first
    new_elem->next = L->first;
    L->first = new_elem; 
    return true;
}


void list_actFirst(list_t *L)
{
/*
** Nastaví aktivitu seznamu L na jeho první prvek.
** Funkci implementujte jako jediný příkaz, aniž byste testovali,
** zda je seznam L prázdný.
**/
	L->act = L->first;
}


bool list_insertPost (list_t *L, char *buffer) {
/*
** Vloží prvek s hodnotou val za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje!
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** zavolá funkci Error().
**/    
    if (L->act == NULL || buffer == NULL)
        return false;
    
    list_elem_t *new_elem = malloc(sizeof(list_elem_t));
    if (new_elem == NULL)
        return false;

    // initialize new element
    new_elem->string = buffer;
    
    // post insert
    new_elem->next = L->act->next;
    L->act->next = new_elem;
    return true;
}


void list_actSucc (list_t *L)
{
	if (L->act == NULL)
        return;
    
    L->act = L->act->next;
}


int list_isActive(list_t *L)
{
	return (L->act != NULL);
}


void print_from_last(list_elem_t *element)
{
    if (element != NULL)
    {
        print_from_last(element->next);
    
        printf("%s", element->string);
    }
    //free(element->string);
    //free(element);
}



#if 0
int main()
{
    list_t *list = list_create();
    char *buffer;

    for (int i = 0; i < 10; i++)
    {
        buffer = sprintf_alloc("%s %d\n", "ahoj retard", i);
        if (buffer == NULL)
            return 1;
        
        if (! list_insertFirst(list, buffer))
            return 1;
    }
    list_destroy(list);
    list = list_create();
    print_from_last(list->first);

    list_destroy(list);
    return 0;
}
#endif

////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////

