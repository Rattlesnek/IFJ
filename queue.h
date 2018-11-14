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
#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>

#include "token.h"

////////////////////////////////////////////////////////////////////////
///                         BASIC TYPES                              ///
////////////////////////////////////////////////////////////////////////

// QUEUE_SIZE has to be 3 so that max nuber of elements in it is 2
#define QUEUE_SIZE 3

typedef struct que_elem {
    token_t *token;
    char *name;
} que_elem_t;


typedef struct queue {
    int f_index;            // Index of first element in queue 
    int b_index;            // Index of last element in queue                
    que_elem_t array[QUEUE_SIZE];    // Array of pointers to elements of queue   
} queue_t;

////////////////////////////////////////////////////////////////////////
///                      MACRO DEFINITIONS                           ///
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
///                    FUNCTION DECLARATIONS                         ///
////////////////////////////////////////////////////////////////////////
queue_t *que_create();


int nextIndex(int index);


int que_empty(queue_t* queue);


int que_full(queue_t* queue);


bool que_up(queue_t *queue, token_t *token, char *name);


token_t *que_get(queue_t *queue, char **name);


void que_destroy(queue_t *queue);



#endif
////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////