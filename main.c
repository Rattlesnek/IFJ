#include <stdio.h>

#include "token.h"
#include "symtable.h"
#include "scanner.h"
#include "scanner.c"

int main()
{
    // 
    token_info_t info = { .dbl = 42.42 };

    token_t *token = createToken("Adam", info);
    if (token == NULL)
    {
        // handle error
    }

    // name
    printf("name:\t%s\n", token->name);
    // test
    printf("int:\t%d\n", token->info.intg);
    printf("double:\t%lf\n", token->info.dbl);
    printf("ptr:\t%ld\n", (size_t) token->info.string);

    destroyToken(token);
    while(1)
        scanner();
    return 0;
}