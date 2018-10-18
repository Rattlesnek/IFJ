#include <stdio.h>

#include "token.h"

int main()
{
    // 
    token_info_t info = { .dbl = 42.42 };

    token_t *token = tk_CreateToken("Adam", info);
    if (token == NULL)
    {
        // handle error
    }

    // name
    printf("name:\t%s\n", token->name);
    // test
    printf("int:\t%d\n", token->info.intg);
    printf("double:\t%lf\n", token->info.dbl);
    printf("ptr:\t%ld\n", (size_t) token->info.ptr);

    tk_DestroyToken(token);
    
    return 0;
}