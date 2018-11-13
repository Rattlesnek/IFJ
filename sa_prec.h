/**CFile****************************************************************

  FileName    [sa_prec.h]

  SystemName  [IFJ - PROJECT]

  PackageName [Syntactic analysis]

  Synopsis    []

  Author      [Lukas Piwowarski, Lukas Valek]

  Affiliation []

  Date        [01/11/2018]

  Revision    []

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                          INCLUDES                                ///
////////////////////////////////////////////////////////////////////////
#ifndef SA_PREC_H
#define SA_PREC_H

#include "dynamicStr.h"
#include "queue.h"
#include "symtable.h"
////////////////////////////////////////////////////////////////////////
///                         BASIC TYPES                              ///
////////////////////////////////////////////////////////////////////////
typedef enum {
    _plus_,     /* Terminal     */
    _mult_,
    _id_,
    _empt_,
    
    _E_,        /* Non-Terminal */
    _L_,
    _S_,

                /* Rules        */
   _sml_,       // <
   _big_,       // >
   _eql_        // =

} table_elem_t;


////////////////////////////////////////////////////////////////////////
///                      MACRO DEFINITIONS                           ///
////////////////////////////////////////////////////////////////////////
#define PREC_TABLE_ROWS 4   /* Max number of rows in prec_table */
#define PREC_TABLE_COLS 4   /* Max number of cols in prec_table */

////////////////////////////////////////////////////////////////////////
///                       GLOBAL VARIABLES                           ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                    FUNCTION DECLARATIONS                         ///
////////////////////////////////////////////////////////////////////////


/* 
 * @brief Operator-precedence parser
 *
 * @params  sc_str    Param for scanner
 *          que       Param for scanner
 *          symtable  Param for scanner
 * @return  true      If analysed expression is correct  
 *          false     If analysed expression is incorrect 
 */   
bool sa_prec(dynamicStr_t *sc_str, queue_t *que, symtable_t *symtable);



#endif
////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////