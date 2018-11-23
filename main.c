#include <stdio.h>
#include <string.h>

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

#include "builtin_gen.h"

#include "error.h"
int main()
{
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
        if (strcmp(token->name, "BUILTIN") == 0)
        {
           // printf("builtin namen : %s\n", token->info.string);
            token_t *bra = scanner_get(sc_str, que);
            if (strcmp(token->info.string, "chr") == 0)
            {
                token_t *in = scanner_get(sc_str, que);
                //printf("token namen : %s\n", in->name);
                chr(gl_var_tab, in);
            }
            else if (strcmp(token->info.string, "ord") == 0)
            {
                token_t *str = scanner_get(sc_str, que);
                token_t *coma = scanner_get(sc_str, que);
                token_t *position = scanner_get(sc_str, que);
                if( strcmp(bra->name, "(") == 0)
                {
                   // printf("token namen : %s\n", str->name);
                   // printf("token namen : %s\n", position->name);
                    ord(gl_var_tab, str, position);
                }
                else{
                   // printf("token namen : %s\n", str->name);
                   // printf("token namen : %s\n", position->name);
                    ord(gl_var_tab, bra, coma);
                }
                
            }
            destroyToken(bra);
        }

    } while (strcmp(token->name, "EOF") != 0);

    destroyToken(token);

    dynamicStr_free(sc_str);
    que_destroy(que);

    return 0;
}