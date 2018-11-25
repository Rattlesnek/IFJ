/**CFile****************************************************************

  FileName    [main.c]

  SystemName  [IFJ - PROJECT]

  PackageName []

  Synopsis    []

  Author      [Adam Pankuch]

  Affiliation []

  Date        [25/11/2018]

  Revision    []

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                          INCLUDES                                ///
////////////////////////////////////////////////////////////////////////

#include "dynamicStr.h"
#include "queue.h"
#include "parser.h"
#include "error.h"

////////////////////////////////////////////////////////////////////////
///                       GLOBAL VARIABLES                           ///
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
///                            MAIN                                  ///
////////////////////////////////////////////////////////////////////////

int main()
{
    dynamicStr_t str;
    dynamicStr_t *sc_str = &str;
    
    if (! dynamicStr_init(sc_str))
        return ERR_INTERNAL;
    queue_t *que = que_create();
    if (que == NULL)
    {
        dynamicStr_free(sc_str);
        return ERR_INTERNAL;
    }


    int ret = parser(sc_str, que);


    dynamicStr_free(sc_str);
    que_destroy(que);
    return ret;
}

////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////



