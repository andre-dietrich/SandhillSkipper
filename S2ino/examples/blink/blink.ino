#include "vm.h"

vm_env *env;
char program [] = {
  #include "program.h"
};

void setup() {
  // put your setup code here, to run once:
  env = vm_init(25, 10, 15);
  vm_add_function(env, (ss_str)"digitalWrite", (void*)S2_digitalWrite, (ss_str) "", 0);
}

void loop() {
  // put your main code here, to run repeatedly:
  vm_execute(env, program, 0);
}

char S2_digitalWrite(dyn_c* rslt, dyn_c params[], ss_byte len)  {
  dyn_free(rslt);

  if(len == 2) {
    digitalWrite(dyn_get_int (&params[0]), dyn_get_bool(&params[1]));
    return VM_OK;
  }

  return VM_ERROR;
}
