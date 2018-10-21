#include "symtable.h"

/*11113 is a prime number. This size is a good balance between 
memory needed and performance so going through the table should be fairly efficient.*/
#define MAX 11113


int main()
{

	elem_t *item = NULL;
	symtable_t *symtab = symtab_init(MAX);

	token_t *token = malloc(sizeof(token_t));
	token->name = "FUNC";
	token->info.string = "ahojky";
	item = symtab_token_add(symtab, token);
	item->value_type = string;

	printf("%s\n", token->info.string);
	printf("%d\n", item->token_type);
	symtab_remove(symtab, "ahojky");
	//printf("%d\n", item->token_type);
	free(token);
	symtab_free(symtab);
	return 0;
}
