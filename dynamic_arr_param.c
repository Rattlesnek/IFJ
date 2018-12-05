/**CFile****************************************************************

  FileName    [dynamic_arr_params.c]

  SystemName  [IFJ - PROJECT]

  PackageName [Data type]

  Synopsis    [Dynamic paramsay of integers implementation]

  Author      [Adam Pankuch, Lukas Valek]

  Login       [xpanku00, xvalek15]

  Affiliation []

  Date        [26/10/2018]

  Revision    []

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                          INCLUDES                                ///
////////////////////////////////////////////////////////////////////////

#include "dynamic_arr_param.h"

////////////////////////////////////////////////////////////////////////
///                       GLOBAL VARIABLES                           ///
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
////////////////////////////////////////////////////////////////////////


/**
 * @brief      Initialize the array of pointers
 *
 * @return     True     if succesfull, otherwise false
 */
dynamicArrParam_t *dynamicArrParams_init()
{
    dynamicArrParam_t *a = malloc(sizeof(dynamicArrParam_t));
    if (a == NULL)
    {
        return NULL;
    }

    a->param_arr = malloc(DYNAMICARRPARAM_LEN * sizeof(elem_t));
    if (a->param_arr == NULL)
        return false;

    a->length = 0;
    a->max = DYNAMICARRPARAM_LEN;

    return a;
}


/**
 * @brief      Free array of params
 *
 * @param      a     structure to be freed
 */
void dynamicArrParams_free(dynamicArrParam_t *a)
{
    if (a == NULL)
        return;

    free(a->param_arr);
    a->param_arr = NULL;
    a->length = 0;
    a->max = 0;
    free(a);
}


/**
 * @brief      Add a new param ptr to the array
 *
 * @param      a      ptr to the structure of param pointers
 * @param      param  element ptr to be added
 *
 * @return     true if was succesful, otherwise false
 */
bool dynamicArrParams_add(dynamicArrParam_t *a, elem_t *param)
{
    if (a->length >= a->max)
    {
        unsigned int new_size = a->length + MULT_LEN;
        a->param_arr = realloc(a->param_arr, new_size * sizeof(elem_t));
        if (a->param_arr == NULL)
            return false;

        a->max = new_size;
    }

    a->param_arr[a->length++] = param;

    return true;
}


/*
int main()
{

    dynamicArrParam_t *a = dynamicArrParams_init();
    if (a == NULL)
    {
        return 1;
    }
    symtable_t *symtab_v = symtab_init(10, VARIABLES);
    symtable_t *symtab_f = symtab_init(5, FUNCTIONS);
    elem_t *fce = symtab_elem_add(symtab_f, "funkce");
    fce->func.params = a;


    elem_t *elem;
    char pole[6][6] = {"jedna", "dva", "tri", "ctyri", "pet", "sest"};
    for (int i = 0; i < 6; ++i)
    {
        elem = symtab_elem_add(symtab_v, pole[i]);
        dynamicArrParams_add(fce->func.params, elem);
        printf("key: %s, arr lenth: %d, max: %d\n",
         fce->func.params->param_arr[i]->var.key,
         fce->func.params->length, fce->func.params->max);
    }

    
    symtab_free(symtab_v);
    symtab_free(symtab_f);

    return 0;
}
*/


////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////
