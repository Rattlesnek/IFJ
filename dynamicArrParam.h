/**CFile****************************************************************

  FileName    [dynamicparamsInt.h]

  SystemName  [IFJ - PROJECT]

  PackageName [Data type]

  Synopsis    [Dynamic params of integers implementation]

  Author      [Adam Pankuch, Lukas Valek]

  Affiliation []

  Date        [26/10/2018]

  Revision    []

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                          INCLUDES                                ///
////////////////////////////////////////////////////////////////////////
#ifndef DYNAMICARRPARAM_H
#define DYNAMICARRPARAM_H

#include <stdlib.h>
#include <stdbool.h>
#include "symtable.h"


////////////////////////////////////////////////////////////////////////
///                         BASIC TYPES                              ///
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
///                      MACRO DEFINITIONS                           ///
////////////////////////////////////////////////////////////////////////
#define DYNAMICARRPARAM_LEN 4 //Initializing length of structure
#define MULT_LEN 4  // in case of realloc multiply a->max by MULT_LEN

////////////////////////////////////////////////////////////////////////
///                    FUNCTION DECLARATIONS                         ///
////////////////////////////////////////////////////////////////////////

/**
 * @brief Initialize params of integers
 *
 * @param a params of integers to be initialized
 * @return True if succesfull, otherwise false
 */
dynamicArrParams_t *dynamicArrParams_init();

/**
 * @brief Free params of integers
 *
 * @param a structure to be cleared
 */
void dynamicArrParams_free(dynamicArrParams_t *a);

/**
 * @brief Add a new int to params
 *
 * @param a params of integers to be added
 * @param n int value to be added
 * @return true if was succesful, otherwise false
 */
bool dynamicArrparams_add(dynamicArrParams_t *a, elem_t *param);



#endif
////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////