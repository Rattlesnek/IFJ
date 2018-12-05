/**CFile****************************************************************

  FileName    [error.h]

  SystemName  [IFJ - PROJECT]

  PackageName [Error]

  Synopsis    [Return values]

  Author      [Adam Pankuch, Lukas Piwowarski]

  Affiliation [xpanku00, xpiwow00]

  Date        [29/10/2018]

  Revision    []

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                          INCLUDES                                ///
////////////////////////////////////////////////////////////////////////
#ifndef ERROR_H
#define ERROR_H

#include <stdarg.h>

////////////////////////////////////////////////////////////////////////
///                         BASIC TYPES                              ///
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
///                      MACRO DEFINITIONS                           ///
////////////////////////////////////////////////////////////////////////

#define SUCCESS 0
#define ERR_LEX 1
#define ERR_SYN 2
#define ERR_SEM_UNDEF 3
#define ERR_SEM_TYPE 4
#define ERR_SEM_FUNC 5
#define ERR_SEM_OTHER 6
#define ERR_ZERO_DIV 9
#define ERR_INTERNAL 99

////////////////////////////////////////////////////////////////////////
///                    FUNCTION DECLARATIONS                         ///
////////////////////////////////////////////////////////////////////////

/**
 * @brief Print error message to stderr and fflush
 * 
 * @param fmt format string
 * @param ... arguments
 */
void error_msg(const char *fmt, ...);


/**
 * @brief Print to stdout and fflush
 * 
 * @param fmt format string
 * @param ... aruments
 */
void print_flush(const char *fmt, ...);


#endif
////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////