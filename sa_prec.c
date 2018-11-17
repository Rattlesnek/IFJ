/**CFile****************************************************************

  FileName    [sa_prec.c]

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
#include "token.h"

////////////////////////////////////////////////////////////////////////
///                       GLOBAL VARIABLES                           ///
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
////////////////////////////////////////////////////////////////////////

/* 
 * @brief Operator-precedence parser
 *
 * @param   token1    Token loaded in advance to make sure expression follows
 *          token2    Token loaded in advance to make sure expression follows
 *          err_code  Indicates whether there is an error  
 * @return  token_t * Last token analysed
 *          NULL      If there is an error
 */   
token_t *sa_prec(token_t *token1, token_t *token2, int *err_code)
{

}

////////////////////////////////////////////////////////////////////////
