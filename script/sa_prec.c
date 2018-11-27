/**CFile****************************************************************

  FileName    [sa_prec.c]

  SystemName  [IFJ - PROJECT]

  PackageName [Syntactic analysis]

  Synopsis    []

  Author      [Lukas Valek, Adam Pankuch, Jindrich Sestak, 
               Lukas Piwowarski]

  Affiliation []

  Date        [01/11/2018]

  Revision    []

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                          INCLUDES                                ///
////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <stdbool.h>

#include "sa_prec.h"

#include "token.h"
#include "scanner.h"
#include "code_gen.h"
#include "builtin_gen.h"
#include "stack_sa_prec.h"
#include "stack_tkn.h"
#include "error.h"

////////////////////////////////////////////////////////////////////////
///                       GLOBAL VARIABLES                           ///
////////////////////////////////////////////////////////////////////////

/* Precedence-table */
char sa_prec_table[PREC_TABLE_ROWS][PREC_TABLE_COLS] = {
/*          +      *     (     )     i     -     /     rel    str    f    ,    $  */
/*  +  */{ '>' , '<' , '<' , '>' , '<' , '>' , '<' ,  '>' , '<' , '<' , '>' , '>' }, 
/*  *  */{ '>' , '>' , '<' , '>' , '<' , '>' , '>' ,  '>' , 'X' , '<' , '>' , '>' },
/*  (  */{ '<' , '<' , '<' , '=' , '<' , '<' , '<' ,  '<' , 'X' , '<' , '=' , 'X' },
/*  )  */{ '>' , '>' , 'X' , '>' , 'X' , '>' , '>' ,  '>' , 'X' , 'X' , '>' , '>' },
/*  i  */{ '>' , '>' , 'X' , '>' , 'X' , '>' , '>' ,  '>' , 'X' , 'X' , '>' , '>' },
/*  -  */{ '>' , '<' , '<' , '>' , '<' , '>' , '<' ,  '>' , 'X' , 'X' , '>' , '>' },
/*  /  */{ '>' , '>' , '<' , '>' , '<' , '>' , '>' ,  '>' , 'X' , 'X' , '>' , '>' },
/* rel */{ '<' , '<' , '<' , '>' , '<' , '<' , '<' ,  'X' , '<' , 'X' , '>' , '>' },
/* str */{ '>' , 'X' , 'X' , 'X' , 'X' , 'X' , 'X' ,  '>' , 'X' , 'X' , 'X' , '>' },
/*  f  */{ 'X' , 'X' , '=' , 'X' , '<' , 'X' , 'X' ,  'X' , 'X' , 'X' , '<' , '>' },
/*  ,  */{ '<' , '<' , '<' , '=' , '<' , '<' , '<' ,  '<' , '<' , '<' , '=' , '>' },
/*  $  */{ '<' , '<' , '<' , 'X' , '<' , '<' , '<' ,  '<' , '<' , '<' , 'X' , 'X' }
};

////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
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
int sa_prec(dynamicStr_t *sc_str, queue_t *que, symtable_t *loc_symtab, 
            symtable_t *func_symtab) {

    return SUCCESS;
}

