#include "symtable.h"
#include "token.h"

/*11113 is a prime number. This size is a good balance between 
memory needed and performance so going through the table should be fairly efficient.*/
#define MAX 11113
/*
 *typedef union token_info {
    elem_t *ptr;    // either ptr to info-element in hash table (use in all cases except latter two)
    int intg;       // or integer (use only when token name is "INT")
    double dbl;     // or double (use only when token name is "DBL")
    char *string;   // or string (use only when token name is "STR")
} token_info_t;

typedef struct token {
    char *name;         // token name eg. "ID", "STR", "+", "==", "EOL", ...
    token_info_t info;  // token info      
} token_t;
*/

void print_fun(elem_t *element)
{
    printf("FUN %s\n", element->func.key);
}


void print_var(elem_t *element)
{
    printf("VAR %s\n", element->var.key);
}


int main()
{

	elem_t *item = NULL;
	symtable_t *symtab = symtab_init(MAX, VARIABLES);

	symtab_elem_add(symtab, "test1");


	symtab_foreach(symtab, print_var);

	symtab_remove(symtab, "test1");

	symtab_foreach(symtab, print_var);
	//printf("%d\n", item->token_type);
	symtab_free(symtab);
	return 0;
}
