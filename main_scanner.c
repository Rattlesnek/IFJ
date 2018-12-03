#include <stdio.h>
#include <string.h>

#include <stdbool.h>

#include "parser.h"

#include "scanner.h"
#include "sa_prec.h"

#include "dynamic_arr_param.h"
#include "dynamic_str.h"
#include "stack_str.h"
#include "stack_tkn.h"
#include "queue.h"

#include "token.h"
#include "symtable.h"

#include "error.h"
int main()
{
    //printf(".IFJcode18\n");
    dynamicStr_t str;
    dynamicStr_t *sc_str = &str;

    dynamicStr_init(sc_str);
    queue_t *que = que_create();
    ///////////////////////////////////////////////
    symtable_t *gl_var_tab = symtab_init(NULL, VARIABLES);
    gl_var_tab->name = malloc( (strlen("$GT") + 1) * sizeof(char));
    strcpy(gl_var_tab->name, "$GT");
    /////////////////////////////////////////////////
    token_t *token = NULL;

    do {
        if (token != NULL)
            destroyToken(token);

        token = scanner_get(sc_str, que);
       /* if (strcmp(token->name, "BUILTIN") == 0)
        {
           // printf("builtin namen : %s\n", token->info.string);
            token_t *bra = scanner_get(sc_str, que);
            if (strcmp(token->info.string, "chr") == 0)
            {
                token_t *in = scanner_get(sc_str, que);
                //printf("token namen : %s\n", in->name);
                token_t *ret = chr(gl_var_tab, in);
                destroyToken(bra);
                destroyToken(ret);
            }
            else if (strcmp(token->info.string, "ord") == 0)
            {
                token_t *str = scanner_get(sc_str, que);
                token_t *coma = scanner_get(sc_str, que);
                token_t *position = scanner_get(sc_str, que);
                if( strcmp(bra->name, "(") == 0)
                {
                    token_t *ret = ord(gl_var_tab, str, position);

                    destroyToken(ret);
                    destroyToken(bra);
                    destroyToken(coma);
                }
                else{
                   // printf("token namen : %s\n", str->name);
                   // printf("token namen : %s\n", position->name);
                    token_t *ret = ord(gl_var_tab, bra, coma);
                    destroyToken(ret);
                    destroyToken(str);
                    destroyToken(position);
                }
                
            }
            else if (strcmp(token->info.string, "substr") == 0)
            {
                token_t *str = scanner_get(sc_str, que);
                token_t *coma = scanner_get(sc_str, que);
                token_t *idx = scanner_get(sc_str, que);
                token_t *coma2 = scanner_get(sc_str, que);
                token_t *end = scanner_get(sc_str, que);
                if( strcmp(bra->name, "(") == 0)
                {
                    token_t *ret = substr(gl_var_tab, str, idx, end);
                    destroyToken(bra);
                    destroyToken(coma);
                    destroyToken(coma2);
                    destroyToken(ret);
                }   
                else{
                    token_t *ret = substr(gl_var_tab, bra,coma, coma2);
                    destroyToken(str);
                    destroyToken(idx);
                    destroyToken(end);
                    destroyToken(ret);
                }
            }
        }*/

    } while (strcmp(token->name, "EOF") != 0);

    destroyToken(token);

    dynamicStr_free(sc_str);
    que_destroy(que);
    symtab_free(gl_var_tab);

    return 0;
}