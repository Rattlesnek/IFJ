/**CFile****************************************************************

  FileName    [token.h]

  SystemName  [IFJ - PROJECT]

  PackageName [Lexical analysis]

  Synopsis    [Token manipulation]

  Author      [Adam Pankuch]

  Affiliation []

  Date        [18/10/2018]

  Revision    []

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                          INCLUDES                                ///
////////////////////////////////////////////////////////////////////////
#ifndef TOKEN_H
#define TOKEN_H


////////////////////////////////////////////////////////////////////////
///                         BASIC TYPES                              ///
////////////////////////////////////////////////////////////////////////

/****************************************************/
// HASH TABLE ELEMENT PROTOTYPE
// vymazat ked bude elem_t v hash table vytvoreny
typedef struct element elem_t;

struct element {
    //...........
    //...........
    elem_t *ptr;
};
/****************************************************/

typedef union token_info token_info_t;

typedef struct token token_t;


union token_info {
    elem_t *ptr;    // either ptr to info-element in hash table (use in all cases except latter two)
    int intg;       // or integer (use only when token name is "INT")
    double dbl;     // or double (use only when token name is "DBL")
};

struct token {
    char *name;         // token name eg. "ID", "STR", "+", "==", "EOL", ...
    token_info_t info;  // token info 
};


////////////////////////////////////////////////////////////////////////
///                      MACRO DEFINITIONS                           ///
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
///                    FUNCTION DECLARATIONS                         ///
////////////////////////////////////////////////////////////////////////

/**
 * @brief Creates token and initializes it
 * 
 * @param name token name eg. "ID", "STR", "+", "==", "EOL", ...
 * @param info either info or ptr to corresponding info-element in hashtable
 * @return token_t* initialized token or NULL if not succesful
 */
token_t *tk_CreateToken(char *name, token_info_t info);


/**
 * @brief Destroys token
 * 
 * @param token token to be destroyed
 */
void tk_DestroyToken(token_t *token);


#endif
////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////