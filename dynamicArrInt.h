/**CFile****************************************************************

  FileName    [dynamicArrInt.h]

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
#ifndef DYNAMICARRINT_H
#define DYNAMICARRINT_H

#include <stdbool.h>


////////////////////////////////////////////////////////////////////////
///                         BASIC TYPES                              ///
////////////////////////////////////////////////////////////////////////
typedef struct
{
    int *arr; 
    unsigned int length; // lenght of arr
    unsigned int max; // maximal length of arr
} dynamicArrInt_t;

////////////////////////////////////////////////////////////////////////
///                      MACRO DEFINITIONS                           ///
////////////////////////////////////////////////////////////////////////
#define DYNAMICARRINT_LEN 500 //Initializing length of structure
#define MULT_LEN 2  // in case of realloc multiply a->max by MULT_LEN

////////////////////////////////////////////////////////////////////////
///                    FUNCTION DECLARATIONS                         ///
////////////////////////////////////////////////////////////////////////

/**
 * @brief Initialize array of integers
 * 
 * @param a array of integers to be initialized
 * @return True if succesfull, otherwise false
 */
bool dynamicArrInt_init(dynamicArrInt_t *a);

/**
 * @brief Free array of integers
 * 
 * @param a structure to be cleared
 */
void dynamicArrInt_free(dynamicArrInt_t *a);

/**
 * @brief Add a new int to array
 * 
 * @param a array of integers to be added
 * @param n int value to be added
 * @return true if was succesful, otherwise false
 */
bool dynamicArrInt_add(dynamicArrInt_t *a, int n);



#endif
////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////