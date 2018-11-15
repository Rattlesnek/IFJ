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

	symtable_t *symtab = symtab_init(MAX, FUNCTIONS);

	symtab_elem_add(symtab, "test1");


	symtab_foreach(symtab, print_var);

	symtab_remove(symtab, "test1");

	symtab_foreach(symtab, print_var);
	symtab_elem_add(symtab, "test1");
	symtab_elem_add(symtab, "test1");
	symtab_elem_add(symtab, "test1");
	symtab_elem_add(symtab, "test2");
	symtab_elem_add(symtab, "test3");
	symtab_elem_add(symtab, "test4");
	symtab_elem_add(symtab, "test5");
	symtab_elem_add(symtab, "test7");
	symtab_elem_add(symtab, "test7");
	symtab_elem_add(symtab, "test9");
	

	elem_t *tmp = symtab_elem_add(symtab, "test8");
	tmp->func.n_params = 10;
	tmp->func.is_defined = true;
	tmp->func.params = "a,b,c,d,e,f,g,h,i,j";
	printf("%d\n", tmp->func.n_params);
	printf("%d\n", tmp->func.is_defined);
	printf("%s\n", tmp->func.params);


	symtab_foreach(symtab, print_var);
	//printf("%d\n", item->token_type);
	symtab_free(symtab);
	return 0;
}
