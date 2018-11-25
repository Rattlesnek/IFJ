/**CFile****************************************************************

  FileName    [dynamicStr.c]

  SystemName  [IFJ - PROJECT]

  PackageName [Data type]

  Synopsis    [Dynamic string implementation]

  Author      [Jindrich Sestak]

  Affiliation []

  Date        [27/10/2018]

  Revision    []

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                          INCLUDES                                ///
////////////////////////////////////////////////////////////////////////
#include <stdlib.h>
#include <string.h>

#include "dynamic_str.h"

////////////////////////////////////////////////////////////////////////
///                       GLOBAL VARIABLES                           ///
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
////////////////////////////////////////////////////////////////////////


void dynamicStr_clear(dynamicStr_t *s)
{
	s->length = 0;
	s->str[0] = '\0';
}


bool dynamicStr_init(dynamicStr_t *s)
{
	s->str = malloc(DYNAMICSTR_LEN * sizeof(char));
	if (s->str == NULL)
		return false;

	dynamicStr_clear(s);
	s->max = DYNAMICSTR_LEN;

	return true;
}


void dynamicStr_free(dynamicStr_t *s)
{
	free(s->str);

	s->str = NULL;
	s->length = 0;
	s->max = 0;
}


bool dynamicStr_add(dynamicStr_t *s, char c)
{
	if (s->length + 1 >= s->max)
	{
		unsigned int new_size = s->length + DYNAMICSTR_LEN;
		s->str = realloc(s->str, new_size * sizeof(char));
		if (s->str == NULL)
			return false;

		s->max = new_size;
	}

	s->str[s->length++] = c;
	s->str[s->length] = '\0';

	return true;
}



////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////
