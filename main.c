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
    for ( int i = 0; i < 20; i++)
        scanner(sc_str, que);

    dynamicStr_free(sc_str);

    que_destroy(que);
    return 0;
}