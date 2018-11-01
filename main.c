#include <stdio.h>

#include "token.h"
#include "symtable.h"
#include "scanner.h"
#include "dynamicStr.h"
#include "queue.h"

int main()
{
    dynamicStr_t str;
    dynamicStr_t *sc_str = &str;
    dynamicStr_init(sc_str);

    queue_t *que = que_create();

    symtable_t *symtable = symtab_init(10);

    for ( int i = 0; i < 40; i++)
        scanner(sc_str, que, symtable);

    dynamicStr_free(sc_str);

    que_destroy(que);

    symtab_free(symtable);
    return 0;
}