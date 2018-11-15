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

int main()
{
    dynamicStr_t str;
    dynamicStr_t *sc_str = &str;
    
    dynamicStr_init(sc_str);
    queue_t *que = que_create();

    int ret = parser(sc_str, que);
    
    if (ret == 0)
        printf("SUCCESS\n");

    dynamicStr_free(sc_str);
    que_destroy(que);
    return 0;
}