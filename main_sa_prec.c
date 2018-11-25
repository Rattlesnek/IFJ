#include <stdio.h>
#include <string.h>

#include "token.h"
#include "symtable.h"
#include "scanner.h"
#include "stack_tkn.h"
#include "dynamic_str.h"
#include "dynamic_arr_param.h"
#include "queue.h"
#include "parser.h"
#include "sa_prec.h"

#include "error.h"

int main()
{
    dynamicStr_t str;
    dynamicStr_t *sc_str = &str;

    dynamicStr_init(sc_str);
    queue_t *que = que_create();
    symtable_t *lsymtable = symtab_init(NULL, VARIABLES);
    symtable_t *fsymtable = symtab_init(NULL, FUNCTIONS);
    elem_t *elem = symtab_elem_add(fsymtable, "test");
    elem->func.is_defined = true;
    elem->func.n_params = 2;

    elem_t *elem1 = symtab_elem_add(lsymtable, "a");
    //elem1->var.key = "2";
    elem_t *elem2 = symtab_elem_add(fsymtable, "f");
    int ret_value = sa_prec(sc_str, que, lsymtable, fsymtable);
    if(ret_value)
        printf("=> sa_prec: Something is wrong, I guess... (%d)\n", ret_value);
    else
        printf("=> sa_prec: The expression is OK\n");

    elem_t *element = symtab_find(fsymtable, "test");
    if(element != NULL)
    {
        printf("=> sa_prec: Pocet parametru: %d\n", element->func.n_params);
    }

    dynamicStr_free(sc_str);
    que_destroy(que);
    symtab_free(lsymtable);
    symtab_free(fsymtable);

    return 0;
}