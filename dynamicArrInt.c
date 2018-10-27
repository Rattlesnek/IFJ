/**CFile****************************************************************

  FileName    [dynamicArrInt.c]

  SystemName  [IFJ - PROJECT]

  PackageName [Data type]

  Synopsis    []

  Author      [Adam Pankuch, Jindrich Sestak]

  Affiliation []

  Date        [26/10/2018]

  Revision    []

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                          INCLUDES                                ///
////////////////////////////////////////////////////////////////////////
#include <stdlib.h>

#include "dynamicArrInt.h"

////////////////////////////////////////////////////////////////////////
///                       GLOBAL VARIABLES                           ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
////////////////////////////////////////////////////////////////////////

bool dynamicArrInt_init(dynamicArrInt_t *a)
{
	a->arr = malloc(DYNAMICARRINT_LEN * sizeof(int));
    if (a->arr == NULL)
        return false;
    
    a->length = 0;
    a->max = DYNAMICARRINT_LEN;

    return true;
}


void dynamicArrInt_free(dynamicArrInt_t *a)
{
	free(a->arr);
}


bool dynamicArrInt_add(dynamicArrInt_t *a, int n)
{
	if (a->length >= a->max)
	{
		unsigned int new_size = a->length * MULT_LEN;
		a->arr = realloc(a->arr, new_size * sizeof(int));
        if (a->arr == NULL)
			return false;

		a->max = new_size;
	}

	a->arr[a->length++] = n;

	return true;
}



////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////
