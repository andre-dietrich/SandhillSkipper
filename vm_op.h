#ifndef VM_OP_C_H
#define VM_OP_C_H

#include "vm.h"

trilean vm_op_dispatch  (dyn_c *rslt, dyn_c op[], dyn_byte len, dyn_byte op_id);

trilean vm_sys_help     (vm_env* env, dyn_c* rslt, dyn_c params [], dyn_byte len);
trilean vm_sys_print    (vm_env* env, dyn_c* rslt, dyn_c params [], dyn_byte len);
trilean vm_sys_mem      (vm_env* env, dyn_c* rslt, dyn_c params [], dyn_byte len);
trilean vm_sys_del      (vm_env* env, dyn_c* rslt, dyn_c params [], dyn_byte len);

trilean fct_size     (dyn_c* rslt, dyn_c params[1], dyn_byte len);
trilean fct_float    (dyn_c* rslt, dyn_c params[1], dyn_byte len);
trilean fct_int      (dyn_c* rslt, dyn_c params[1], dyn_byte len);
trilean fct_type     (dyn_c* rslt, dyn_c params[1], dyn_byte len);
trilean fct_len      (dyn_c* rslt, dyn_c params[1], dyn_byte len);
trilean fct_time     (dyn_c* rslt, dyn_c params[1], dyn_byte len);
trilean fct_str      (dyn_c* rslt, dyn_c params[1], dyn_byte len);
trilean fct_is_none  (dyn_c* rslt, dyn_c params[1], dyn_byte len);
trilean fct_is_bool  (dyn_c* rslt, dyn_c params[1], dyn_byte len);
trilean fct_is_int   (dyn_c* rslt, dyn_c params[1], dyn_byte len);
trilean fct_is_float (dyn_c* rslt, dyn_c params[1], dyn_byte len);
trilean fct_is_str   (dyn_c* rslt, dyn_c params[1], dyn_byte len);
trilean fct_is_list  (dyn_c* rslt, dyn_c params[1], dyn_byte len);
trilean fct_is_dict  (dyn_c* rslt, dyn_c params[1], dyn_byte len);
trilean fct_is_proc  (dyn_c* rslt, dyn_c params[1], dyn_byte len);
trilean fct_is_ex    (dyn_c* rslt, dyn_c params[1], dyn_byte len);

trilean fct_insert       (dyn_c* rslt, dyn_c params[3], dyn_byte len);
trilean fct_remove       (dyn_c* rslt, dyn_c params[2], dyn_byte len);
trilean fct_pop          (dyn_c* rslt, dyn_c params[2], dyn_byte len);

dyn_uint hash            (dyn_char* val, dyn_ushort init);
trilean fct_hash         (dyn_c* rslt, dyn_c params[2], dyn_byte len);


#endif // VM_OP_C_H
