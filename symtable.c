/**CFile****************************************************************

  FileName    [symtable.c]

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

#include "symtable.h"

////////////////////////////////////////////////////////////////////////
///                       GLOBAL VARIABLES                           ///
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
////////////////////////////////////////////////////////////////////////


	
symtable_t * symtab_init(size_t size)
{
	symtable_t *symtab = (symtable_t*)malloc(sizeof(symtable_t) + size*sizeof(elem_t));
	if (symtab == NULL)
	{
		return NULL;
	}

	symtab->arr_size = size;
	symtab->size = 0;

	for (size_t i = 0; i < symtab->arr_size; ++i)
	{
		symtab->ptr[i] = NULL;
	}

	return symtab;
}

elem_t *symtab_elem_add(symtable_t *symtab, char *name /*(ID, FUNC)*/, char *key /*(realne jmeno promenne).*/) 
{

	if (!symtab)
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
	if (!elem)
	{
		return NULL;
	}
	elem->key = (char*)malloc(sizeof(char) * strlen(key) + 1);
	if (!elem->key)
	{	
		free(elem);
		return NULL;
	}	
	strcpy(elem->key, key);
	if (strcmp(name, "ID") == 0)
	{
		elem->token_type = ID;
		elem->value_type = UNKNOWN;
	}
	else if (strcmp(key, "FUNC") == 0)
	{
		elem->token_type = FUNC;
	}
	elem->data.intg = 0;
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
			while(elem)
			{
				next = elem->next;
				if (elem->value_type == STR)
				{
					free(elem->data.string);
				}
				free(elem->key);
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
	while(elem != NULL)
	{
		if (strcmp(elem->key, key) == 0)
		{
			return elem;
		}
		elem = elem->next;
	}
	return NULL;
}

void symtab_foreach(symtable_t *symtab, void (*func) (elem_t *elem))
{
	if (!symtab)
	{
		return;
	}

	for (size_t i = 0; i < symtab_bucket_count(symtab); ++i)
	{
		for (elem_t *elem = symtab->ptr[i]; elem != NULL ; elem = elem->next)
		{
			func(elem);
		}
	}
}

void symtab_free(symtable_t *symtab)
{
	if (!symtab)
	{
		return;
	}

	symtab_clear(symtab);
	free(symtab);
}

size_t symtab_hash_function(const char *str) 
	{ 
		size_t h=0; 	// 32bit
		const unsigned char *p; 
		for(p=(const unsigned char*)str; *p!='\0'; p++) 
		h = 65599*h + *p; 
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

	
	size_t index = (symtab_hash_function(key)%symtab_bucket_count(symtab));

	if (symtab->ptr[index] == elem)
	{
		if (elem->next == NULL)
		{
			free(elem->key);
			free(elem);
			symtab->size--;
			symtab->ptr[index] = NULL;
		}
		else
		{
			symtab->ptr[index] = elem->next;
			free(elem->key);
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
					free(elem->key);
					free(elem);
					symtab->size--;
					tmp->next = NULL;
					return true;
				}
				else
				{
					tmp->next = tmp->next->next;
					free(elem->key);
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

