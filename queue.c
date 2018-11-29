/**CFile****************************************************************

  FileName    [queue.c]

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
    if (queue == NULL)
        return NULL;

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


bool que_up(queue_t *queue, token_t *token, char *name)
{
	if (que_full(queue))
	    return false;

    que_elem_t element;
    element.token = token;
    element.name = malloc((strlen(name) + 1)  * sizeof(char));
    if (element.name == NULL)
        return false;
    strcpy(element.name, name);

	// add to b_index
	queue->array[queue->b_index] = element;
	// shifts b_index
	queue->b_index = nextIndex(queue->b_index);
    
    return true;
}


token_t *que_get(queue_t *queue, char **name)
{
	if (que_empty(queue))
        return NULL;

    token_t *token = queue->array[queue->f_index].token;
    *name = queue->array[queue->f_index].name;

	queue->f_index = nextIndex(queue->f_index);

    return token;
}


void que_destroy(queue_t *queue)
{
    while (queue->f_index != queue->b_index)
    {
        free(queue->array[queue->f_index].name);
        destroyToken(queue->array[queue->f_index].token);
        queue->f_index = nextIndex(queue->f_index);
    }
    
    free(queue);
}


////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////

