/**CFile****************************************************************

  FileName    [main.c]

  SystemName  [IFJ - PROJECT]

  PackageName [Compiler]

  Synopsis    [Main function -- calls Parser]

  Author      [Adam Pankuch]

  Login       [xpanku00]

  Affiliation []

  Date        [25/11/2018]

  Revision    []

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                          INCLUDES                                ///
////////////////////////////////////////////////////////////////////////

#include "parser.h"

#include "error.h"

#include "dynamic_str.h"
#include "queue.h"
#include "list.h"
#include "stack_str.h"
#include "stack_tkn.h"


////////////////////////////////////////////////////////////////////////
///                       GLOBAL VARIABLES                           ///
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
///                            MAIN                                  ///
////////////////////////////////////////////////////////////////////////

int main()
{
    /// INIT STRUCTURES
    stack_tkn_t *stack_tkn = stcTkn_create();
    if (stack_tkn == NULL)
        return ERR_INTERNAL;
    stack_str_t *stack_str = stcStr_create();
    if (stack_str == NULL)
        goto err_internal_1;
    
    dynamicStr_t str;
    dynamicStr_t *sc_str = &str;
    if (! dynamicStr_init(sc_str))
        goto err_internal_2;
    queue_t *que = que_create();
    if (que == NULL)
        goto err_internal_3;

    list_t *code_buffer = list_create();
    if (code_buffer == NULL)
        goto err_internal_4;
    list_t *defvar_buffer = list_create();
    if (defvar_buffer == NULL)
        goto err_internal_5;

    // RUN COMPILER
    int ret = parser(stack_tkn, stack_str, code_buffer, defvar_buffer, sc_str, que);

    // CLEAN ALL
    stcTkn_destroy(stack_tkn);
    stcStr_destroy(stack_str);
    dynamicStr_free(sc_str);
    que_destroy(que);
    list_destroy(code_buffer);
    list_destroy(defvar_buffer);
    return ret;
    
////////////////////////////////////////////////////////////////////////
///                       ERROR HANDLING                             ///
////////////////////////////////////////////////////////////////////////
err_internal_1:
    stcTkn_destroy(stack_tkn);
    return ERR_INTERNAL;

err_internal_2:
    stcTkn_destroy(stack_tkn);
    stcStr_destroy(stack_str);
    return ERR_INTERNAL;

err_internal_3:
    stcTkn_destroy(stack_tkn);
    stcStr_destroy(stack_str);
    dynamicStr_free(sc_str);
    return ERR_INTERNAL;

err_internal_4:
    stcTkn_destroy(stack_tkn);
    stcStr_destroy(stack_str);
    dynamicStr_free(sc_str);
    que_destroy(que);
    return ERR_INTERNAL;

err_internal_5:
    stcTkn_destroy(stack_tkn);
    stcStr_destroy(stack_str);
    dynamicStr_free(sc_str);
    que_destroy(que);
    list_destroy(code_buffer);
    return ERR_INTERNAL;
}

////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////



