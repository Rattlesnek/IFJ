/**CFile****************************************************************

  FileName    [sa_prec.h]

  SystemName  [IFJ - PROJECT]

  PackageName [Syntactic analysis]

  Synopsis    []

  Author      [Lukas Valek, Adam Pankuch, Jindrich Sestak, 
               Lukas Piwowarski]

  Affiliation []

  Date        [01/11/2018]

  Revision    []

***********************************************************************/
#ifndef SA_PREC_H
#define SA_PREC_H

////////////////////////////////////////////////////////////////////////
///                          INCLUDES                                ///
////////////////////////////////////////////////////////////////////////
#include "dynamic_str.h"
#include "queue.h"
#include "symtable.h"

////////////////////////////////////////////////////////////////////////
///                         BASIC TYPES                              ///
////////////////////////////////////////////////////////////////////////

/* Constants assigned to:
 *    - Terminals (also indexes to precedence-table)
 *    - Operators from precedence-table
 *    - Nonterminal (E)
 */
typedef enum {
/*** Terminal ***/
    _plus_,     // +
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

/*** Rules ***/
    _sml_,      // <
    _big_,      // >
    _eql_,      // =

/*** Nonterminal ***/
    _E_,        
} table_elem_t;

////////////////////////////////////////////////////////////////////////
///                    FUNCTION DECLARATIONS                         ///
////////////////////////////////////////////////////////////////////////

/* 
 * @brief Operator-precedence parser. 
 *
 * @params  sc_str         Param for scanner
 *          que            Param for scanner
 *          symtable       Param for scanner
 *          loc_symtable   List of declared variables (either global or local)
 *          func_symtable  List of declared functions
 *           
 * @return  true         If analysed expression is correct  
 *          false        If analysed expression is incorrect  
 */ 
int sa_prec(dynamicStr_t *sc_str, queue_t *que, symtable_t *loc_symtable, 
            symtable_t *func_symtable);

#endif
////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////