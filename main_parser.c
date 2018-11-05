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

#include "error.h"

dynamicArrInt_t left_pars; // left analysis of program

int main()
{
    dynamicStr_t str;
    dynamicStr_t *sc_str = &str;
    
    dynamicStr_init(sc_str);
    queue_t *que = que_create();
    symtable_t *symtable = symtab_init(10);

    int ret = parser(sc_str, que, symtable);
    
    if (ret == 0)
        printf("SUCCESS\n");

    dynamicStr_free(sc_str);
    que_destroy(que);
    symtab_free(symtable);
    return 0;
}