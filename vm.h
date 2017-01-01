/**
 * @file vm.h
 * @author André Dietrich
 * @date 9 April 2016
 * @brief Implementation of the Virtual Machine for SelectScript.
 *
 * todo.
 * @see http://todo
 */


#ifndef VM_C_H
#define VM_C_H

#include "dynamic.h"

#include "vm_defines.h"
#include "vm_opcode.h"
#include "vm_error_codes.h"


//typedef char (*ops) (dyn_c*, dyn_c[], dyn_byte len);
//typedef char (*sys) (vm_env*, dyn_c*, dyn_c[], unsigned char)

typedef trilean (*fct) (dyn_c*, dyn_c[], dyn_byte);

/**
 * @brief Virtual Machine environment.
 *
 * todo.
 */
struct vm {
    dyn_c memory;               /**< Dict, that is used to store variables. */
    dyn_c stack;                /**< List, that is used for the stack implementation. */
    dyn_c rslt;                 /**< Result container, that is used to store the
                                     result of the computation, if and only ift he
                                     process is finised. */
    dyn_c* loc;

    char* data;
    //dyn_ushort dp;

    dyn_ushort sp;
    dyn_char *pc;

    dyn_c functions;

    dyn_char   status;
    dyn_ushort execution_steps;
    dyn_ushort memory_size;
    dyn_ushort stack_size;
} __attribute__ ((packed));
typedef struct vm vm_env;

typedef dyn_char (*sys) (vm_env*, dyn_c*, dyn_c [], dyn_byte);


vm_env*  vm_init        (dyn_ushort memory_size,
                         dyn_ushort stack_size,
                         dyn_ushort execution_steps);

vm_env*  vm_init2       (vm_env* env_main,
                         dyn_ushort stack_size,
                         dyn_ushort execution_steps);

dyn_char vm_execute     (vm_env* env, dyn_char* code, dyn_char trace);

void     vm_free        (vm_env* env);

void     vm_reset       (vm_env* env, dyn_char hard);
void     vm_result      (vm_env* env, dyn_c* rslt);
dyn_char vm_ready       (vm_env* env);

dyn_int  vm_size        (vm_env* env);

dyn_c*  vm_get_rslt     (vm_env* env);

dyn_char vm_add_variable(vm_env* env, dyn_str key, dyn_c* value);
dyn_c*  vm_call_variable(vm_env* env, dyn_str key);

dyn_char vm_add_function (vm_env* env, dyn_str key, void *ptr, const dyn_str info, dyn_char sys);
dyn_char vm_call_function(vm_env* env, dyn_str key,
                         dyn_c* rslt, dyn_c params [],
                         dyn_byte len);

dyn_char vm_get_iterator (dyn_c* iter_rslt, dyn_c* iter_dict, dyn_int count);//(vm_env* env, dyn_int count);

void    vm_printf        (dyn_str str, dyn_char newline);

/*
static struct capabilities {
    uint16_t debug :1;
    uint16_t list  :1;
    uint16_t set   :1;
} vm_cabilities;

vm_cabilities = {.debug=1, .list=1, .set=1};
*/

#endif
