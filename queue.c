/**CFile****************************************************************

  FileName    []

  SystemName  [IFJ - PROJECT]

  PackageName []

  Synopsis    []

  Author      [Adam Pankuch]

  Affiliation []

  Date        [1/11/2018]

  Revision    []

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                          INCLUDES                                ///
////////////////////////////////////////////////////////////////////////
#include <stdlib.h>
#include <stdbool.h>

#include "queue.h"

////////////////////////////////////////////////////////////////////////
///                       GLOBAL VARIABLES                           ///
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
////////////////////////////////////////////////////////////////////////
queue_t *que_create()
{
    queue_t *queue = malloc(sizeof(queue_t));
    
    for (int i = 0; i < QUEUE_SIZE; i++)
        queue->array[i] = NULL;

    queue->f_index = 0;
    queue->b_index = 0;

    return queue;
}


int nextIndex(int index)
{
	return ((index + 1) % QUEUE_SIZE); 
}


int que_empty(queue_t* queue)
{
	return (queue->f_index == queue->b_index);
}


int que_full(queue_t* queue)
{
	return (nextIndex(queue->b_index) == queue->f_index);
}


bool que_up(queue_t *queue, token_t *token)
{
	if (que_full(queue))
	    return false;

	// add to b_index
	queue->array[queue->b_index] = token;
	// shifts b_index
	queue->b_index = nextIndex(queue->b_index);
    
    return true;
}


token_t *que_get(queue_t *queue)
{
	if (que_empty(queue))
        return NULL;

    token_t *token = queue->array[queue->f_index];
	queue->f_index = nextIndex(queue->f_index);

    return token;
}


void que_destroy(queue_t *queue)
{
    free(queue);
}





////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////

