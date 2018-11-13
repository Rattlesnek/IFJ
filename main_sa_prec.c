#include <stdio.h>
#include <string.h>

#include "token.h"
#include "symtable.h"
#include "scanner.h"
#include "stack.h"
#include "dynamicStr.h"
#include "dynamicArrInt.h"
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
    symtable_t *symtable = symtab_init(10);

    if(!sa_prec(sc_str, que, symtable))
        printf("=> sa_prec: Something is wrong, I guess...\n");
    else
        printf("=> sa_prec: The expression is OK\n");

    dynamicStr_free(sc_str);
    que_destroy(que);
    symtab_free(symtable);

    return 0;
}