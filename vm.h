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


/**
 * @brief Virtual Machine environment.
 *
 * It is the general container for storing all VM related data and status
 * information.
 */
struct vm {
    dyn_c memory;               /**< Dict, that is used to store variables. */
    dyn_c stack;                /**< List, that is used for the stack implementation. */
    dyn_c rslt;                 /**< Result container, that is used to store the
                                     result of the computation, if and only ift he
                                     process is finised. */
    dyn_c* loc;

    dyn_str data;               /**< Repository for storing strings and string
                                     identifiers */
    dyn_ushort sp;              /**< Stack-Pointer */
    dyn_char *pc;               /**< Program-Counter */

    dyn_c functions;            /**< Dict, used as repository for C-functions */

    dyn_char   status;
    dyn_ushort execution_steps; /**< Maximum number of subsequent steps, a zero
                                     value is used to indicate a run until the
                                     result is generated */
    dyn_ushort memory_size;     /**< Maximum size of global memory usage, used
                                     for global variables */
    dyn_ushort stack_size;      /**< Maximum memory allocated by the stack */
} __attribute__ ((packed));
typedef struct vm vm_env;

//! Function-type definition for applied operators
typedef trilean (*fct) (dyn_c*, dyn_c[], dyn_byte);

//! Function-type definition for external C-Functions
typedef trilean (*sys) (vm_env*, dyn_c*, dyn_c [], dyn_byte);

//! Basic initialization function of the VM
vm_env*  vm_init        (dyn_ushort memory_size,
                         dyn_ushort stack_size,
                         dyn_ushort execution_steps);
//! Initialize a second VM with shared memory and functions to another main VM
vm_env*  vm_init2       (vm_env* env_main,
                         dyn_ushort stack_size,
                         dyn_ushort execution_steps);
//! Execute a byte-code, with or without stack-trace printout
dyn_char vm_execute     (vm_env* env, dyn_char* code, dyn_char trace);
//! Delete all allocated VM memory
void     vm_free        (vm_env* env);
//! Reset the entire VM (hard), or only the stack and the result by keeping the global variables
void     vm_reset       (vm_env* env, dyn_char hard);
//! Return the generated program result
void     vm_result      (vm_env* env, dyn_c* rslt);
//! Check if the VM execution is in mode idle/finished/error
dyn_char vm_ready       (vm_env* env);
//! Calculate the allocated memory by the VM
dyn_int  vm_size        (vm_env* env);

dyn_c*  vm_get_rslt     (vm_env* env);

//! Add a global variable from C
trilean vm_add_variable(vm_env* env, dyn_const_str key, dyn_c* value);
//! Get the value of a global variable
dyn_c*  vm_call_variable(vm_env* env, dyn_const_str key);

//! Add a C-function to the VM environment
trilean vm_add_function (vm_env* env, dyn_char type, dyn_const_str key, void *ptr, dyn_const_str info);
//! Depricated
//trilean vm_call_function(vm_env* env, dyn_const_str key,
//                         dyn_c* rslt, dyn_c params [],
//                         dyn_byte len);

//! Used by SELECT - statements to iterate over the Cartesian product of a given input
trilean vm_get_iterator (dyn_c* iter_rslt, dyn_c* iter_dict, dyn_int count);

//! System-function for printing out to the console
void    vm_printf        (dyn_const_str str, dyn_char newline);

/*
static struct capabilities {
    uint16_t debug :1;
    uint16_t list  :1;
    uint16_t set   :1;
} vm_cabilities;

vm_cabilities = {.debug=1, .list=1, .set=1};
*/

#endif
