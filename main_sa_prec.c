#include <stdio.h>
#include <string.h>

#include "token.h"
#include "symtable.h"
#include "scanner.h"
#include "stackTkn.h"
#include "dynamicStr.h"
#include "dynamicArrParam.h"
#include "queue.h"
#include "parser.h"
#include "sa_prec.h"

#include "error.h"

int main()
{
    dynamicStr_t str;
    dynamicStr_t *sc_str = &str;

    dynamicStr_init(sc_str);
    queue_t *que = que_create();
    symtable_t *lsymtable = symtab_init(20, VARIABLES);
    symtable_t *fsymtable = symtab_init(20, FUNCTIONS);

    if(!sa_prec(sc_str, que, lsymtable, fsymtable))
        printf("=> sa_prec: Something is wrong, I guess...\n");
    else
        printf("=> sa_prec: The expression is OK\n");

    dynamicStr_free(sc_str);
    que_destroy(que);
    symtab_free(lsymtable);
    symtab_free(fsymtable);

    return 0;
}