/**CFile****************************************************************

  FileName    [scanner.c]

  SystemName  [IFJ - PROJECT]

  PackageName [Scanner]

  Synopsis    []

  Author      [Jindrich Sestak]

  Affiliation []

  Date        19/10/2018]

  Revision    []

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                          INCLUDES                                ///
////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <string.h>

#include "token.h"
#include "scanner.h"

////////////////////////////////////////////////////////////////////////
///                       GLOBAL VARIABLES                           ///
////////////////////////////////////////////////////////////////////////
char sc_name[20] = "\0";
int sc_ptrToName = 0;
int state = State_S;

////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
////////////////////////////////////////////////////////////////////////
void sc_next_char(int c);

int main()
{
    int c = 0;
    while( (c=getc(stdin)) != EOF)
    {  
        sc_next_char(c);

        printf("State_S %d\n", state);
       
    }

}

void sc_next_char(int c)
{
 switch(state){
            case State_S:    
                if (c == '<')
                    state = State_LTN;
                else if (c == '>')
                    state = State_MTN;
                else if (c == ' ' || c == '\t')
                    state = State_S;

                break;
            case State_LTN:
                if (c == '=')
                    state = State_LEQ;
                else if (c != '=')
                {
                    state = State_S;
                    ungetc(c, stdin);
                   // token_info_t info = { .ptr = NULL};
                    //token_t *token = tk_CreateToken("<", info);
                }
                break;
            case State_LEQ:
                state = State_S;
                ungetc(c, stdin);
               // token_info_t info = { .ptr = NULL};
                //token_t *token = tk_CreateToken("<", info);

                break;
            case State_MTN:
                if (c == '=')
                    state = State_MEQ;
                else if (c == ' ')
                {
                    state = State_S;
                    ungetc(c, stdin);
                   // token_info_t info = { .ptr = NULL};
                   // token_t *token = tk_CreateToken(">", info);
                }
                break;
            case State_MEQ:
                state = State_S;
                ungetc(c, stdin);
                //token_info_t info = { .ptr = NULL};
                //token_t *token = tk_CreateToken(">+", info);
        }
}
////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////
