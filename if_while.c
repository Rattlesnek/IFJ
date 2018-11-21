/**CFile****************************************************************

  FileName    [if_while.c]

  SystemName  [IFJ - PROJECT]

  PackageName []

  Synopsis    []

  Author      [Jindrich Sestak]

  Affiliation []

  Date        [20/11/2018]

  Revision    []

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                          INCLUDES                                ///
////////////////////////////////////////////////////////////////////////

#include <stdbool.h>

#include "parser.h"
#include "sa_prec.h"
#include "stackStr.h"
#include "error.h"
#include "if_while.h"

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

bool generate_while_ending(stack_str_t *stack)
{
    /******** END of While***********/
    //"LABEL $end_while$%llu\nJUMP $while$%llu\n" has max length 72 bits
    char *b = malloc(sizeof(char) * 72);
    sprintf(b,  "JUMP $while$%llu\n"
                "LABEL $end_while$%llu\n",
                 count,count);
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

void generate_LABEL_while()
{
    /******* LABEL WHILE*************/
    printf("LABEL $while$%llu\n", count);
}

void generate_while_false()
{
    printf("JUMPIFEQ $end_while$%llu COND int@0\n", count);
}







////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////
