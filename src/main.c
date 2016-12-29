//#include <stdio.h>
//#include <string.h>
//#include <stdlib.h>

#define S(X)

#include "vm.h"
#include "dynamic.h"
#include "ss_string.h"

int main(int argc, char **argv) {

    vm_env *env = vm_init(25, 10, -1);

    char program [] = {
        #include "test.c"
    };
    vm_execute(env, program, 1);
    vm_printf("--------------------",1);
    char *rslt = dyn_get_string(vm_get_rslt(env));
    vm_printf(rslt,1);
    free(rslt);

    vm_free(env);

/*
    dyn_c list;
    DYN_INIT(&list);

    DYN_SET_LIST(&list);
    DYN_SET_LIST(&list);
    DYN_SET_LIST(&list);

    dyn_free(&list);
*/
    return 0;
}
