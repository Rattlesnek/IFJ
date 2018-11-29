/**CFile****************************************************************

  FileName    [queue.h]

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
#define QUEUE_SIZE 5 // to be sure!


/**
 * @brief Element of queue
 * 
 */
typedef struct que_elem {
    token_t *token; // token stored in queue
    char *name;     // name stored in queue
} que_elem_t;


/**
 * @brief Queue of tokens and names -- structure
 * 
 */
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

/**
 * @brief Creates queue
 * 
 * @return queue_t* if success
 *         NULL     if internal error
 */
queue_t *que_create();


/**
 * @brief Calculate next index
 * 
 * @param index current index
 * @return int  next index
 */
int nextIndex(int index);


/**
 * @brief Is queue empty?
 * 
 * @param queue queue of tokens and names
 * @return int  1 if queue is empy
 *              0 if not
 */
int que_empty(queue_t* queue);


/**
 * @brief Is queue full?
 * 
 * @param queue queue of tokens and names
 * @return int  1 if queue is full
 *              0 if not
 */
int que_full(queue_t* queue);


/**
 * @brief Add to queue
 * 
 * @param queue     queue of tokens and names
 * @param token     token to be added
 * @param name      name to be added
 * @return true     if success
 * @return false    if internal error
 */
bool que_up(queue_t *queue, token_t *token, char *name);


/**
 * @brief Get from queue
 * 
 * @param queue     queue of tokens and names
 * @param name      get name from queue
 * @return token_t* get token from queue
 *                  NULL if queue empty
 */
token_t *que_get(queue_t *queue, char **name);


/**
 * @brief Destroy queue
 * 
 * @param queue queue of tokens and names
 */
void que_destroy(queue_t *queue);


#endif
////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////