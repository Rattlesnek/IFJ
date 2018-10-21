/**CFile****************************************************************

  FileName    []

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
#include <stdlib.h>
#include <string.h>

#include "dynamicStr.h"

////////////////////////////////////////////////////////////////////////
///                       GLOBAL VARIABLES                           ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
////////////////////////////////////////////////////////////////////////

/**
 * @brief Clearing string
 * 
 * @param s 
 */
void dynamicStr_clear(dynamicStr_t *s)
{
	s->length = 0;
	s->str[s->length] = '\0';
}

/**
 * @brief initialize
 * 
 * @param s 
 * @return true If succesful, otherwise false 
 */
bool dynamicStr_init(dynamicStr_t *s)
{
	if (!(s->str = (char *) malloc(DYNAMICSTR_LEN)))
	{
		return false;
	}

	dynamicStr_clear(s);
	s->max = DYNAMICSTR_LEN;

	return true;
}

void dynamicStr_free(dynamicStr_t *s)
{
	free(s->str);
}

bool dynamicStr_add(dynamicStr_t *s, char c)
{
	if (s->length + 1 >= s->max)
	{
		unsigned int new_size = s->length + DYNAMICSTR_LEN;
		if (!(s->str = (char *) realloc(s->str, new_size)))
		{
			return false;
		}
		s->max = new_size;
	}

	s->str[s->length++] = c;
	s->str[s->length] = '\0';

	return true;
}



////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////
