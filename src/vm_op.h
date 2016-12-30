#ifndef VM_OP_C_H
#define VM_OP_C_H

#include "vm.h"

trilean vm_op_dispatch  (dyn_c *rslt, dyn_c op[], dyn_byte len, dyn_byte op_id);

trilean vm_sys_help     (vm_env* env, dyn_c* rslt, dyn_c params [], dyn_byte len);
trilean vm_sys_print    (vm_env* env, dyn_c* rslt, dyn_c params [], dyn_byte len);
trilean vm_sys_mem      (vm_env* env, dyn_c* rslt, dyn_c params [], dyn_byte len);
trilean vm_sys_del      (vm_env* env, dyn_c* rslt, dyn_c params [], dyn_byte len);

trilean ss_size_fct     (dyn_c* rslt, dyn_c params[1], dyn_byte len);
trilean ss_float_fct    (dyn_c* rslt, dyn_c params[1], dyn_byte len);
trilean ss_int_fct      (dyn_c* rslt, dyn_c params[1], dyn_byte len);
trilean ss_type_fct     (dyn_c* rslt, dyn_c params[1], dyn_byte len);
trilean ss_len_fct      (dyn_c* rslt, dyn_c params[1], dyn_byte len);
trilean ss_time_fct     (dyn_c* rslt, dyn_c params[1], dyn_byte len);
trilean ss_str_fct      (dyn_c* rslt, dyn_c params[1], dyn_byte len);
trilean ss_is_none_fct  (dyn_c* rslt, dyn_c params[1], dyn_byte len);
trilean ss_is_bool_fct  (dyn_c* rslt, dyn_c params[1], dyn_byte len);
trilean ss_is_int_fct   (dyn_c* rslt, dyn_c params[1], dyn_byte len);
trilean ss_is_float_fct (dyn_c* rslt, dyn_c params[1], dyn_byte len);
trilean ss_is_str_fct   (dyn_c* rslt, dyn_c params[1], dyn_byte len);
trilean ss_is_list_fct  (dyn_c* rslt, dyn_c params[1], dyn_byte len);
trilean ss_is_dict_fct  (dyn_c* rslt, dyn_c params[1], dyn_byte len);
trilean ss_is_proc_fct  (dyn_c* rslt, dyn_c params[1], dyn_byte len);
trilean ss_is_ex_fct    (dyn_c* rslt, dyn_c params[1], dyn_byte len);

trilean ss_insert       (dyn_c* rslt, dyn_c params[3], dyn_byte len);
trilean ss_remove       (dyn_c* rslt, dyn_c params[2], dyn_byte len);
trilean ss_pop          (dyn_c* rslt, dyn_c params[2], dyn_byte len);

dyn_uint hash            (dyn_char* val, dyn_ushort init);
trilean ss_hash         (dyn_c* rslt, dyn_c params[2], dyn_byte len);


#endif // VM_OP_C_H
