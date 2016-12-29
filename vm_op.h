#ifndef VM_OP_C_H
#define VM_OP_C_H

#include "vm.h"

ss_char vm_op_dispatch  (dyn_c *rslt, dyn_c op[], ss_byte len, ss_byte op_id);

ss_char vm_sys_help     (vm_env* env, dyn_c* rslt, dyn_c params [], ss_byte len);
ss_char vm_sys_print    (vm_env* env, dyn_c* rslt, dyn_c params [], ss_byte len);
ss_char vm_sys_mem      (vm_env* env, dyn_c* rslt, dyn_c params [], ss_byte len);
ss_char vm_sys_del      (vm_env* env, dyn_c* rslt, dyn_c params [], ss_byte len);

ss_char ss_size_fct     (dyn_c* rslt, dyn_c params[1], ss_byte len);
ss_char ss_float_fct    (dyn_c* rslt, dyn_c params[1], ss_byte len);
ss_char ss_int_fct      (dyn_c* rslt, dyn_c params[1], ss_byte len);
ss_char ss_type_fct     (dyn_c* rslt, dyn_c params[1], ss_byte len);
ss_char ss_len_fct      (dyn_c* rslt, dyn_c params[1], ss_byte len);
ss_char ss_time_fct     (dyn_c* rslt, dyn_c params[1], ss_byte len);
ss_char ss_str_fct      (dyn_c* rslt, dyn_c params[1], ss_byte len);
ss_char ss_is_none_fct  (dyn_c* rslt, dyn_c params[1], ss_byte len);
ss_char ss_is_bool_fct  (dyn_c* rslt, dyn_c params[1], ss_byte len);
ss_char ss_is_int_fct   (dyn_c* rslt, dyn_c params[1], ss_byte len);
ss_char ss_is_float_fct (dyn_c* rslt, dyn_c params[1], ss_byte len);
ss_char ss_is_str_fct   (dyn_c* rslt, dyn_c params[1], ss_byte len);
ss_char ss_is_list_fct  (dyn_c* rslt, dyn_c params[1], ss_byte len);
ss_char ss_is_dict_fct  (dyn_c* rslt, dyn_c params[1], ss_byte len);
ss_char ss_is_proc_fct  (dyn_c* rslt, dyn_c params[1], ss_byte len);
ss_char ss_is_ex_fct    (dyn_c* rslt, dyn_c params[1], ss_byte len);

ss_char ss_insert       (dyn_c* rslt, dyn_c params[3], ss_byte len);
ss_char ss_remove       (dyn_c* rslt, dyn_c params[2], ss_byte len);

#endif // VM_OP_C_H
