/**CFile****************************************************************

  FileName    [symtable.c]

  SystemName  [IFJ - PROJECT]

  PackageName [Symbol table]

  Synopsis    [Table operations]

  Author      [Lukas Valek]

  Login       [xvalek15]

  Affiliation []

  Date        [20/10/2018]

  Revision    []

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                          INCLUDES                                ///
////////////////////////////////////////////////////////////////////////

#include "symtable.h"
#include "dynamic_arr_param.h"

////////////////////////////////////////////////////////////////////////
///                       GLOBAL VARIABLES                           ///
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
////////////////////////////////////////////////////////////////////////



symtable_t * symtab_init(char *name, table_type_t type)
{
    symtable_t *symtab = (symtable_t*)malloc(sizeof(symtable_t) + SYMTAB_SIZE * sizeof(elem_t *));
    if (symtab == NULL)
    {
        return NULL;
    }
    if (name == NULL)
    {
        symtab->name = NULL;
    }
    else
    {
        symtab->name = malloc((strlen(name) + 1) * sizeof(char));
        if (symtab->name == NULL)
        {
            free(symtab);
            return NULL;
        }    
        strcpy(symtab->name, name);
    }
    symtab->arr_size = SYMTAB_SIZE;
    symtab->size = 0;

    for (size_t i = 0; i < symtab->arr_size; ++i)
    {
        symtab->ptr[i] = NULL;
    }
    if (type == VARIABLES)
    {
        symtab->type = VARIABLES;
    }
    else if (type == FUNCTIONS)
    {
        symtab->type = FUNCTIONS;
    }
    else
    {
        fprintf(stderr, "Incorrent symtable type!\n");
    }
    return symtab;
}

elem_t *symtab_elem_add(symtable_t *symtab, char *key)
{

    if (symtab == NULL)
    {
        return NULL;
    }
    size_t index = symtab_hash_function(key) % symtab_bucket_count(symtab);
    elem_t *elem = symtab_find(symtab, key);

    if (elem)
    {
        return elem;
    }

    elem = (elem_t*)malloc(sizeof(elem_t));
    if (elem == NULL)
    {
        return NULL;
    }
    if (symtab->type == VARIABLES)
    {
        elem->var.key = malloc(sizeof(char) * (strlen(key) + 1));
        if (elem->var.key == NULL)
        {
            free(elem);
            return NULL;
        }
        strcpy(elem->var.key, key);
    }
    else if (symtab->type == FUNCTIONS)
    {
        elem->func.key = malloc(sizeof(char) * (strlen(key) + 1));
        if (elem->func.key == NULL)
        {
            free(elem);
            return NULL;
        }
        strcpy(elem->func.key, key);
        elem->func.is_defined = false;
        elem->func.n_params = UNDEF_NO_PARAMS;
    }

    elem->next = NULL;
    symtab->size = symtab->size + 1;

    if (symtab->ptr[index] == NULL)
    {
        symtab->ptr[index] = elem;
    }
    else
    {
        for (elem_t *tmp = symtab->ptr[index]; tmp; tmp = tmp->next)
        {
            if (tmp->next == NULL)
            {
                tmp->next = elem;
                return elem;
            }
        }
    }
    return elem;
}

void symtab_update(symtable_t *symtab, bool is_defined, char *key)
{
    if (symtab == NULL)
    {
        fprintf(stderr, "Pointer to the symtable is NULL\n");
    }

    if (symtab->type == FUNCTIONS)
    {
        elem_t *tmp = symtab_find(symtab, key);
        if (tmp == NULL)
        {
            fprintf(stderr, "Element with the given key was not found!\n");
            return;
        }
        tmp->func.is_defined = is_defined;
    }

    return;
}
size_t symtab_bucket_count(symtable_t *symtab)
{
    if (!symtab)
    {
        return 0;
    }

    return symtab->arr_size;
}


void symtab_clear(symtable_t *symtab)
{
    if (!symtab)
    {
        return;
    }

    elem_t *elem;
    elem_t *next;

    for (size_t i = 0; i < symtab_bucket_count(symtab); ++i)
    {
        if (symtab->ptr[i])
        {
            elem = symtab->ptr[i];
            while (elem)
            {
                next = elem->next;
                if (symtab->type == VARIABLES)
                {
                    free(elem->var.key);
                }
                else if (symtab->type == FUNCTIONS)
                {
                    free(elem->func.key);
                }
                free(elem);
                elem = next;
                symtab->size--;
            }
            symtab->ptr[i] = NULL;
        }

    }
    return;
}

elem_t *symtab_find(symtable_t *symtab, const char *key)
{
    unsigned int index = symtab_hash_function(key) % symtab_bucket_count(symtab);
    elem_t *elem = symtab->ptr[index];
    if (symtab->type == VARIABLES)
    {
        while (elem != NULL)
        {
            if (strcmp(elem->var.key, key) == 0)
            {
                return elem;
            }
            elem = elem->next;
        }
    }
    else if (symtab->type == FUNCTIONS)
    {
        while (elem != NULL)
        {
            if (strcmp(elem->func.key, key) == 0)
            {
                return elem;
            }
            elem = elem->next;
        }
    }
    return NULL;
}

bool symtab_foreach(symtable_t *symtab, bool (*func) (elem_t *elem))
{
    if (!symtab)
    {
        return false;
    }

    for (size_t i = 0; i < symtab_bucket_count(symtab); ++i)
    {
        for (elem_t *elem = symtab->ptr[i]; elem != NULL ; elem = elem->next)
        {
            if ( ! func(elem))
                return false;
        }
    }
    return true;
}

void symtab_free(symtable_t *symtab)
{
    if (symtab == NULL)
        return;

    symtab_clear(symtab);
    if (symtab->name != NULL)
        free(symtab->name);
    free(symtab);
}

size_t symtab_hash_function(const char *str)
{
    size_t h = 0;   // 32bit
    const unsigned char *p;
    for (p = (const unsigned char*)str; *p != '\0'; p++)
        h = 65599 * h + *p;
    return h;
}


bool symtab_remove(symtable_t *symtab, const char *key)
{
    if (!symtab)
    {
        return false;
    }

    elem_t *elem = symtab_find(symtab, key);
    if (elem == NULL)
    {
        return false;
    }


    size_t index = (symtab_hash_function(key) % symtab_bucket_count(symtab));

    if (symtab->ptr[index] == elem)
    {
        if (elem->next == NULL)
        {
            if (symtab->type == VARIABLES)
            {
                free(elem->var.key);
            }
            else if (symtab->type == FUNCTIONS)
            {
                free(elem->func.key);
            }
            free(elem);
            symtab->size--;
            symtab->ptr[index] = NULL;
        }
        else
        {
            symtab->ptr[index] = elem->next;
            if (symtab->type == VARIABLES)
            {
                free(elem->var.key);
            }
            else if (symtab->type == FUNCTIONS)
            {
                free(elem->func.key);
            }
            free(elem);
            symtab->size--;
        }

    }
    else if (symtab->ptr[index] == NULL)
        return false;
    else
    {
        for (elem_t *tmp = symtab->ptr[index]; tmp != NULL; tmp = tmp->next)
        {
            if (tmp->next == elem)
            {
                if (tmp->next->next == NULL)
                {
                    if (symtab->type == VARIABLES)
                    {
                        free(elem->var.key);
                    }
                    else if (symtab->type == FUNCTIONS)
                    {
                        free(elem->func.key);
                    }
                    free(elem);
                    symtab->size--;
                    tmp->next = NULL;
                    return true;
                }
                else
                {
                    tmp->next = tmp->next->next;
                    if (symtab->type == VARIABLES)
                    {
                        free(elem->var.key);
                    }
                    else if (symtab->type == FUNCTIONS)
                    {
                        free(elem->func.key);
                    }
                    free(elem);
                    symtab->size--;
                    return true;
                }
            }
        }
    }

    return true;
}


size_t symtab_size(symtable_t *symtab)
{
    if (!symtab)
    {
        return 0;
    }

    return symtab->size;
}

////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////

