/**CFile****************************************************************

  FileName    [dynamicStr.h]

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
#ifndef DYNAMICSTR_H
#define DYNAMICSTR_H

#include <stdbool.h>


////////////////////////////////////////////////////////////////////////
///                         BASIC TYPES                              ///
////////////////////////////////////////////////////////////////////////
typedef struct
{
	char *str; /// string ened by '\0' byte
	unsigned int length; /// lenght of string
  unsigned int max; //Maximal length od string
} dynamicStr_t;

////////////////////////////////////////////////////////////////////////
///                      MACRO DEFINITIONS                           ///
////////////////////////////////////////////////////////////////////////
#define DYNAMICSTR_LEN 10 //Initializing length of structure

////////////////////////////////////////////////////////////////////////
///                    FUNCTION DECLARATIONS                         ///
////////////////////////////////////////////////////////////////////////

/**
 * @brief Initializing structure
 * 
 * @param s Dynamic string to be initialized
 * @return True if succesfull, otherwise false
 */
bool dynamicStr_init(dynamicStr_t *s);

/**
 * @brief Clear string
 * 
 * @param s Dynamic string
 */
void dynamicStr_clear(dynamicStr_t *s);

/**
 * @brief Freeing space after structure
 * 
 * @param s Structure to be cleared
 */
void dynamicStr_free(dynamicStr_t *s);

/**
 * @brief Adding a new char to array of strings
 * 
 * @param s Array to add
 * @param c New character
 * @return true if was succesful, otherwise false
 */
bool dynamicStr_add(dynamicStr_t *s, char c);



#endif
////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////