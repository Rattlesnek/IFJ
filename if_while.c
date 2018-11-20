/**CFile****************************************************************

  FileName    []

  SystemName  [IFJ - PROJECT]

  PackageName []

  Synopsis    []

  Author      []

  Affiliation []

  Date        [--/--/2018]

  Revision    []

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                          INCLUDES                                ///
////////////////////////////////////////////////////////////////////////

#include <stdbool.h>

#include "parser.h"

#include "scanner.h"
#include "sa_prec.h"

#include "dynamicArrParam.h"
#include "dynamicStr.h"
#include "stackStr.h"
#include "stackTkn.h"
#include "queue.h"

#include "token.h"
#include "symtable.h"

#include "funcGen.h"

#include "error.h"

////////////////////////////////////////////////////////////////////////
///                       GLOBAL VARIABLES                           ///
////////////////////////////////////////////////////////////////////////

unsigned long long int count = 0;

////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
////////////////////////////////////////////////////////////////////////

bool generate_if(stack_str_t *stack)
{
    /*********Jump to ELSE if cond == false*********/ 
    printf("JUMPIFEQ $else$%llu COND int@0\n", count);
    /********* DOING IF statement *************/

    //Must go from bottom to top (LABEL endif -> JMP endif) viz. assembler
    //"LABEL $endif$%llu" has max length 34 bits
    char *b= malloc(sizeof(char)*34);
    sprintf(b, "LABEL $endif$%llu\n", count);
    if (stcStr_push(stack, b) == false)
        return false; 

    /*********END IF, ELSE branch **************/
    //"LABEL $else$%llu\nJMP $endif$%llu" has max length 66 bits
    char *c= malloc(sizeof(char)*66);
    sprintf(c, "JMP $endif$%llu\n"
                "LABEL $else$%llu\n",
                count, count);
    if (stcStr_push(stack,c) == false)
        return false;

    count++;
    return true;
}

bool generate_while(stack_str_t *stack)
{
    /******* LABEL WHILE*************/
    printf("LABEL $while$%llu\n", count);

    /******** END of While***********/
    //"LABEL $end_while$%llu\nJUMP $while$%llu\n" has max length 72 bits
    char *b = malloc(sizeof(char) * 125);
    sprintf(b, "JUMPIFEQ $end_while$%llu COND int@0\n"
                "JUMP $while$%llu\n"
                "LABEL $end_while$%llu\n",
                 count,count, count);
    if (stcStr_push(stack, b) == false)
        return false;

  /*  //"JUMPIFEQ $end_while$%llu COND int@0\n" has max length 53 bits
    char *d =malloc(sizeof(char) * 53);
    sprintf(d, "JUMPIFEQ $end_while$%llu COND int@0\n", count);
    if (stcStr_push(stack, d) == false)
        return false;
 */  
    count++;
    return true;
}





////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////
