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
    _mult_,     // *
    _lbrc_,     // (
    _rbrc_,     // )
    _id_,       // id 
    _mins_,     // - 
    _div_,      // /
    _rel_,      // < > <= >=
    _str_,      // str
    _func_,     // f-ce() 
    _coma_,     // , 
    _empt_,     // $
    
    _E_,        /* Non-Terminal */
    _L_,
    _S_,
    _F_,

                /* Rules        */
   _sml_,       // <
   _big_,       // >
   _eql_        // =

} table_elem_t;

typedef enum {
  _inputs_ = 1,       // 0 param
  _inputi_,           // 0 param
  _inputf_,           // 0 param
  _length_,           // 1 param
  _chr_,              // 1 param
  _ord_,              // 2 params
  _substr_,           // 3 params
  _print_,            // X params
} test;
////////////////////////////////////////////////////////////////////////
///                      MACRO DEFINITIONS                           ///
////////////////////////////////////////////////////////////////////////
#define PREC_TABLE_ROWS 17   /* Max number of rows in prec_table */
#define PREC_TABLE_COLS 17   /* Max number of cols in prec_table */

////////////////////////////////////////////////////////////////////////
///                       GLOBAL VARIABLES                           ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                    FUNCTION DECLARATIONS                         ///
////////////////////////////////////////////////////////////////////////
///
/* 
 * @brief Operator-precedence parser
 *
 * @params  sc_str    Param for scanner
 *          que       Param for scanner
 *          symtable  Param for scanner
 * @return  true      If analysed expression is correct  
 *          false     If analysed expression is incorrect 
 */   
int sa_prec(dynamicStr_t *sc_str, queue_t *que, symtable_t *loc_symtable, 
             symtable_t *func_symtable, char **ret_code);



#endif
////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////