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

char *program = NULL;


#include <Base64.h>
void S2runner (char* str, size_t length) {
  if (program) {
    free(program);
    program = NULL;
  }

  program = malloc(Base64.decodedLength(str, length));

  Base64.decode(program, str, length);
}

void InitGUI(){
  frm.print(F("!h<h1>SelectScript & ArduinoView</h1><p></p>"));
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


  frm.print(F("!H<input type='text' id='server' class='sql' size='80' maxlength='100' value='ws://localhost:8080'>"));
  frm.print(F("<button onclick='send()'>Send</button>"));
  frm.print(F("<p>Query: <p><textarea id='code' rows='15' cols='92' name='code'></textarea>"));
  frm.print(F("<p>Result:<p><textarea id='output' rows='5' cols='92'></textarea>"));
  frm.end();

  frm.print(F("!jrunner['RS']=function(x){document.getElementById('output').value=x}"));
  frm.end();
}


vm_env *env;

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(OUTPUT__BAUD_RATE);

  //request reset of gui
  frm.print("!!");
  frm.end();

  delay(500);

  env = vm_init(25, 10, 15);
}

// the loop routine runs over and over again forever:
void loop() {

  if(program){
    vm_execute(env, program, 0);

    if(env->status != VM_IDLE) {
      frm.print("RS");

      if(env->status == VM_OK) {
        char *rslt = dyn_get_string(vm_get_rslt(env));
        frm.print(rslt);
        free(rslt);
      }
      else {
        vm_reset(env, 0);
        frm.print("VM_ERROR");
      }
      frm.end();

      free(program);
      program = NULL;
    }
  }

  while(frm.run());
}
