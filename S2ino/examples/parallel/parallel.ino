#include "dynamic.h"
#include "vm.h"

#define FRAMEREADERSIZE 1500
#include <FrameStream.h>
#include <Frameiterator.h>

#define OUTPUT__BAUD_RATE 57600
FrameStream frm(Serial);

// runner list just for the Initialisation
beginrunnerlist();
  fwdrunner(S2, S2runner);
  callrunner(!!,InitGUI);
endrunnerlist();



vm_env *env_main;
vm_env *env_user;

char program_main [] = {
  #include "program.h"
};

char *program_user = NULL;


#include <Base64.h>
void S2runner (char* str, size_t length) {
  if (program_user) {
    free(program_user);
    program_user = NULL;
  }
  program_user = malloc(Base64.decodedLength(str, length));
  Base64.decode(program_user, str, length);
}

void InitGUI(){
  frm.print(F("!h<h1>SelectScript & ArduinoView</h1><p></p>"));
  frm.end();
  frm.print("!SGgrv50");
  frm.end();

  frm.print(F("!jws = null;\n"
              "send = function() {\n"
              "if (ws == null) {\n"
              "ws = new WebSocket(document.getElementById('server').value);\n"
              "ws.onmessage = function (event) {\n"
              "sendframe('S2'+event.data);\n"
              "ws = null;}\n"
              "ws.onopen = function (event) {\n"
              "ws.send(document.getElementById('code').value);}}}"));

  frm.end();

  frm.print(F("!H<input type='text' id='server' class='sql' size='80' maxlength='100' value='ws://localhost:8080'>"
              "<button onclick='send()'>Send</button>"
              "<p>Query: <p><textarea id='code' rows='8' cols='92' name='code'></textarea>"
              "<p>Result:<p><textarea id='output' rows='3' cols='92'></textarea>"));
  frm.end();

  frm.print(F("!jrunner['RS']=function(x){document.getElementById('output').value=x}"));
  frm.end();
}




// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(OUTPUT__BAUD_RATE);

  //request reset of gui
  frm.print("!!");
  frm.end();

  delay(500);

  env_main = vm_init(25, 10, 15);

  vm_add_function(env_main, (ss_str)"analogRead", (void*)S2_analogRead, (ss_str) "", 0);
  vm_add_function(env_main, (ss_str)"draw", (void*)S2_draw, (ss_str) "", 0);

  env_user = vm_init2(env_main, 100, 10);
}

// the loop routine runs over and over again forever:
void loop() {

  if(program_user){
    vm_execute(env_user, program_user, 0);

    if(env_user->status != VM_IDLE) {
      frm.print("RS");

      if(env_user->status == VM_OK) {
        char *rslt = dyn_get_string(vm_get_rslt(env_user));
        frm.print(rslt);
        free(rslt);
      }
      else {
        vm_reset(env_user, 0);
        frm.print("VM_ERROR");
      }
      frm.end();

      free(program_user);
      program_user = NULL;
    }
  }

  while(frm.run());

  vm_execute(env_main, program_main, 0);
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

char S2_draw(dyn_c* rslt, dyn_c params[], ss_byte len)  {

  if (len) {
    for(int i=0; i<len; ++i) {
      char* text = dyn_get_string(&params[i]);
      frm.print(text);
      free(text);
    }
    frm.end();
    return VM_OK;
  }

  return VM_ERROR;
}
