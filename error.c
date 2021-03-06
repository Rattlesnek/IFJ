/**CFile****************************************************************

  FileName    [error.c]

  SystemName  [IFJ - PROJECT]

  PackageName [Error]

  Synopsis    [Error messages]

  Author      [Adam Pankuch]

  Login       [xpanku00]

  Affiliation []

  Date        [29/10/2018]

  Revision    []

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                          INCLUDES                                ///
////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "error.h"


////////////////////////////////////////////////////////////////////////
///                       GLOBAL VARIABLES                           ///
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
////////////////////////////////////////////////////////////////////////


void print_flush(const char *fmt, ...)
{
    va_list arg;

    va_start(arg, fmt);
    vfprintf(stdout, fmt, arg);
    va_end(arg);

    fflush(stdout);
}


void error_msg(const char *fmt, ...)
{
    va_list arg;

    fprintf(stderr, "ERROR: ");

    va_start(arg, fmt);
    vfprintf(stderr, fmt, arg);
    va_end(arg);

}

////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////
