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
#include <stdbool.h>
#include <ctype.h>

#include "token.h"
#include "scanner.h"
#include "dynamicStr.h"


////////////////////////////////////////////////////////////////////////
///                       GLOBAL VARIABLES                           ///
////////////////////////////////////////////////////////////////////////
int state = State_S;
//token_info_t sc_info;
//token_t *sc_token;
dynamicStr_t str;
dynamicStr_t *sc_str = &str;

////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
////////////////////////////////////////////////////////////////////////
token_t* sc_nextChar(int c);
void sc_unget(int c);
int toInt(dynamicStr_t *str);
double toDouble(dynamicStr_t *str);

int main()
{
    printf("Start\n");
    dynamicStr_init(sc_str);
    int c = 0;
    while( (c=getc(stdin)) != EOF)
    {  
        sc_nextChar(c);
        //printf("State_S %d\n", state);

    }
    return 0;
}


/**
 * @brief Function returns char and set state to default (State_S)
 * 
 * @param c character to be ungetc (returning character)
 */
void sc_unget(int c)
{
    state = State_S;
    ungetc(c, stdin);
}

/**
 * @brief Converting str to int
 * 
 * @param str Structure with string to be converted
 * @return int Whole number
 */
int toInt(dynamicStr_t *sc_str)
{
    char *endptr;
    int num = strtol(sc_str->str, &endptr, 10);
    if(*endptr)
    {
        dynamicStr_free(sc_str);
        return -1  ;
    }
    return num;
}

/**
 * @brief Converting str to float
 * 
 * @param str Structure with str to be converted
 * @return float Floating number
 */
double toDouble(dynamicStr_t *sc_str)
{
    char *endptr;
    double num = strtod(sc_str->str, &endptr);
    if(*endptr)
    {
        dynamicStr_free(sc_str);
        return -1  ;
    }
    return num;
}

/**
 * @brief Scanner of the input, generates tokens (Lexical analysis)
 * 
 * @param c character to be read
 * @return token_t* token to be returned
 */
token_t* sc_nextChar(int c)
{
 token_info_t sc_info;
 token_t *sc_token;
 switch(state){
            case State_S:    
                if (c == '<')
                    state = State_LTN;
                else if (c == '>')
                    state = State_MTN;
                else if (c == ' ' || c == '\t')
                    state = State_S;
                else if (c == '=')
                    state = State_ASSIGN;
                else if (c == '!')
                    state = State_EXLAMATION;
                else if (c == ',')
                    state = State_COMMA;
                else if (c == '(')
                    state = State_LEFT_BRACKET;
                else if (c ==')')
                    state = State_RIGHT_BRACKET;
                else if (c == '/')
                    state = State_DIV;
                else if (c == '*')
                    state = State_MUL;
                else if (c == '+')
                    state = State_PLUS;
                else if (c == ' ' || c == '\t')
                    state = State_S;
                else if (c == '"')
                    state = State_QUATATION1;

                else if (c == '-')
                {
                    state = State_MINUS;
                    if(!dynamicStr_add(sc_str, c))
                    {   
                        dynamicStr_free(sc_str);
                        return 0  ;
                    }
                }
                    
                else if (c == '0')
                {
                    state = State_INT0;
                    if(!dynamicStr_add(sc_str, c))
                    {   
                        dynamicStr_free(sc_str);
                        return 0  ;
                    }
                }
                    
                else if (isdigit(c))
                {
                    state = State_INT;
                    if(!dynamicStr_add(sc_str, c))
                    {
                        dynamicStr_free(sc_str);
                        return 0  ;
                    }
                }
                    
                else if (isalpha(c))
                {
                    if(!dynamicStr_add(sc_str, c))
                    {
                        dynamicStr_free(sc_str);
                        return 0  ;
                    }
                    state = State_ID;
                } 

                else if ( c == '\n')
                    state = State_EOL;
                else if ( c == '#')
                    state = State_LCOMM;
                break;

            case State_EOL:
            if ( c == '=')
                state = State_COMM;
            else
            {
                state = State_S;
                sc_unget(c);
                sc_info.string = NULL;
                sc_token= tk_CreateToken("EOL", sc_info);
                printf("Token-name %s\n", sc_token->name);
                dynamicStr_clear(sc_str);
                return sc_token; 
            }
            break;
 
 /******************
  *******COMMENTS*********
**********************/
            case State_COMM:
                if (c == 'b')       //Must save 'b', what if '=bob'
                                    //so identify 'bob' as ID
                {
                    if(!dynamicStr_add(sc_str, c))
                    {
                        dynamicStr_free(sc_str);
                        return 0  ;
                    }
                    state = State_COMMB;
                }
                    
                else        //EOL->'=a' --Error, wrong Assignment
                {           //return token '='
                    state = State_S;
                    sc_unget(c);
                    sc_info.string = NULL;
                    sc_token= tk_CreateToken("=", sc_info);
                     printf("Token-name %s\n", sc_token->name);
                    dynamicStr_clear(sc_str);
                    return sc_token; 
                }
            break;

            case State_COMMB:
                if (c == 'e')
                {
                    if(!dynamicStr_add(sc_str, c))
                    {
                        dynamicStr_free(sc_str);
                        return 0  ;
                    }
                    state = State_COMMBE;
                }
                    
                else
                {   
                    sc_unget(c);
                    state = State_ID;
                }
            break;

            case State_COMMBE:
                if (c == 'g')
                {
                    if(!dynamicStr_add(sc_str, c))
                    {
                        dynamicStr_free(sc_str);
                        return 0  ;
                    }
                    state = State_COMMBEG;
                }
                    
                else
                {   
                    sc_unget(c);
                    state = State_ID;
                }
            break;

            case State_COMMBEG:
                if (c == 'i')
                {
                    if(!dynamicStr_add(sc_str, c))
                    {
                        dynamicStr_free(sc_str);
                        return 0  ;
                    }
                    state = State_COMMBEGI;
                }
                    
                else
                {   
                    sc_unget(c);
                    state = State_ID;
                }
            break;

            case State_COMMBEGI:
                if (c == 'n')
                {
                    if(!dynamicStr_add(sc_str, c))
                    {
                        dynamicStr_free(sc_str);
                        return 0  ;
                    }
                    state = State_COMMBEGIN;
                }
                    
                else
                {   
                    sc_unget(c);
                    state = State_ID;
                }
            break;

            case State_COMMBEGIN:
                if (c == '\n')
                    state = State_MAY_END;
                else if (c == ' ' || c == '\t')
                    state = State_COMM_LINE;
                else        // What if "=beginer"
                {
                    sc_unget(c);
                    state = State_ID;
                }
            break;

            case State_COMM_LINE: //No need to send comments as tokens, thez are just blank space
                if ( c == '\n')
                    state = State_MAY_END;
                else 
                    state = State_COMM_LINE;
                break;

            case State_MAY_END:
                if ( c == '\n')
                    state = State_MAY_END;
                else if (c == '=')
                    state = State_END_COMM;
                else
                    state = State_COMM_LINE;
                break;

            case State_END_COMM:
                if (c == '\n')
                    state = State_MAY_END;
                if (c == 'e')       //It can be "=e?"
                    state = State_END_COMME;
                else
                    state = State_COMM_LINE;
                break;

            case State_END_COMME:
                if (c == 'n')
                    state = State_END_COMMEN;
                else if (c == '\n')
                    state = State_MAY_END;
                else
                {   
                    state = State_COMM_LINE;
                }
            break;

            case State_END_COMMEN:
                if (c == 'd')
                    state = State_END_COMMEND;
                else if (c == '\n')
                    state = State_MAY_END;
                else
                {   
                    state = State_COMM_LINE;
                }
            break;

            case State_END_COMMEND:
                if ( c == '\n')
                {
                   state = State_EOL;
                   sc_unget(c);
                   sc_info.string = NULL;
                   sc_token = tk_CreateToken("BLOCK COMM", sc_info);
                   printf("Token-name %s\n", sc_token->name);
                   return sc_token;
                }
                if ( c == ' ' || c == '\t')
                    state = State_LCOMM;
                else    
                    state = State_COMM_LINE; 
            break;

            case State_LCOMM:
                if ( c == '\n')
                {
                    state = State_S;
                    sc_unget(c);
                    sc_info.string = NULL;
                    sc_token= tk_CreateToken("LINE_COMM", sc_info);
                     printf("Token-name %s\n", sc_token->name);
                    return sc_token; 
                }
                else
                    state = State_LCOMM;
            break;
/***************
 * ********End of Comments***********
 * **************/

/******************
 * ******Strings*********
 * ******************/
            case State_QUATATION1:
                if (c == '\n')           //According to automat "OKREM EOL" - don;t knwo what to do
                {
                   state = State_S;
                }
                else if (c == '\\')     // Char '\' add after we know it means something
                {
                    state = State_SPEC_CHAR;
                }
                else if (c == '"')
                    state = State_QUATATION2;
                else 
                {
                    if(!dynamicStr_add(sc_str, c))
                    {
                        dynamicStr_free(sc_str);
                        return 0  ;
                    }
                    state = State_QUATATION1;
                }
                break;

            case State_SPEC_CHAR:
                if (c == 'n' || c == 't' || c == 's' || c == '\\')
                {   
                    if(!dynamicStr_add(sc_str, '\\'))   //First I add '\\'
                    {
                        dynamicStr_free(sc_str);
                        return 0  ;
                    }
                     if(!dynamicStr_add(sc_str, c))
                    {
                        dynamicStr_free(sc_str);
                        return 0  ;
                    }
                    state = State_QUATATION1;
                }
                else if (c == 'x')
                {
                    if(!dynamicStr_add(sc_str, '\\')) //First I add '\\'
                    {
                        dynamicStr_free(sc_str);
                        return 0  ;
                    }
                    if(!dynamicStr_add(sc_str, c))
                    {
                        dynamicStr_free(sc_str);
                        return 0  ;
                    }
                    state = State_HEX;
                }
                else        //Now I know that '\?' means nothing and shouldn't be taken as char ('nothings is there')
                {
                    state = State_QUATATION1;
                }
                    
                break;

            case State_HEX:
                if ((c >= 'A' && c <= 'F') || isdigit(c))
                {
                    if(!dynamicStr_add(sc_str, c))
                    {
                        dynamicStr_free(sc_str);
                        return 0  ;
                    }
                    state = State_HEX_NUM;
                } 
                else
                {
                    if(!dynamicStr_add(sc_str, c))
                    {
                        dynamicStr_free(sc_str);
                        return 0  ;
                    }
                    state = State_S;
                }
            break;

            case State_HEX_NUM:
                if ((c == 'A' && c <= 'F') || isdigit(c))
                {
                    if(!dynamicStr_add(sc_str, c))
                    {
                        dynamicStr_free(sc_str);
                        return 0  ;
                    }
                    state = State_HEX_NUM_END;
                }
                else if (c == '\n')  //According to automat "OKREM EOL" - don;t knwo what to do
                {
                    state = State_S;
                }
                else
                {
                    if(!dynamicStr_add(sc_str, c))
                    {
                        dynamicStr_free(sc_str);
                        return 0  ;
                    }
                    state = State_QUATATION1;
                }
            break;

            case State_HEX_NUM_END:
                if (c == '\n')  //According to automat "OKREM EOL" - don;t knwo what to do
                {               //Maybe clear sc_str
                    state = State_S;
                }
                else
                {
                    if(!dynamicStr_add(sc_str, c))
                    {
                        dynamicStr_free(sc_str);
                        return 0  ;
                    }
                    state = State_QUATATION1;
                }
            break;

            case State_QUATATION2:
                state = State_S;
                sc_unget(c);
                sc_info.string = sc_str->str;
                sc_token= tk_CreateToken("STR", sc_info);
                printf("Token-name %s  || Value : %s\n", sc_token->name, sc_str->str );
                dynamicStr_clear(sc_str);
                return sc_token;  

/**********************
 * ***********ENd of Strings***********
 * *************************/

/****************************
 * ******ID,FUNC, INT, FLOAT************
 * **********************/
            case State_ID:
                if (isalpha(c) || isdigit(c))
                {
                    if(!dynamicStr_add(sc_str, c))
                    {
                        dynamicStr_free(sc_str);
                        return 0  ;
                    }
                    state = State_ID;
                }

                else if (c == '?' || c == '!')
                {
                    if(!dynamicStr_add(sc_str, c))
                    {
                        dynamicStr_free(sc_str);
                        return 0  ;
                    }
                    state = State_FUNC;
                }
                    
                else 
                {
                    state = State_S;
                    sc_unget(c);
                    sc_info.string = sc_str->str;
                    sc_token= tk_CreateToken("ID", sc_info);
                    printf("Token-name %s  || Value : %s\n", sc_token->name, sc_str->str );
                    dynamicStr_clear(sc_str);
                    return sc_token;  
                }
                break;

            case State_FUNC:
                state = State_S;
                sc_unget(c);
                sc_info.string = sc_str->str;
                sc_token= tk_CreateToken("FUNC", sc_info);
                printf("Token-name %s  || Value : %s\n", sc_token->name, sc_str->str );
                dynamicStr_clear(sc_str);
                return sc_token;  


            case State_INT:
                if (isdigit(c))
                {
                    if(!dynamicStr_add(sc_str, c))
                    {
                        dynamicStr_free(sc_str);
                        return 0  ;
                    }
                    state = State_INT;
                }
                    
                else if (tolower(c) == 'e')
                {
                    if(!dynamicStr_add(sc_str, c))
                    {
                        dynamicStr_free(sc_str);
                        return 0  ;
                    }
                    state = State_EXP;
                }
                    
                else if (c == '.')
                {
                    if(!dynamicStr_add(sc_str, c))
                    {
                        dynamicStr_free(sc_str);
                        return 0  ;
                    }
                    state = State_DOT;
                }

                else
                {
                    state = State_S;
                    sc_unget(c);
                    sc_info.intg = toInt(sc_str); 
                    sc_token= tk_CreateToken("INT", sc_info);
                    printf("Token-name %s  || Value : %d\n", sc_token->name, sc_info.intg );
                    dynamicStr_clear(sc_str);
                    return sc_token;   
                }
                break;

/******************************
 * ***********TODO************     What to do when "00"
 * ****************************/
            case State_INT0:
                if (c == '.')
                {
                    if(!dynamicStr_add(sc_str, c))
                    {
                        dynamicStr_free(sc_str);
                        return 0  ;
                    }
                    state = State_DOT;
                }
                    
                else if (tolower(c) == 'e')
                {
                    if(!dynamicStr_add(sc_str, c))
                    {
                        dynamicStr_free(sc_str);
                        return 0  ;
                    }
                    state = State_EXP;
                }
                    
                else if (c == '0')      //Don't know what to do if "00"
                {                       //Maybe ungetc and try this character
                                        //Clear sc_str
                    state = State_S;
                    dynamicStr_clear(sc_str);
                    printf("Wrong stoDoubleuence of chars");
                }  

                else
                {
                    state = State_S;
                    sc_unget(c);
                    sc_info.intg = 0;
                    sc_token= tk_CreateToken("ZERO", sc_info);
                    dynamicStr_clear(sc_str);
                    return sc_token;  
                }
                break;

/******************************
 * ***********TODO************      What to do when "0.?"
 * ****************************/
            case State_DOT:
                if (isdigit(c))
                {
                    if(!dynamicStr_add(sc_str, c))
                    {
                        dynamicStr_free(sc_str);
                        return 0  ;
                    }
                    state = State_FLOAT;
                }
                    
                else
                {
                    state = State_S;            //Don't know what to do if "0.?"
                    dynamicStr_clear(sc_str);   //Maybe ungetc and try this character
                    printf("Wrong stoDoubleuence of chars");    //Clear sc_str    
                                                                
                }
                break;
/******************************
 * ***********TODO************      What to do when "0e?"
 * ****************************/
            case State_EXP:
                if (isdigit(c))
                {
                    if(!dynamicStr_add(sc_str, c))
                    {
                        dynamicStr_free(sc_str);
                        return 0  ;
                    }
                    state = State_FLOAT;
                }
                    
                else if (c == '+' || c == '-')
                {
                    if(!dynamicStr_add(sc_str, c))
                    {
                        dynamicStr_free(sc_str);
                        return 0  ;
                    }
                    state = State_EXPS;
                }
            
                else 
                {
                    state = State_S;            //Don't know what to do if "0.?"
                    dynamicStr_clear(sc_str);   //Maybe ungetc and try this character
                    printf("Wrong stoDoubleuence of chars");    //Clear sc_strtc and try this character
                }
                break;

/******************************
 * ***********TODO************      What to do when "0e-?"
 * ****************************/
            case State_EXPS:
                if (isdigit(c))
                {
                    if(!dynamicStr_add(sc_str, c))
                    {
                        dynamicStr_free(sc_str);
                        return 0  ;
                    }
                    state = State_FLOAT;
                }
                    
                else 
                {
                    state = State_S;            //Don't know what to do if "0.?"
                    dynamicStr_clear(sc_str);   //Maybe ungetc and try this character
                    printf("Wrong stoDoubleuence of chars");    //Clear sc_strtc and try this character
                }
                break;

            case State_FLOAT:
                if (isdigit(c))
                {
                    if(!dynamicStr_add(sc_str, c))
                    {
                        dynamicStr_free(sc_str);
                        return 0  ;
                    }
                    state = State_FLOAT;
                }
                    
                else
                {
                    state = State_S;
                    sc_unget(c);
                    sc_info.dbl = toDouble(sc_str); 
                    sc_token= tk_CreateToken("INT", sc_info);
                    printf("Token-name %s  || Value : %f\n", sc_token->name, sc_info.dbl );
                    dynamicStr_clear(sc_str);
                    return sc_token;   
                }
                break;

/*****************************
 * *********End of ID, FUNC, INT, FLOAT *********
 * *********************/


/******************************
 * ***********LOGICAL OPERATORS and so************      Check if ELSE brach is correct?
 * ****************************/
            case State_LTN:
                if (c == '=')
                    state = State_LEQ;
                else
                {
                    state = State_S;
                    sc_unget(c);
                    sc_info.string = NULL;
                    sc_token= tk_CreateToken("<", sc_info);
                    return sc_token;
                }
                break;

            case State_LEQ:
                state = State_S;
                sc_unget(c);
                sc_info.string = NULL;
                sc_token= tk_CreateToken("<=", sc_info);
                return sc_token;
                break;

            case State_MTN:
                if (c == '=')
                    state = State_MEQ;
                else
                {
                    state = State_S;
                    sc_unget(c);
                    sc_info.string = NULL;
                    sc_token= tk_CreateToken(">", sc_info);
                    return sc_token;
                }
                break;

            case State_MEQ:
                state = State_S;
                sc_unget(c);
                sc_info.string = NULL;
                sc_token= tk_CreateToken(">=", sc_info);
                return sc_token;
                break;

            case State_ASSIGN:
                if (c == '=')
                    state = State_EQ;
                else
                {
                    state = State_S;
                    sc_unget(c);
                    sc_info.string = NULL;
                    sc_token= tk_CreateToken("=", sc_info);
                    return sc_token;
                }
                break;

            case State_EQ:
                state = State_S;
                sc_unget(c);
                sc_info.string = NULL;
                sc_token= tk_CreateToken("==", sc_info);
                return sc_token;
                break;

/***********************
 * *********TODO*********       Check if "!" is allowed as neg
 **********************/
            case State_EXLAMATION:
                if (c == '=')
                    state = State_NEQ;
                else
                {
                    state = State_S;
                    sc_unget(c);        //not defined symbol "!"
                }
                    
                break;
            
            case State_NEQ:
                state = State_S;
                sc_unget(c);
                sc_info.string = NULL;
                sc_token= tk_CreateToken("!=", sc_info);
                return sc_token;
                break;
                
            case State_COMMA:
                state = State_S;
                sc_unget(c);
                sc_info.string = NULL;
                sc_token= tk_CreateToken(",", sc_info);
                return sc_token;
                break;

            case State_LEFT_BRACKET:
                state = State_S;
                sc_unget(c);
                sc_info.string = NULL;
                sc_token= tk_CreateToken("(", sc_info);
                return sc_token;
                break;

            case State_RIGHT_BRACKET:
                state = State_S;
                sc_unget(c);
                sc_info.string = NULL;
                sc_token= tk_CreateToken(")", sc_info);
                return sc_token;
                break;

            case State_DIV:
                state = State_S;
                sc_unget(c);
                sc_info.string = NULL;
                sc_token= tk_CreateToken("/", sc_info);
                return sc_token;
                break;
                
            case State_MUL:
                state = State_S;
                sc_unget(c);
                sc_info.string = NULL;
                sc_token= tk_CreateToken("*", sc_info);
                return sc_token;
                break;

            case State_MINUS:
                if (isdigit(c))
                {
                    if(!dynamicStr_add(sc_str, c))
                    {
                        dynamicStr_free(sc_str);
                        return 0  ;
                    }
                    state = State_INT;
                }
                else
                {
                    state = State_S;
                    sc_unget(c);
                    sc_info.string = NULL;
                    sc_token= tk_CreateToken("-", sc_info);
                    dynamicStr_clear(sc_str);
                    return sc_token;
                }
                break;

            case State_PLUS:
                state = State_S;
                sc_unget(c);
                sc_info.string = NULL;
                sc_token= tk_CreateToken("+", sc_info);
                return sc_token;
                break;
        }
    return NULL;
}
////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////
