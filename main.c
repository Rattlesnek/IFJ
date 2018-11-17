#include <stdio.h>
#include <string.h>

#include "token.h"
#include "scanner.h"
#include "dynamicStr.h"
#include "queue.h"

int main()
{
    dynamicStr_t str;
    dynamicStr_t *sc_str = &str;
    
    dynamicStr_init(sc_str);
    queue_t *que = que_create();


    token_t *token = NULL;

    do {
        if (token != NULL)
            destroyToken(token);
        
        token = scanner_get(sc_str, que);
        
    } while (strcmp(token->name, "EOF") != 0);

    destroyToken(token);

    dynamicStr_free(sc_str);
    que_destroy(que);

    return 0;
}