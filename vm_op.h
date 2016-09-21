#ifndef VM_OP_C_H
#define VM_OP_C_H

#include "vm.h"

ss_char op_NOT          (dyn_c *rslt, dyn_c op[], ss_byte len);
ss_char op_NEG          (dyn_c *rslt, dyn_c op[], ss_byte len);

ss_char op_ADD          (dyn_c *rslt, dyn_c op[], ss_byte len);

ss_char op_SUB          (dyn_c *rslt, dyn_c op[], ss_byte len);
ss_char op_MUL          (dyn_c *rslt, dyn_c op[], ss_byte len);
ss_char op_DIV          (dyn_c *rslt, dyn_c op[], ss_byte len);
ss_char op_MOD          (dyn_c *rslt, dyn_c op[], ss_byte len);
ss_char op_POW          (dyn_c *rslt, dyn_c op[], ss_byte len);

ss_char op_LT           (dyn_c *rslt, dyn_c op[], ss_byte len);
ss_char op_LE           (dyn_c *rslt, dyn_c op[], ss_byte len);
ss_char op_GT           (dyn_c *rslt, dyn_c op[], ss_byte len);
ss_char op_GE           (dyn_c *rslt, dyn_c op[], ss_byte len);
ss_char op_NE           (dyn_c *rslt, dyn_c op[], ss_byte len);
ss_char op_EQ           (dyn_c *rslt, dyn_c op[], ss_byte len);

ss_char op_AND          (dyn_c *rslt, dyn_c op[], ss_byte len);
ss_char op_OR           (dyn_c *rslt, dyn_c op[], ss_byte len);
ss_char op_XOR          (dyn_c *rslt, dyn_c op[], ss_byte len);

ss_char op_IN           (dyn_c *rslt, dyn_c op[], ss_byte len);

ss_char op_B_NOT        (dyn_c *rslt, dyn_c op[], ss_byte len);
ss_char op_B_AND        (dyn_c *rslt, dyn_c op[], ss_byte len);
ss_char op_B_OR         (dyn_c *rslt, dyn_c op[], ss_byte len);
ss_char op_B_XOR        (dyn_c *rslt, dyn_c op[], ss_byte len);

ss_char op_B_SHIFT_L    (dyn_c *rslt, dyn_c op[], ss_byte len);
ss_char op_B_SHIFT_R    (dyn_c *rslt, dyn_c op[], ss_byte len);

ss_char op_EX           (dyn_c *rslt, dyn_c op[], ss_byte len);

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
