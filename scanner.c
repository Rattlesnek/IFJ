/**CFile****************************************************************

  FileName    [scanner.c]

  SystemName  [IFJ - PROJECT]

  PackageName [Scanner]

  Synopsis    [Lexical analysis - reads input and create tokens]

  Author      [Jindrich Sestak]

  Login       [xsesta05]

  Affiliation []

  Date        [19/10/2018]

  Revision    []

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                          INCLUDES                                ///
////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "token.h"
#include "dynamic_str.h"
#include "error.h"
#include "queue.h"
#include "symtable.h"

#include "scanner.h"

#ifdef SCANNER_PRINT
    #define SCANNER_DBG_PRINT(...) do{ printf( __VA_ARGS__ ); } while(0)
#else
    #define SCANNER_DBG_PRINT(...) do{ } while(0)
#endif

////////////////////////////////////////////////////////////////////////
///                       GLOBAL VARIABLES                           ///
////////////////////////////////////////////////////////////////////////
int state = State_S;
char *keywords[] = {"def", "do", "else", "end", "if", "not", "nil", "then", "while", "elsif"};
int KeywordLen = 10;

char *builtin_func[] = {"inputs", "inputi", "inputf", "print", "length", "substr", "ord", "chr"};
int Built_Func = 8;

////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
///////////////////////////////////////////////////////////////////////


void sc_unget(int c)
{
    state = State_S;
    ungetc(c, stdin);
}

bool Str_with_hex(dynamicStr_t *sc_str,unsigned int position)
{
    if (position > sc_str->length)
        return false;

    unsigned int count = 0;
    char hex[4];
    char *endptr;
    long ret = strtol(sc_str->str + position, &endptr, 16);
    if(*endptr)
        return false;

    sprintf(hex, "%li", ret); 

    //re-write last characters ("\xhh) with number ("\xiii")
    sc_str->length = position;
    if (strlen(hex) < 2)
    {   
        if(!dynamicStr_add(sc_str, '0'))
            return false;

        if(!dynamicStr_add(sc_str, '0'))
            return false;
    }
    else if (strlen(hex) < 3)
    {
        if(!dynamicStr_add(sc_str, '0'))
            return false;
    }   

    for (unsigned int i = 0; i < strlen(hex); i++)
    {
        if(!dynamicStr_add(sc_str, hex[count]))
            return false;

        count++;
    }
    return true;
}

bool InttoStr (char *s, dynamicStr_t *sc_str)
{
    char *endptr;
    int num = strtol(sc_str->str, &endptr, 10);
    if(*endptr)
    {
        return false;
    }
    sprintf(s, "%d", num);
    return true;
}


bool DobtoStr (char *s, dynamicStr_t *sc_str)
{
    char *endptr;
    double num = strtod(sc_str->str, &endptr);
    if(*endptr)
    {
        return false;
    }
    while ( (unsigned)(sprintf(s, "%a", num)) > strlen(s) )
    {
        unsigned int new_size = strlen(s) + 10;
		s = realloc(s, new_size * sizeof(char));
    }
    return true;
}

char* inKeyword(char *str, char **keywords)
{
    for (int i = 0; i < KeywordLen; i++)
    {
        if (strcmp(keywords[i], str) == 0)
            return keywords[i];
    }
   
    return "ID";
}

char *inBuiltin_Func(char *str, char **builtin_func)
{
    for (int i = 0; i < Built_Func; i++)
    {
        if (strcmp(builtin_func[i], str) == 0)
            return builtin_func[i];
    }
   
    return "ID";
}

bool BintoStr (char *s, dynamicStr_t *sc_str)
{
    char *endptr;
    int num = strtol(sc_str->str, &endptr, 2);
    if(*endptr)
    {
        return false;
    }
    while ( (unsigned)(sprintf(s, "%d", num)) > strlen(s) )
    {
        unsigned int new_size = strlen(s) + 10;
		s = realloc(s, new_size * sizeof(char));
    }
    return true;
}

bool OcttoStr (char *s, dynamicStr_t *sc_str)
{
    char *endptr;
    int num = strtol(sc_str->str, &endptr, 8);
    if(*endptr)
    {
        return false;
    }
    while ( (unsigned)(sprintf(s, "%d", num)) > strlen(s) )
    {
        unsigned int new_size = strlen(s) + 10;
		s = realloc(s, new_size * sizeof(char));
    }
    return true;
}

bool HextoStr (char *s, dynamicStr_t *sc_str)
{
    char *endptr;
    int num = strtol(sc_str->str, &endptr, 16);
    if(*endptr)
    {
        return false;
    }
    while ( (unsigned)(sprintf(s, "%d", num)) > strlen(s) )
    {
        unsigned int new_size = strlen(s) + 10;
		s = realloc(s, new_size * sizeof(char));
    }
    return true;
}

bool scanner_unget(queue_t *que, token_t *token, char *name)
{
    return que_up(que, token, name);
}


token_t* scanner_get(dynamicStr_t *sc_str, queue_t *que)
{
    dynamicStr_clear(sc_str);
    token_info_t sc_info;
    token_t *sc_token;
    int c = 0;

////////////////////////////////////////////////////////////
///////     Comment in the begginig of the file     ///////
///////////////////////////////////////////////////////////
    static bool test_begin = true;
    if(test_begin)
    {
        test_begin = false;
        if((c = getc(stdin)) != EOF)
        {
            if(c == '=')
            {
                state = State_COMM;
            }
            else if (c == '#')
                state = State_LCOMM;

            else 
                ungetc(c, stdin);
            
        }
    }
    if (que_empty(que))
    { 
        while( (c=getc(stdin)) != EOF)
        { 
            switch(state){
                case State_S:    
                    if (c == '<')
                        state = State_LTN;
                    else if (c == '>')
                        state = State_MTN;
                    else if (isspace(c) && c != '\n')
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
                        state = State_MINUS;
                        
                    else if (c == '0')
                    {
                        state = State_INT0;
                        if(!dynamicStr_add(sc_str, c))   
                            goto err_internal;
                    }
                        
                    else if (isdigit(c))
                    {
                        state = State_INT;
                        if(!dynamicStr_add(sc_str, c))
                            goto err_internal;
                    }
                        
                    else if ( (c >= 'a' && c <= 'z') || c == '_')
                    {
                        if(!dynamicStr_add(sc_str, c))
                            goto err_internal;

                        state = State_ID;
                    } 
                    else if ( c == '\\')
                        state = State_Backslash;

                    else if ( c == '\n')
                        state = State_EOL;
                    else if ( c == '#')
                        state = State_LCOMM;
                    else
                        state = State_ERR;
                    break;

                case State_EOL:
                    if ( c == '=')
                        state = State_COMM;
                    else
                    {
                        state = State_S;
                        sc_unget(c);
                        sc_info.ptr = NULL;
                        sc_token= createToken("EOL", sc_info);
                        if (sc_token == NULL)
                            goto err_internal;

                        SCANNER_DBG_PRINT("Token-name: %s\n", sc_token->name);
                        return sc_token; 
                    }
                    break;
    ///////////////////////////////////////
    ///             COMMENTS            ///
    ///////////////////////////////////////
                case State_COMM:
                    if (c == 'b')
                    {
                        if(!dynamicStr_add(sc_str, c))
                            goto err_internal;

                        state = State_COMMB;
                    }
                        
                    else 
                    {
                        goto err_lexical;
                    }
                    break;

                case State_COMMB:
                    if (c == 'e')
                    {
                        if(!dynamicStr_add(sc_str, c))
                            goto err_internal;

                        state = State_COMMBE;
                    }
                        
                    else 
                        goto err_lexical;
                    break;

                case State_COMMBE:
                    if (c == 'g')
                    {
                        if(!dynamicStr_add(sc_str, c))
                            goto err_internal;

                        state = State_COMMBEG;
                    }
                        
                    else   
                        goto err_lexical;
                    
                    break;

                case State_COMMBEG:
                    if (c == 'i')
                    {
                        if(!dynamicStr_add(sc_str, c))
                            goto err_internal;

                        state = State_COMMBEGI;
                    }
                        
                    else   
                        goto err_lexical;

                    break;

                case State_COMMBEGI:
                    if (c == 'n')
                    {
                        if(!dynamicStr_add(sc_str, c))
                            goto err_internal;
    
                        state = State_COMMBEGIN;
                    }
                        
                    else   
                        goto err_lexical;
                    
                    break;

                case State_COMMBEGIN:
                    if (c == '\n')
                        state = State_MAY_END;
                    else if (isspace(c) && c != '\n')
                        state = State_COMM_LINE;
                    else 
                        goto err_lexical;

                    break;

                case State_COMM_LINE:
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
                    if (c == 'e')  
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
                        state = State_COMM_LINE;
                    break;

                case State_END_COMMEN:
                    if (c == 'd')
                        state = State_END_COMMEND;
                    else if (c == '\n')
                        state = State_MAY_END;
                    else  
                        state = State_COMM_LINE;
                    break;

                case State_END_COMMEND:
                    if ( c == '\n')
                    {
                        state = State_S;
                        sc_unget(c);
                        sc_info.ptr = NULL;
                        sc_token = createToken("EOL", sc_info);
                        if (sc_token == NULL)
                            goto err_internal;

                        SCANNER_DBG_PRINT("Token-name: %s\n", sc_token->name);
                        return sc_token;
                    }
                    if ( isspace(c) && c != '\n')
                        state = State_LCOMM;
                    else    
                        state = State_COMM_LINE; 
                    break;

                case State_LCOMM:
                    if ( c == '\n')
                    {
                        state = State_S;
                        sc_unget(c);
                        sc_info.ptr = NULL;
                        sc_token= createToken("EOL", sc_info);
                        if (sc_token == NULL)
                            goto err_internal;

                        SCANNER_DBG_PRINT("Token-name: %s\n", sc_token->name);
                        return sc_token; 
                    }
                    else
                        state = State_LCOMM;
                    break;

    ///////////////////////////////////////
    ///             STRINGS            ///
    ///////////////////////////////////////
                case State_QUATATION1:
                    if (c == '\n')   
                        goto err_lexical;
                    
                    else if (c == '\\')     // Char '\' add after we know it means something
                    {
                        state = State_SPEC_CHAR;
                    }
                    else if (c == '"')
                        state = State_QUATATION2;

                    else if (c == ' ')
                    {
                        if(!dynamicStr_add(sc_str, '\\'))
                            goto err_internal;

                        if(!dynamicStr_add(sc_str, '0'))
                            goto err_internal;

                        if(!dynamicStr_add(sc_str, '3'))
                            goto err_internal;

                        if(!dynamicStr_add(sc_str, '2'))
                            goto err_internal;
                    }
                    else 
                    {
                        if(!dynamicStr_add(sc_str, c))
                            goto err_internal;

                        state = State_QUATATION1;
                    }
                    break;

                case State_SPEC_CHAR:
                    state = State_QUATATION1;
                    if (c == '\\')
                    {   
                        if(!dynamicStr_add(sc_str, '\\'))
                            goto err_internal;

                        if(!dynamicStr_add(sc_str, '0'))
                            goto err_internal;

                        if(!dynamicStr_add(sc_str, '9'))
                            goto err_internal;

                        if(!dynamicStr_add(sc_str, '2'))
                            goto err_internal;
                    }
                    else if (c == 't')
                    {
                        if(!dynamicStr_add(sc_str, '\\'))
                            goto err_internal;

                        if(!dynamicStr_add(sc_str, '0'))
                            goto err_internal;

                        if(!dynamicStr_add(sc_str, '0'))
                            goto err_internal;

                        if(!dynamicStr_add(sc_str, '9'))
                            goto err_internal;
                    }
                    else if (c == 'n')
                    {
                        if(!dynamicStr_add(sc_str, '\\'))
                            goto err_internal;

                        if(!dynamicStr_add(sc_str, '0'))
                            goto err_internal;

                        if(!dynamicStr_add(sc_str, '1'))
                            goto err_internal;

                        if(!dynamicStr_add(sc_str, '0'))
                            goto err_internal;
                    }
                    else if (c == 's')
                    {
                        if(!dynamicStr_add(sc_str, '\\'))
                            goto err_internal;

                        if(!dynamicStr_add(sc_str, '0'))
                            goto err_internal;

                        if(!dynamicStr_add(sc_str, '3'))
                            goto err_internal;

                        if(!dynamicStr_add(sc_str, '2'))
                            goto err_internal;
                    }
                    else if (c == '"')
                    {
                        if(!dynamicStr_add(sc_str, '\\'))
                            goto err_internal;

                        if(!dynamicStr_add(sc_str, '0'))
                            goto err_internal;

                        if(!dynamicStr_add(sc_str, '3'))
                            goto err_internal;

                        if(!dynamicStr_add(sc_str, '4'))
                            goto err_internal;
                    }
                    else if (c == 'x')
                    {
                        if(!dynamicStr_add(sc_str, '\\')) //First I add '\\'
                            goto err_internal;

                        state = State_HEX;
                    }
                    else
                        state = State_ERR;
                        
                    break;

                case State_HEX:
                    if ((c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f') || isdigit(c))
                    {
                        if(!dynamicStr_add(sc_str, c))
                            goto err_internal;

                        state = State_HEX_NUM;
                    }
                    else  
                        state = State_ERR;
                    
                    break;

                case State_HEX_NUM:
                    if ((c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f') || isdigit(c))
                    {
                        if(!dynamicStr_add(sc_str, c))
                            goto err_internal;

                        if ((Str_with_hex(sc_str, sc_str->length - 2)) == false)
                            goto err_internal;

                        state = State_QUATATION1;
                    }
                    else if (c == '\n')
                        goto err_lexical;

                    else if (c == '"')
                    {
                        if ((Str_with_hex(sc_str, sc_str->length - 1)) == false)
                            goto err_internal;

                        state = State_QUATATION2;
                    }
                    else
                    {
                        if ((Str_with_hex(sc_str, sc_str->length - 1)) == false)
                            goto err_internal;
                        
                        sc_unget(c);

                        state = State_QUATATION1;
                    }
                    break;

                case State_QUATATION2:
                    state = State_S;
                    sc_unget(c);
                    // allocate space for string which will be stored in token info
                    sc_info.string = malloc( (strlen(sc_str->str) + 1) * sizeof(char) );
                    // copy string from dynamicStr to token info
                    strcpy(sc_info.string, sc_str->str);
                    sc_token = createToken("STR", sc_info);
                    if (sc_token == NULL)
                        goto err_internal;

                    SCANNER_DBG_PRINT("Token-name: %s  || Value : %s\n", sc_token->name, sc_str->str );
                    
                    return sc_token;  

    ///////////////////////////////////////
    ///        ID, FUNC INT, FLOAT      ///
    ///////////////////////////////////////
                case State_ID:
                    if (isalpha(c) || isdigit(c) || c == '_')
                    {
                        if(!dynamicStr_add(sc_str, c))
                            goto err_internal;
                        
                        state = State_ID;
                    }

                    else if (c == '?')
                    {
                        if(!dynamicStr_add(sc_str, c))
                            goto err_internal;

                        state = State_FUNC;
                    }

                    else if ( c == '!')
                    {
                        state = State_IDEXL;
                    }
                            //( ,) ,* ,+ , , , -              /             <,=,>
                    else if ((c >= '(' && c <= '-') || c == '/' || (c >= '<' && c <= '>') || isspace(c))
                    {
                        state = State_S;
                        sc_unget(c);
                        char *name = "ID";
                        if (strcmp(inKeyword(sc_str->str, keywords),"ID") != 0 )            //Token: "IF", atribut:NULL
                        {
                            name = inKeyword(sc_str->str, keywords);
                            if (strcmp(name, "do") == 0 || strcmp(name, "else")==0 ||strcmp(name, "end")==0 || strcmp(name, "then")==0)
                            {
                                if ( !isspace(c))  //after "do", "else" ....must be \n
                                {
                                    printf("DPRINT int@5\n");
                                    state = State_ERR;
                                    break;
                                }
                            }
                            sc_info.ptr = NULL;
                            sc_token= createToken(name, sc_info);
                            if (sc_token == NULL)
                                goto err_internal;

                            SCANNER_DBG_PRINT("Token-name: %s\n", sc_token->name); 
                            return sc_token; 
                        }
                        else if (strcmp(inBuiltin_Func(sc_str->str, builtin_func),"ID") != 0)   //Token:"BUILTIN", atribut: "name_of_func"
                        {
                            name = inBuiltin_Func(sc_str->str, builtin_func);
                            sc_info.string = malloc(sizeof(char)* (strlen(name)+1));
                            strcpy(sc_info.string, name);
                            sc_token= createToken("BUILTIN", sc_info);
                            if (sc_token == NULL)
                                goto err_internal;

                            SCANNER_DBG_PRINT("Token-name: %s || Value : %s\n", sc_token->name, sc_token->info.string );
                            return sc_token; 
                        }
                        else
                        {
                            sc_info.ptr = NULL;
                            sc_token= createToken("ID", sc_info);
                            if (sc_token == NULL)
                                goto err_internal;
    
                            SCANNER_DBG_PRINT("Token-name: %s\n", sc_token->name);
                            return sc_token;  
                        }
                    
                    }
                    else 
                        state = State_ERR;
                    break;

                case State_IDEXL:
                    if ( c == '\n')
                    {
                        if(!dynamicStr_add(sc_str, '!'))
                            goto err_internal;

                        state = State_S;
                        sc_info.ptr = NULL;
                        sc_token= createToken("FUNC", sc_info);
                        if (sc_token == NULL)
                            goto err_internal;

                        SCANNER_DBG_PRINT("Token-name: %s \n", sc_token->name);
                        return sc_token;  
                    }
                    else if ( c == '=')
                    {
                        token_info_t sc_info2;
                        sc_info2.ptr = NULL;
                        token_t *sc_token2 = createToken("!=", sc_info2);
                        que_up(que, sc_token2, sc_str->str);

                        char *name = "ID";
                        if (strcmp(inKeyword(sc_str->str, keywords),"ID") != 0 )
                            goto err_lexical;

                        sc_info.ptr = NULL; 
                        sc_token= createToken(name, sc_info);
                        if (sc_token == NULL)
                            goto err_internal;

                        SCANNER_DBG_PRINT("Token-name: %s\n", sc_token->name);
                        return sc_token;  
                    }
                    else if ( c == '(' || c == ' ')
                    {
                        if(!dynamicStr_add(sc_str, '!'))
                            goto err_internal;

                        sc_unget(c);
                        state = State_FUNC;
                    }
                    else
                    {
                        sc_unget(c);
                        state = State_ERR;
                    }
                    break;

                case State_FUNC:
                    if ( c == '(' || c == ' ')
                    {
                        state = State_S;
                        sc_unget(c);
                        sc_info.ptr = NULL;
                        sc_token= createToken("FUNC", sc_info);
                        if (sc_token == NULL)
                            goto err_internal;

                        SCANNER_DBG_PRINT("Token-name: %s \n", sc_token->name);
                        return sc_token;
                    }
                    else
                    {
                        sc_unget(c);
                        state = State_ERR;
                    }
                    break;
                     


                case State_INT:
                    if (isdigit(c))
                    {
                        if(!dynamicStr_add(sc_str, c))
                            goto err_internal;
                        
                        state = State_INT;
                    }   
                    else if (tolower(c) == 'e')
                    {
                        if(!dynamicStr_add(sc_str, c))
                            goto err_internal;
                        
                        state = State_EXP;
                    }  
                    else if (c == '.')
                    {
                        if(!dynamicStr_add(sc_str, c))
                            goto err_internal;
                        
                        state = State_DOT;
                    }
                                //(,),*,+, , ,-         /             <,=,>
                    else if ( (c >= '(' && c <= '-') || c == '/' || (c >= '<' && c <= '>') || isspace(c))    //another characters like: +,-,*,/
                    {
                        state = State_S;
                        sc_unget(c);
                        sc_info.string = malloc( (strlen(sc_str->str) + 1) * sizeof(char) );
                        if ( !(InttoStr(sc_info.string, sc_str)))   //False = convert wasn't succesful
                            goto err_internal;

                        sc_token= createToken("INT", sc_info);
                        if (sc_token == NULL)
                            goto err_internal;

                        SCANNER_DBG_PRINT("Token-name: %s  || Value : %s\n", sc_token->name, sc_info.string );
                        return sc_token;   
                    }
                    else
                    {
                        sc_unget(c);
                        state = State_ERR;
                    }
                    break;

                case State_INT0:
                    if (c == '.')
                    {
                        if(!dynamicStr_add(sc_str, c))
                            goto err_internal;
                        
                        state = State_DOT;
                    }     
                    else if (tolower(c) == 'e')
                    {
                        if(!dynamicStr_add(sc_str, c))
                            goto err_internal;
                        
                        state = State_EXP;
                    }
                                //(,),*,+, , ,-             /             <,=,>
                    else if ( (c >= '(' && c <= '-') || c == '/' || (c >= '<' && c <= '>') || isspace(c))
                    {
                        state = State_S;
                        sc_unget(c);
                        sc_info.string = malloc( (strlen(sc_str->str) + 1) * sizeof(char) );
                        if ( !(InttoStr(sc_info.string, sc_str)))   //False = convert wasn't succesful
                            goto err_internal;

                        sc_token= createToken("INT", sc_info);
                        if (sc_token == NULL)
                            goto err_internal;

                        SCANNER_DBG_PRINT("Token-name: %s  || Value : %s\n", sc_token->name, sc_info.string);
                        return sc_token;  
                    }
                    else if ( isdigit(c))
                    {
                        if(!dynamicStr_add(sc_str, c))
                            goto err_internal;

                        state = State_Octal;
                    }
                    else if (c == 'b')
                    {
                        dynamicStr_clear(sc_str);
                        state = State_Binary;
                    }
                    else
                    {
                        sc_unget(c);
                        state = State_ERR;
                    }
                    break;

                case State_DOT:
                    if (isdigit(c))
                    {
                        if(!dynamicStr_add(sc_str, c))
                            goto err_internal;
                        
                        state = State_FLOAT;
                    }
                    else
                    {
                        sc_unget(c);
                        state = State_ERR;
                    }
                    break;

                case State_EXP:
                    if (isdigit(c))
                    {
                        if(!dynamicStr_add(sc_str, c))
                            goto err_internal;
                        
                        state = State_FLOAT;
                    }
                    else if (c == '+' || c == '-')
                    {
                        if(!dynamicStr_add(sc_str, c))
                            goto err_internal;
                        
                        state = State_EXPS;
                    }
                    else
                    {
                        sc_unget(c);
                        state = State_ERR;
                    }
                    break;

                case State_EXPS:
                    if (isdigit(c))
                    {
                        if(!dynamicStr_add(sc_str, c))
                            goto err_internal;
                        
                        state = State_FLOAT;
                    }
                    else
                    {
                        sc_unget(c);
                        state = State_ERR;
                    }
                    break;

                case State_FLOAT:
                    if (isdigit(c))
                    {
                        if(!dynamicStr_add(sc_str, c))
                            goto err_internal;
                        
                        state = State_FLOAT;
                    }
                    else if (tolower(c) == 'e')
                    {
                        if(!dynamicStr_add(sc_str, c))
                            goto err_internal;
                        
                        state = State_EXP;
                    }
                                //(,),*,+, , ,-         /             <,=,>
                    else if ( (c >= '(' && c <= '-') || c == '/' || (c >= '<' && c <= '>') || isspace(c))
                    {
                        state = State_S;
                        sc_unget(c);
                        sc_info.string = malloc( 25 * sizeof(char) );
                        if ( !(DobtoStr(sc_info.string, sc_str)))   //False = convert wasn't succesful
                            goto err_internal;

                        sc_token = createToken("DBL", sc_info);
                        if (sc_token == NULL)
                            goto err_internal;

                        SCANNER_DBG_PRINT("Token-name: %s  || Value : %s\n", sc_token->name, sc_info.string );
                        return sc_token;   
                    }
                    else
                    {
                        sc_unget(c);
                        state = State_ERR;
                    }
                    break;

    ///////////////////////////////////////
    /// LOGICAL AND NUMERICAL OPERATORS ///
    ///////////////////////////////////////
                case State_LTN:
                    if (c == '=')
                        state = State_LEQ;
                    else
                    {
                        state = State_S;
                        sc_unget(c);
                        sc_info.ptr = NULL;
                        sc_token= createToken("<", sc_info);
                        if (sc_token == NULL)
                            goto err_internal;

                        SCANNER_DBG_PRINT("Token-name: %s\n", sc_token->name);
                        return sc_token;
                    }
                    break;

                case State_LEQ:
                    state = State_S;
                    sc_unget(c);
                    sc_info.ptr = NULL;
                    sc_token= createToken("<=", sc_info);
                    if (sc_token == NULL)
                        goto err_internal;

                    SCANNER_DBG_PRINT("Token-name: %s\n", sc_token->name);
                    return sc_token;
                    break;

                case State_MTN:
                    if (c == '=')
                        state = State_MEQ;
                    else
                    {
                        state = State_S;
                        sc_unget(c);
                        sc_info.ptr = NULL;
                        sc_token= createToken(">", sc_info);
                        if (sc_token == NULL)
                            goto err_internal;

                        SCANNER_DBG_PRINT("Token-name: %s\n", sc_token->name);
                        return sc_token;
                    }
                    break;

                case State_MEQ:
                    state = State_S;
                    sc_unget(c);
                    sc_info.ptr = NULL;
                    sc_token= createToken(">=", sc_info);
                    if (sc_token == NULL)
                        goto err_internal;

                    SCANNER_DBG_PRINT("Token-name: %s\n", sc_token->name);
                    return sc_token;
                    break;

                case State_ASSIGN:
                    if (c == '=')
                        state = State_EQ;
                    else
                    {
                        state = State_S;
                        sc_unget(c);
                        sc_info.ptr = NULL;
                        sc_token= createToken("=", sc_info);
                        if (sc_token == NULL)
                            goto err_internal;

                        SCANNER_DBG_PRINT("Token-name: %s\n", sc_token->name);
                        return sc_token;
                    }
                    break;

                case State_EQ:
                    state = State_S;
                    sc_unget(c);
                    sc_info.ptr = NULL;
                    sc_token= createToken("==", sc_info);
                    if (sc_token == NULL)
                        goto err_internal;

                    SCANNER_DBG_PRINT("Token-name: %s\n", sc_token->name);
                    return sc_token;
                    break;

                case State_EXLAMATION:
                    if (c == '=')
                        state = State_NEQ;
                    else
                    {
                        goto err_lexical;
                    }
                        
                    break;
                
                case State_NEQ:
                    state = State_S;
                    sc_unget(c);
                    sc_info.ptr = NULL;
                    sc_token= createToken("!=", sc_info);
                    if (sc_token == NULL)
                        goto err_internal;

                    SCANNER_DBG_PRINT("Token-name: %s\n", sc_token->name);
                    return sc_token;
                    break;
                    
                case State_COMMA:
                    state = State_S;
                    sc_unget(c);
                    sc_info.ptr = NULL;
                    sc_token= createToken(",", sc_info);
                    if (sc_token == NULL)
                        goto err_internal;

                    SCANNER_DBG_PRINT("Token-name: %s\n", sc_token->name);
                    return sc_token;
                    break;

                case State_LEFT_BRACKET:
                    state = State_S;
                    sc_unget(c);
                    sc_info.ptr = NULL;
                    sc_token= createToken("(", sc_info);
                    if (sc_token == NULL)
                        goto err_internal;

                    SCANNER_DBG_PRINT("Token-name: %s\n", sc_token->name);
                    return sc_token;
                    break;

                case State_RIGHT_BRACKET:
                    state = State_S;
                    sc_unget(c);
                    sc_info.ptr = NULL;
                    sc_token= createToken(")", sc_info);
                    if (sc_token == NULL)
                        goto err_internal;

                    SCANNER_DBG_PRINT("Token-name: %s\n", sc_token->name);
                    return sc_token;
                    break;

                case State_DIV:
                    state = State_S;
                    sc_unget(c);
                    sc_info.ptr = NULL;
                    sc_token= createToken("/", sc_info);
                    if (sc_token == NULL)
                        goto err_internal;

                    SCANNER_DBG_PRINT("Token-name: %s\n", sc_token->name);
                    return sc_token;
                    break;
                    
                case State_MUL:
                    state = State_S;
                    sc_unget(c);
                    sc_info.ptr = NULL;
                    sc_token= createToken("*", sc_info);
                    if (sc_token == NULL)
                        goto err_internal;

                    SCANNER_DBG_PRINT("Token-name: %s\n", sc_token->name);
                    return sc_token;
                    break;

                case State_MINUS:
                    state = State_S;
                    sc_unget(c);
                    sc_info.ptr = NULL;
                    sc_token= createToken("-", sc_info);
                    if (sc_token == NULL)
                        goto err_internal;

                    SCANNER_DBG_PRINT("Token-name: %s\n", sc_token->name);
                    return sc_token;
                    break;

                case State_PLUS:
                    state = State_S;
                    sc_unget(c);
                    sc_info.ptr = NULL;
                    sc_token= createToken("+", sc_info);
                    if (sc_token == NULL)
                        goto err_internal;

                    SCANNER_DBG_PRINT("Token-name: %s\n", sc_token->name);
                    return sc_token;
                    break;

                case State_Octal:
                    if ( c >= '0' && c <= '7')
                    {
                        if(!dynamicStr_add(sc_str, c))
                            goto err_internal;
                        
                        state = State_Octal;
                    }   
                                //(,),*,+, , ,-         /             <,=,>
                    else if ( (c >= '(' && c <= '-') || c == '/' || (c >= '<' && c <= '>') || isspace(c))    //another characters like: +,-,*,/
                    {
                        state = State_S;
                        sc_unget(c);
                        sc_info.string = malloc( (strlen(sc_str->str) + 1) * sizeof(char) );
                        if ( !(OcttoStr(sc_info.string, sc_str)))   //False = convert wasn't succesful
                            goto err_internal;

                        sc_token= createToken("INT", sc_info);
                        if (sc_token == NULL)
                            goto err_internal;

                        SCANNER_DBG_PRINT("Token-name: %s  || Value : %s\n", sc_token->name, sc_info.string );
                        return sc_token;   
                    }
                    else
                    {
                        sc_unget(c);
                        state = State_ERR;
                    }
                    break;

                case State_Binary:
                    if ( c == '1' || c == '0' )
                    {
                        if(!dynamicStr_add(sc_str, c))
                            goto err_internal;
                        
                        state = State_Binary;
                    }   
                                //(,),*,+, , ,-         /             <,=,>
                    else if ( (c >= '(' && c <= '-') || c == '/' || (c >= '<' && c <= '>') || isspace(c))    //another characters like: +,-,*,/
                    {
                        state = State_S;
                        sc_unget(c);
                        sc_info.string = malloc( (strlen(sc_str->str) + 1) * sizeof(char) );
                        if ( !(BintoStr(sc_info.string, sc_str)))   //False = convert wasn't succesful
                            goto err_internal;

                        sc_token= createToken("INT", sc_info);
                        if (sc_token == NULL)
                            goto err_internal;

                        SCANNER_DBG_PRINT("Token-name: %s  || Value : %s\n", sc_token->name, sc_info.string );
                        return sc_token;   
                    }
                    else
                    {
                        sc_unget(c);
                        state = State_ERR;
                    }
                    break;

                case State_Backslash:
                    if (c == 'x')
                    {
                        state = State_Hexidecimal;
                    }
                    else
                    {
                        sc_unget(c);
                        state = State_ERR;
                    }
                    break;

                case State_Hexidecimal:
                    if ( isdigit(c) || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'))
                    {
                        if(!dynamicStr_add(sc_str, c))
                            goto err_internal;
                        
                        state = State_Hexidecimal;
                    }   
                                //(,),*,+, , ,-         /             <,=,>
                    else if ( (c >= '(' && c <= '-') || c == '/' || (c >= '<' && c <= '>') || isspace(c))    //another characters like: +,-,*,/
                    {
                        state = State_S;
                        sc_unget(c);
                        sc_info.string = malloc( (strlen(sc_str->str) + 1) * sizeof(char) );
                        if ( !(HextoStr(sc_info.string, sc_str)))   //False = convert wasn't succesful
                            goto err_internal;

                        sc_token= createToken("INT", sc_info);
                        if (sc_token == NULL)
                            goto err_internal;

                        SCANNER_DBG_PRINT("Token-name: %s  || Value : %s\n", sc_token->name, sc_info.string );
                        return sc_token;   
                    }
                    else
                    {
                        sc_unget(c);
                        state = State_ERR;
                    }
                    break;

                case State_ERR:
                    if ( c != '\n')         //while not a new line stays in this state
                        state = State_ERR;
                    else                    //new line ==> generate ERR_token - lexical
                    {
                        state = State_S;
                        goto err_lexical;  
                    }
                    break;
            }
        }

        if ((c=getc(stdin)) == EOF)
        {   
            switch (state)
            {
                case State_S:
                    sc_info.ptr = NULL;
                    sc_token= createToken("EOF", sc_info);
                    if (sc_token == NULL)
                        goto err_internal;

                    SCANNER_DBG_PRINT("Token-name: %s\n", sc_token->name);
                    return sc_token;
    

                case State_EOL:
                    state = State_S;
                    sc_info.ptr = NULL;
                    sc_token= createToken("EOL", sc_info);
                    if (sc_token == NULL)
                        goto err_internal;

                    SCANNER_DBG_PRINT("Token-name: %s\n", sc_token->name);
                    return sc_token;  
                

                case State_END_COMMEND:
                    state = State_S;
                    sc_info.ptr = NULL;
                    sc_token = createToken("EOL", sc_info);
                    if (sc_token == NULL)
                        goto err_internal;

                    SCANNER_DBG_PRINT("Token-name: %s\n", sc_token->name);
                    return sc_token;

                case State_LCOMM:
                    state = State_S;
                    sc_info.ptr = NULL;
                    sc_token= createToken("EOL", sc_info);
                    if (sc_token == NULL)
                        goto err_internal;

                    SCANNER_DBG_PRINT("Token-name: %s\n", sc_token->name);
                    return sc_token; 

                case State_QUATATION2:
                    state = State_S;
                    // allocate space for string which will be stored in token info
                    sc_info.string = malloc( (strlen(sc_str->str) + 1) * sizeof(char) );
                    // copy string from dynamicStr to token info
                    strcpy(sc_info.string, sc_str->str);
                    sc_token = createToken("STR", sc_info);
                    if (sc_token == NULL)
                        goto err_internal;

                    SCANNER_DBG_PRINT("Token-name: %s  || Value : %s\n", sc_token->name, sc_str->str );
                    return sc_token;  

                case State_ID:
                    state = State_S;
                    char *name = "ID";
                    if (strcmp(inKeyword(sc_str->str, keywords),"ID") != 0 )
                        {
                            name = inKeyword(sc_str->str, keywords);
                            sc_info.ptr = NULL;
                            sc_token= createToken(name, sc_info);
                            if (sc_token == NULL)
                                goto err_internal;
    
                            SCANNER_DBG_PRINT("Token-name: %s\n", sc_token->name);
                            return sc_token; 
                        }
                    else if (strcmp(inBuiltin_Func(sc_str->str, builtin_func),"ID") != 0)   //Token:"BUILTIN", atribut: "name_of_func"
                        {
                            name = inBuiltin_Func(sc_str->str, builtin_func);
                            sc_info.string = malloc(sizeof(char)* (strlen(name)+1));
                            strcpy(sc_info.string, name);
                            sc_token= createToken("BUILTIN", sc_info);
                            if (sc_token == NULL)
                                goto err_internal;

                            SCANNER_DBG_PRINT("Token-name: %s || Value : %s\n", sc_token->name, sc_token->info.string );
                            return sc_token; 
                        }
                    else
                    {
                        sc_info.ptr = NULL;  
                        sc_token= createToken(name, sc_info);
                        if (sc_token == NULL)
                            goto err_internal;

                        SCANNER_DBG_PRINT("Token-name: %s\n", sc_token->name);
                        return sc_token;  
                    }

                case State_IDEXL:
                    if(!dynamicStr_add(sc_str, '!'))
                        goto err_internal;

                    token_info_t sc_info2;
                    sc_info2.ptr = NULL;
                    token_t *sc_token2 = createToken("EOL", sc_info2);
                    que_up(que, sc_token2, sc_str->str);

                    state = State_S;
                    sc_info.ptr = NULL;
                    sc_token= createToken("FUNC", sc_info);
                    if (sc_token == NULL)
                        goto err_internal;

                    SCANNER_DBG_PRINT("Token-name: %s \n", sc_token->name);
                    return sc_token;  

                case State_FUNC:
                    state = State_S;
                    sc_info.ptr = NULL;
                    sc_token= createToken("FUNC", sc_info);
                    if (sc_token == NULL)
                        goto err_internal;

                    SCANNER_DBG_PRINT("Token-name: %s \n", sc_token->name);
                    return sc_token; 

                case State_INT:
                    state = State_S;
                    sc_info.string = malloc( (strlen(sc_str->str) + 1) * sizeof(char) ); 
                    if ( !(InttoStr(sc_info.string, sc_str)))   //False = convert wasn't succesful
                        goto err_internal;

                    sc_token= createToken("INT", sc_info);
                    if (sc_token == NULL)
                        goto err_internal;

                    SCANNER_DBG_PRINT("Token-name: %s  || Value : %s\n", sc_token->name, sc_info.string );
                    return sc_token; 

                case State_INT0:
                    state = State_S;
                    sc_info.string = malloc( (strlen(sc_str->str) + 1) * sizeof(char) ); 
                    if ( !(InttoStr(sc_info.string, sc_str)))   //False = convert wasn't succesful
                        goto err_internal;

                    sc_token= createToken("INT", sc_info);
                    if (sc_token == NULL)
                        goto err_internal;

                    SCANNER_DBG_PRINT("Token-name: %s  ||  Value :%s\n", sc_token->name, sc_info.string);
                    return sc_token;  

                case State_FLOAT:
                    state = State_S;
                    sc_info.string = malloc( 25 * sizeof(char) ); 
                    if ( !(DobtoStr(sc_info.string, sc_str)))   //False = convert wasn't succesful
                            goto err_internal;

                    sc_token= createToken("DBL", sc_info);
                    if (sc_token == NULL)
                        goto err_internal;

                    SCANNER_DBG_PRINT("Token-name: %s  || Value : %s\n", sc_token->name, sc_info.string);;
                    return sc_token;   

                case State_LTN:
                        state = State_S;
                        sc_info.ptr = NULL;
                        sc_token= createToken("<", sc_info);
                        if (sc_token == NULL)
                            goto err_internal;

                        SCANNER_DBG_PRINT("Token-name: %s\n", sc_token->name);
                        return sc_token;

                case State_LEQ:
                    state = State_S;
                    sc_info.ptr = NULL;
                    sc_token= createToken("<=", sc_info);
                    if (sc_token == NULL)
                        goto err_internal;

                    SCANNER_DBG_PRINT("Token-name: %s\n", sc_token->name);
                    return sc_token;

                case State_MTN:
                        state = State_S;
                        sc_info.ptr = NULL;
                        sc_token= createToken(">", sc_info);
                        if (sc_token == NULL)
                            goto err_internal;

                        SCANNER_DBG_PRINT("Token-name: %s\n", sc_token->name);
                        return sc_token;

                case State_MEQ:
                    state = State_S;
                    sc_info.ptr = NULL;
                    sc_token= createToken(">=", sc_info);
                    if (sc_token == NULL)
                        goto err_internal;

                    SCANNER_DBG_PRINT("Token-name: %s\n", sc_token->name);
                    return sc_token;

                case State_ASSIGN:
                    state = State_S;
                    sc_info.ptr = NULL;
                    sc_token= createToken("=", sc_info);
                    if (sc_token == NULL)
                        goto err_internal;

                    SCANNER_DBG_PRINT("Token-name: %s\n", sc_token->name);
                    return sc_token;

                case State_EQ:
                    state = State_S;
                    sc_info.ptr = NULL;
                    sc_token= createToken("==", sc_info);
                    if (sc_token == NULL)
                        goto err_internal;

                    SCANNER_DBG_PRINT("Token-name: %s\n", sc_token->name);
                    return sc_token;

                case State_NEQ:
                    state = State_S;
                    sc_info.ptr = NULL;
                    sc_token= createToken("!=", sc_info);
                    if (sc_token == NULL)
                        goto err_internal;

                    SCANNER_DBG_PRINT("Token-name: %s\n", sc_token->name);
                    return sc_token;
                    
                case State_COMMA:
                    state = State_S;
                    sc_info.ptr = NULL;
                    sc_token= createToken(",", sc_info);
                    if (sc_token == NULL)
                        goto err_internal;

                    SCANNER_DBG_PRINT("Token-name: %s\n", sc_token->name);
                    return sc_token;

                case State_LEFT_BRACKET:
                    state = State_S;
                    sc_info.ptr = NULL;
                    sc_token= createToken("(", sc_info);
                    if (sc_token == NULL)
                        goto err_internal;

                    SCANNER_DBG_PRINT("Token-name: %s\n", sc_token->name);
                    return sc_token;

                case State_RIGHT_BRACKET:
                    state = State_S;
                    sc_info.ptr = NULL;
                    sc_token= createToken(")", sc_info);
                    if (sc_token == NULL)
                        goto err_internal;

                    SCANNER_DBG_PRINT("Token-name: %s\n", sc_token->name);
                    return sc_token;

                case State_DIV:
                    state = State_S;
                    sc_info.ptr = NULL;
                    sc_token= createToken("/", sc_info);
                    if (sc_token == NULL)
                        goto err_internal;

                    SCANNER_DBG_PRINT("Token-name: %s\n", sc_token->name);
                    return sc_token;
                    
                case State_MUL:
                    state = State_S;
                    sc_info.ptr = NULL;
                    sc_token= createToken("*", sc_info);
                    if (sc_token == NULL)
                        goto err_internal;

                    SCANNER_DBG_PRINT("Token-name: %s\n", sc_token->name);
                    return sc_token;

                case State_MINUS:
                    state = State_S;
                    sc_info.ptr = NULL;
                    sc_token= createToken("-", sc_info);
                    if (sc_token == NULL)
                        goto err_internal;

                    SCANNER_DBG_PRINT("Token-name: %s\n", sc_token->name);
                    return sc_token;

                case State_PLUS:
                    state = State_S;
                    sc_info.ptr = NULL;
                    sc_token= createToken("+", sc_info);
                    if (sc_token == NULL)
                        goto err_internal;

                    SCANNER_DBG_PRINT("Token-name: %s\n", sc_token->name);
                    return sc_token;

                default:
                    goto err_lexical;
            }
        }
    }
    else
    {
        state = State_S;
        char *name;
        token_t *sc_token3 = que_get(que, &name);
        // load back to dynamic string
        int i = 0;
        while (name[i] != '\0')
        {
            if (! dynamicStr_add(sc_str, name[i]))
                goto err_internal; 
            i++;
        }
        free(name);

        SCANNER_DBG_PRINT("Token-name: %s\n", sc_token3->name);
        return sc_token3;
    }

///////////////////////////////////////
///         ERROR HANDLING          ///
///////////////////////////////////////
err_lexical:
    sc_unget(c); 
    sc_info.ptr = NULL;
    sc_token = createToken("ERR_LEX", sc_info);
    SCANNER_DBG_PRINT("Token-name: %s\n", sc_token->name);
    //exit(1);
    return sc_token;

err_internal:
    sc_info.ptr = NULL;
    sc_token = createToken("ERR_INTERNAL", sc_info);
    SCANNER_DBG_PRINT("Token-name: %s\n", sc_token->name);
    //exit(99);
    return sc_token;
}
////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////
