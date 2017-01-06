#include "vm.h"

vm_env *env;
char program [] = {
  #include "program.h"
};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  env = vm_init(25, 10, 15);
  vm_add_function(env, (ss_str)"analogRead", (void*)S2_analogRead, (ss_str) "", 0);
}

void loop() {
  // put your main code here, to run repeatedly:
  vm_execute(env, program, 0);
}

char S2_analogRead(dyn_c* rslt, dyn_c params[], ss_byte len)  {
  dyn_free(rslt); // free rslt and set it to None

  switch (len) {
#if defined (_VARIANT_ARDUINO_DUE_X_)
    case 2: if (DYN_TYPE(&params[1]) <= FLOAT && DYN_TYPE(&params[1]))
              analogReadResolution(dyn_get_int(&params[1]));
            else return VM_ERROR;
#endif
    case 1: if (DYN_TYPE(&params[0]) <= FLOAT && DYN_TYPE(&params[0]))
              dyn_set_int(rslt, analogRead(dyn_get_int(params)));
            else return VM_ERROR;
  }

  return VM_OK;
}
