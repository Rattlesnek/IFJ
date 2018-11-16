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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "token.h"
#include "dynamicStr.h"
#include "error.h"
#include "queue.h"
#include "symtable.h"

#include "scanner.h"

////////////////////////////////////////////////////////////////////////
///                       GLOBAL VARIABLES                           ///
////////////////////////////////////////////////////////////////////////
int state = State_S;
char *keywords[] = {"def", "do", "else", "end", "if", "not", "nil", "then", "while", "elif"};
int KeywordLen = 10;

////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
///////////////////////////////////////////////////////////////////////

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
 * @brief Check if ID is keywords
 * 
 * @param str string to be compared with keywords
 * @param keywords 
 * @return char* return token.name
 */
char* inKeyword(char *str, char **keywords)
{
    for (int i = 0; i < KeywordLen; i++)
    {
        if (strcmp(keywords[i], str) == 0)
            return keywords[i];
    }
   
    return "ID";
}


bool scanner_unget(queue_t *que, token_t *token, char *name)
{
    return que_up(que, token, name);
}


/**
 * @brief Scanner of the input, generates tokens (Lexical analysis)
 * 
 * @param c character to be read
 * @return token_t* token to be returned
 */
token_t* scanner_get(dynamicStr_t *sc_str, queue_t *que)
{
    dynamicStr_clear(sc_str);
    token_info_t sc_info;
    token_t *sc_token;
    int c = 0;
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
                    {
                        state = State_MINUS;
                        if(!dynamicStr_add(sc_str, c)) 
                            goto err_internal;
                    }
                        
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
                        
                    else if (isalpha(c) || c == '_')
                    {
                        if(!dynamicStr_add(sc_str, c))
                            goto err_internal;

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
                        sc_info.ptr = NULL;
                        sc_token= createToken("EOL", sc_info);
                        if (sc_token == NULL)
                            goto err_internal;

                        printf("Token-name: %s\n", sc_token->name);
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

                        printf("Token-name: %s\n", sc_token->name);
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

                        printf("Token-name: %s\n", sc_token->name);
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
                    else 
                    {
                        if(!dynamicStr_add(sc_str, c))
                            goto err_internal;

                        state = State_QUATATION1;
                    }
                    break;

                case State_SPEC_CHAR:
                    if (c == 'n' || c == 't' || c == 's' || c == '\\')
                    {   
                        if(!dynamicStr_add(sc_str, '\\'))   //First I add '\\'
                            goto err_internal;

                        if(!dynamicStr_add(sc_str, c))
                            goto err_internal;

                        state = State_QUATATION1;
                    }
                    else if (c == 'x')
                    {
                        if(!dynamicStr_add(sc_str, '\\')) //First I add '\\'
                            goto err_internal;

                        if(!dynamicStr_add(sc_str, c))
                            goto err_internal;

                        state = State_HEX;
                    }
                    else
                        goto err_lexical;
                        
                    break;

                case State_HEX:
                    if ((c >= 'A' && c <= 'F') || isdigit(c))
                    {
                        if(!dynamicStr_add(sc_str, c))
                            goto err_internal;

                        state = State_HEX_NUM;
                    } 
                    else
                        goto err_lexical;
                    break;

                case State_HEX_NUM:
                    if ((c == 'A' && c <= 'F') || isdigit(c))
                    {
                        if(!dynamicStr_add(sc_str, c))
                            goto err_internal;

                        state = State_HEX_NUM_END;
                    }
                    else if (c == '\n')
                        goto err_lexical;

                    else
                    {
                        if(!dynamicStr_add(sc_str, c))
                            goto err_internal;

                        state = State_QUATATION1;
                    }
                    break;

                case State_HEX_NUM_END:
                    if (c == '\n') 
                        goto err_lexical;

                    else
                    {
                        if(!dynamicStr_add(sc_str, c))
                            goto err_internal;

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

                    printf("Token-name: %s  || Value : %s\n", sc_token->name, sc_str->str );
                    
                    return sc_token;  

    ///////////////////////////////////////
    ///        ID, FUNC INT, FLOAT      ///
    ///////////////////////////////////////
                case State_ID:
                    if (isalpha(c) || isdigit(c))
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

                    else 
                    {
                        state = State_S;
                        sc_unget(c);
                        char *name = "ID";
                        if (strcmp(inKeyword(sc_str->str, keywords),"ID") != 0 )
                            {
                                name = inKeyword(sc_str->str, keywords);
                                sc_info.ptr = NULL;
                                sc_token= createToken(name, sc_info);
                                if (sc_token == NULL)
                                    goto err_internal;

                                printf("Token-name: %s\n", sc_token->name); 
                                return sc_token; 
                            }
                        else
                        {
                            sc_info.ptr = NULL;
                            sc_token= createToken("ID", sc_info);
                            if (sc_token == NULL)
                                goto err_internal;
    
                            printf("Token-name: %s\n", sc_token->name);
                            return sc_token;  
                        }
                    
                    }
                    break;

                case State_IDEXL:
                    if ( c == '\n')
                    {
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

                        printf("Token-name: %s \n", sc_token->name);
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

                        printf("Token-name: %s\n", sc_token->name);
                        return sc_token;  
                    }

                    else
                    {
                        sc_unget(c);
                        state = State_FUNC;
                    }
                    break;

                case State_FUNC:
                    state = State_S;
                    sc_unget(c);
                    sc_info.ptr = NULL;
                    sc_token= createToken("FUNC", sc_info);
                    if (sc_token == NULL)
                        goto err_internal;

                    printf("Token-name: %s \n", sc_token->name);
                    return sc_token;  


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

                    else
                    {
                        state = State_S;
                        sc_unget(c);
                        sc_info.intg = toInt(sc_str); 
                        sc_token= createToken("INT", sc_info);
                        if (sc_token == NULL)
                            goto err_internal;

                        printf("Token-name: %s  || Value : %d\n", sc_token->name, sc_info.intg );
                        return sc_token;   
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

                    else if (isspace(c))
                    {
                        state = State_S;
                        sc_unget(c);
                        sc_info.intg = 0;
                        sc_token= createToken("INT", sc_info);
                        if (sc_token == NULL)
                            goto err_internal;

                        printf("Token-name: %s\n", sc_token->name);
                        return sc_token;  
                    }
                    else
                        goto err_lexical;
                    break;

                case State_DOT:
                    if (isdigit(c))
                    {
                        if(!dynamicStr_add(sc_str, c))
                            goto err_internal;
                        
                        state = State_FLOAT;
                    }
                    else
                        goto err_lexical;

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
                        goto err_lexical;

                    break;

                case State_EXPS:
                    if (isdigit(c))
                    {
                        if(!dynamicStr_add(sc_str, c))
                            goto err_internal;
                        
                        state = State_FLOAT;
                    }
                    else 
                        goto err_lexical;

                    break;

                case State_FLOAT:
                    if (isdigit(c))
                    {
                        if(!dynamicStr_add(sc_str, c))
                            goto err_internal;
                        
                        state = State_FLOAT;
                    }
                    else
                    {
                        state = State_S;
                        sc_unget(c);
                        sc_info.dbl = toDouble(sc_str); 
                        sc_token= createToken("DBL", sc_info);
                        if (sc_token == NULL)
                            goto err_internal;

                        printf("Token-name: %s  || Value : %f\n", sc_token->name, sc_info.dbl );
                        return sc_token;   
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

                        printf("Token-name: %s\n", sc_token->name);
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

                    printf("Token-name: %s\n", sc_token->name);
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

                        printf("Token-name: %s\n", sc_token->name);
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

                    printf("Token-name: %s\n", sc_token->name);
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

                        printf("Token-name: %s\n", sc_token->name);
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

                    printf("Token-name: %s\n", sc_token->name);
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

                    printf("Token-name: %s\n", sc_token->name);
                    return sc_token;
                    break;
                    
                case State_COMMA:
                    state = State_S;
                    sc_unget(c);
                    sc_info.ptr = NULL;
                    sc_token= createToken(",", sc_info);
                    if (sc_token == NULL)
                        goto err_internal;

                    printf("Token-name: %s\n", sc_token->name);
                    return sc_token;
                    break;

                case State_LEFT_BRACKET:
                    state = State_S;
                    sc_unget(c);
                    sc_info.ptr = NULL;
                    sc_token= createToken("(", sc_info);
                    if (sc_token == NULL)
                        goto err_internal;

                    printf("Token-name: %s\n", sc_token->name);
                    return sc_token;
                    break;

                case State_RIGHT_BRACKET:
                    state = State_S;
                    sc_unget(c);
                    sc_info.ptr = NULL;
                    sc_token= createToken(")", sc_info);
                    if (sc_token == NULL)
                        goto err_internal;

                    printf("Token-name: %s\n", sc_token->name);
                    return sc_token;
                    break;

                case State_DIV:
                    state = State_S;
                    sc_unget(c);
                    sc_info.ptr = NULL;
                    sc_token= createToken("/", sc_info);
                    if (sc_token == NULL)
                        goto err_internal;

                    printf("Token-name: %s\n", sc_token->name);
                    return sc_token;
                    break;
                    
                case State_MUL:
                    state = State_S;
                    sc_unget(c);
                    sc_info.ptr = NULL;
                    sc_token= createToken("*", sc_info);
                    if (sc_token == NULL)
                        goto err_internal;

                    printf("Token-name: %s\n", sc_token->name);
                    return sc_token;
                    break;

                case State_MINUS:
                    state = State_S;
                    sc_unget(c);
                    sc_info.ptr = NULL;
                    sc_token= createToken("-", sc_info);
                    if (sc_token == NULL)
                        goto err_internal;

                    printf("Token-name: %s\n", sc_token->name);
                    return sc_token;
                    break;

                case State_PLUS:
                    state = State_S;
                    sc_unget(c);
                    sc_info.ptr = NULL;
                    sc_token= createToken("+", sc_info);
                    if (sc_token == NULL)
                        goto err_internal;

                    printf("Token-name: %s\n", sc_token->name);
                    return sc_token;
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

                    printf("Token-name: %s\n", sc_token->name);
                    return sc_token;
    

                case State_EOL:
                    state = State_S;
                    sc_info.ptr = NULL;
                    sc_token= createToken("EOL", sc_info);
                    if (sc_token == NULL)
                        goto err_internal;

                    printf("Token-name: %s\n", sc_token->name);
                    return sc_token;  
                

                case State_END_COMMEND:
                    state = State_S;
                    sc_info.ptr = NULL;
                    sc_token = createToken("EOL", sc_info);
                    if (sc_token == NULL)
                        goto err_internal;

                    printf("Token-name: %s\n", sc_token->name);
                    return sc_token;

                case State_LCOMM:
                    state = State_S;
                    sc_info.ptr = NULL;
                    sc_token= createToken("EOL", sc_info);
                    if (sc_token == NULL)
                        goto err_internal;

                    printf("Token-name: %s\n", sc_token->name);
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

                    printf("Token-name: %s  || Value : %s\n", sc_token->name, sc_str->str );
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
    
                            printf("Token-name: %s\n", sc_token->name);
                            return sc_token; 
                        }
                    else
                    {
                        sc_info.ptr = NULL;  
                        sc_token= createToken(name, sc_info);
                        if (sc_token == NULL)
                            goto err_internal;

                        printf("Token-name: %s\n", sc_token->name);
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

                    printf("Token-name: %s \n", sc_token->name);
                    return sc_token;  

                case State_FUNC:
                    state = State_S;
                    sc_info.ptr = NULL;
                    sc_token= createToken("FUNC", sc_info);
                    if (sc_token == NULL)
                        goto err_internal;

                    printf("Token-name: %s \n", sc_token->name);
                    return sc_token; 

                case State_INT:
                    state = State_S;
                    sc_info.intg = toInt(sc_str); 
                    sc_token= createToken("INT", sc_info);
                    if (sc_token == NULL)
                        goto err_internal;

                    printf("Token-name: %s  || Value : %d\n", sc_token->name, sc_info.intg );
                    return sc_token; 

                case State_INT0:
                    state = State_S;
                    sc_info.intg = 0;
                    sc_token= createToken("INT", sc_info);
                    if (sc_token == NULL)
                        goto err_internal;

                    printf("Token-name: %s\n", sc_token->name);
                    return sc_token;  

                case State_FLOAT:
                    state = State_S;
                    sc_info.dbl = toDouble(sc_str); 
                    sc_token= createToken("DBL", sc_info);
                    if (sc_token == NULL)
                        goto err_internal;

                    printf("Token-name: %s  || Value : %f\n", sc_token->name, sc_info.dbl );;
                    return sc_token;   

                case State_LTN:
                        state = State_S;
                        sc_info.ptr = NULL;
                        sc_token= createToken("<", sc_info);
                        if (sc_token == NULL)
                            goto err_internal;

                        printf("Token-name: %s\n", sc_token->name);
                        return sc_token;

                case State_LEQ:
                    state = State_S;
                    sc_info.ptr = NULL;
                    sc_token= createToken("<=", sc_info);
                    if (sc_token == NULL)
                        goto err_internal;

                    printf("Token-name: %s\n", sc_token->name);
                    return sc_token;

                case State_MTN:
                        state = State_S;
                        sc_info.ptr = NULL;
                        sc_token= createToken(">", sc_info);
                        if (sc_token == NULL)
                            goto err_internal;

                        printf("Token-name: %s\n", sc_token->name);
                        return sc_token;

                case State_MEQ:
                    state = State_S;
                    sc_info.ptr = NULL;
                    sc_token= createToken(">=", sc_info);
                    if (sc_token == NULL)
                        goto err_internal;

                    printf("Token-name: %s\n", sc_token->name);
                    return sc_token;

                case State_ASSIGN:
                    state = State_S;
                    sc_info.ptr = NULL;
                    sc_token= createToken("=", sc_info);
                    if (sc_token == NULL)
                        goto err_internal;

                    printf("Token-name: %s\n", sc_token->name);
                    return sc_token;

                case State_EQ:
                    state = State_S;
                    sc_info.ptr = NULL;
                    sc_token= createToken("==", sc_info);
                    if (sc_token == NULL)
                        goto err_internal;

                    printf("Token-name: %s\n", sc_token->name);
                    return sc_token;

                case State_NEQ:
                    state = State_S;
                    sc_info.ptr = NULL;
                    sc_token= createToken("!=", sc_info);
                    if (sc_token == NULL)
                        goto err_internal;

                    printf("Token-name: %s\n", sc_token->name);
                    return sc_token;
                    
                case State_COMMA:
                    state = State_S;
                    sc_info.ptr = NULL;
                    sc_token= createToken(",", sc_info);
                    if (sc_token == NULL)
                        goto err_internal;

                    printf("Token-name: %s\n", sc_token->name);
                    return sc_token;

                case State_LEFT_BRACKET:
                    state = State_S;
                    sc_info.ptr = NULL;
                    sc_token= createToken("(", sc_info);
                    if (sc_token == NULL)
                        goto err_internal;

                    printf("Token-name: %s\n", sc_token->name);
                    return sc_token;

                case State_RIGHT_BRACKET:
                    state = State_S;
                    sc_info.ptr = NULL;
                    sc_token= createToken(")", sc_info);
                    if (sc_token == NULL)
                        goto err_internal;

                    printf("Token-name: %s\n", sc_token->name);
                    return sc_token;

                case State_DIV:
                    state = State_S;
                    sc_info.ptr = NULL;
                    sc_token= createToken("/", sc_info);
                    if (sc_token == NULL)
                        goto err_internal;

                    printf("Token-name: %s\n", sc_token->name);
                    return sc_token;
                    
                case State_MUL:
                    state = State_S;
                    sc_info.ptr = NULL;
                    sc_token= createToken("*", sc_info);
                    if (sc_token == NULL)
                        goto err_internal;

                    printf("Token-name: %s\n", sc_token->name);
                    return sc_token;

                case State_MINUS:
                    state = State_S;
                    sc_info.ptr = NULL;
                    sc_token= createToken("-", sc_info);
                    if (sc_token == NULL)
                        goto err_internal;

                    printf("Token-name: %s\n", sc_token->name);
                    return sc_token;

                case State_PLUS:
                    state = State_S;
                    sc_info.ptr = NULL;
                    sc_token= createToken("+", sc_info);
                    if (sc_token == NULL)
                        goto err_internal;

                    printf("Token-name: %s\n", sc_token->name);
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

        printf("Token-name: %s\n", sc_token3->name);
        return sc_token3;
    }

///////////////////////////////////////
///         ERROR HANDLING          ///
///////////////////////////////////////
err_lexical:
    sc_unget(c);
    dynamicStr_clear(sc_str); 
    sc_info.ptr = NULL;
    sc_token = createToken("ERR_LEX", sc_info);
    return sc_token;

err_internal:
    dynamicStr_free(sc_str);
    sc_info.ptr = NULL;
    sc_token = createToken("ERR_INTERNAL", sc_info);
   return sc_token;
}
////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////
