//#include <stdio.h>
//#include <string.h>
//#include <stdlib.h>

#define S(X)

#include "vm.h"
#include "dynamic.h"
#include "ss_string.h"

int main(int argc, char **argv) {

    vm_env *env = vm_init(25, 10, -1);

    char program [] =
     //{1, 0, 97, 0, 3, 7, 1, 7, 2, 7, 3, 7, 4, 7, 5, 12, 5, 0, -111, 0, 15, 0, 21, 0, 24, -107, 0, 12, 15, 0, 21, 0, 24, 15, 0, -107, 1, 12, 15, 0, 21, 0, 24, 15, 0, 15, 0, 21, 0, 24, 21, 2, 12, 0, };
     {2, 0, 97, 0, 115, 116, 114, 0, 3, 7, 1, 7, 2, 7, 3, 7, 4, 7, 5, 12, 5, 0, -111, 0, 15, 0, 21, 0, 24, -107, 0, 2, 15, 0, 21, 0, 24, 7, 1, -107, 1, 2, 7, 1, 15, 0, 21, 0, 24, 7, 1, -107, 2, 2, 15, 0, 7, 1, -107, 1, 2, 7, 1, 15, 0, 7, 1, -107, 2, 2, 7, 1, 15, 0, 11, 1, -107, 2, 2, 15, 0, 21, 0, 24, 7, 1, 11, 1, -107, 2, 2, 15, 0, 7, 6, -108, 1, 2, 15, 0, 11, 1, 20, 1, 2, 0, };
    vm_execute(env, program, 1);
    vm_printf("--------------------",1);

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
