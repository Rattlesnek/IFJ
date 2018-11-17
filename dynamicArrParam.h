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
#define MULT_LEN 4  // in case of realloc add a->max and MULT_LEN

////////////////////////////////////////////////////////////////////////
///                    FUNCTION DECLARATIONS                         ///
////////////////////////////////////////////////////////////////////////


/**
 * @brief      Initialize the array of pointers
 *
 * @return     True     if succesfull, otherwise false
 */
dynamicArrParams_t *dynamicArrParams_init();


/**
 * @brief      Free array of params
 *
 * @param      a     structure to be freed
 */
void dynamicArrParams_free(dynamicArrParams_t *a);


/**
 * @brief      Add a new param ptr to the array
 *
 * @param      a      ptr to the structure of param pointers
 * @param      param  element ptr to be added
 *
 * @return     true if was succesful, otherwise false
 */
bool dynamicArrParams_add(dynamicArrParams_t *a, elem_t *param);



#endif
////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////