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
#include "ss_string.h"

#include "vm_defines.h"
#include "vm_opcode.h"
#include "vm_error_codes.h"


//typedef char (*ops) (dyn_c*, dyn_c[], ss_byte len);
//typedef char (*sys) (vm_env*, dyn_c*, dyn_c[], unsigned char)

typedef ss_char (*fct) (dyn_c*, dyn_c[], ss_byte);

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
    dyn_c local;                /**< List of dict, used to store the values for the
                                     evaluation of the loc-pointer.*/
    ss_byte new_scope;
    dyn_c* loc;

    dyn_c data;
    ss_ushort dp;

    ss_ushort sp;
    ss_char *pc;

    dyn_c functions;

    ss_char status;
    ss_short execution_steps;

    fct operator_list[25];

    dyn_c exceptions;
} __attribute__ ((packed));
typedef struct vm vm_env;

typedef ss_char (*sys) (vm_env*, dyn_c*, dyn_c [], ss_byte);


vm_env* vm_init         (ss_ushort memory_size,
                         ss_ushort functions_size,
                         ss_short execution_steps);
ss_char vm_execute      (vm_env* env, ss_char* code, ss_char trace);

void    vm_free         (vm_env* env);

void    vm_reset        (vm_env* env, ss_char hard);
void    vm_result       (vm_env* env, dyn_c* rslt);
ss_char vm_ready        (vm_env* env);

ss_int  vm_size         (vm_env* env);

dyn_c*  vm_get_rslt      (vm_env* env);

ss_char vm_add_variable (vm_env* env, ss_str key, dyn_c* value);
dyn_c*  vm_call_variable (vm_env* env, ss_str key);

ss_char vm_add_function (vm_env* env, ss_str key, void *ptr, const ss_str info, ss_char sys);
ss_char vm_call_function(vm_env* env, ss_str key,
                         dyn_c* rslt, dyn_c params [],
                         ss_byte len);

ss_char vm_get_iterator (dyn_c* iter_rslt, dyn_c* iter_dict, ss_int count);//(vm_env* env, ss_int count);

void    vm_printf       (ss_str str, ss_char newline);

/*
static struct capabilities {
    uint16_t debug :1;
    uint16_t list  :1;
    uint16_t set   :1;
} vm_cabilities;

vm_cabilities = {.debug=1, .list=1, .set=1};
*/

#endif
