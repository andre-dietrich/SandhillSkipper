#include "vm.h"
#include "vm_trace.h"

#ifdef S2_DEBUG

static dyn_const_str opcodes[] = {
    "ENC_NONE",
    "ENC_TRUE",
    "ENC_FALSE",
    "ENC_INT1",
    "ENC_INT2",
    "ENC_INT4",
    "ENC_FLOAT",
    "ENC_LIST",
    "ENC_SET",
    "ENC_PROC",

    "ENC_STR",
    "ENC_DICT",

    "SP_SAVEX",
    "SP_SAVE",

    "RET",
    "RET_P",

    "LOC",
    "LOCX",
    "LOAD",
    "ELEM",

    "STORE",
    "STORE_RF",
    "STORE_LOC",

    "CALL_FCTX",
    "CALL_FCT",

    "FJUMP",
    "JUMP",

    "IT_INIT",
    "IT_NEXT0",
    "IT_NEXT1",
    "IT_NEXT2",
    "IT_NEXT3",
    "IT_STORE",
    "IT_LIMIT",

    "IT_GROUP",
    "IT_ORDER",
    "IT_AS",

    "EXIT",
    "REC_SET",

    "TRY_1",
    "TRY_0",

    "REF",

    "IT_INITX",
    "IT_STOREX",
    "CHK_FIRST",
    "RET_X",
    "IT_STOREX2",

    "LOC_STEP",
    "LOC_COUNT",

    "IT_CYCLE",
    "IT_UNIQUE",

    "YIELD",

    "PROC_LOAD",
};

static dyn_const_str operators[] = {
  "!",
  "--",
  "+",
  "-",
  "*",
  "/",
  "%",
  "**",

  "<",
  "<=",
  ">",
  ">=",
  "!=",
  "==",
  "===",
  "and",
  "xor",
  "or",
  "in",

  "~",
  "&",
  "|",
  "^",
  "<<",
  ">>",

  "EX" };


void vm_trace (vm_env* env, dyn_char* code)
{
    dyn_char log[140] = {'0','0','0','0',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
                        ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
                        ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
                        ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
                        ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
                        ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};

    // print and command
    if( (dyn_int)(env->pc - code)>=0 && (dyn_int)(env->pc - code)<9999){
        if ((dyn_int)(env->pc - code) >= 1000) {
            dyn_itoa(&log[0], (dyn_int)(env->pc - code));
        } else if ((dyn_int)(env->pc - code) >= 100) {
            dyn_itoa(&log[1], (dyn_int)(env->pc - code));
        } else if ((dyn_int)(env->pc - code) >= 10) {
            dyn_itoa(&log[2], (dyn_int)(env->pc - code));
        } else {
            dyn_itoa(&log[3], (dyn_int)(env->pc - code));
        }
    } else {
        dyn_strcpy(&log[0], (dyn_str)"XXXX");
    }
    dyn_strcat(log, (dyn_str)": ");

    if ((*env->pc & POP_I) > 63) {
        dyn_strcat(log, (dyn_byte)(*env->pc & OPERATION) >= 96
                       ? (dyn_str)"OPX|"
                       : (dyn_str)"OP|");

        dyn_strcpy(&log[dyn_strlen(log)], operators[*env->pc & OP_I]);
    }
    else
        dyn_strcat(log, opcodes[(dyn_int)*env->pc & POP_I]);

    if (*env->pc & POP)
        dyn_strcat(log, (dyn_str)"|P");

    log[dyn_strlen(log)]=' ';

    switch ( (*env->pc) & POP_I ) {
        case ENC_PROC:
            dyn_itoa(&log[16], *((dyn_byte*) env->pc+1));
            dyn_strcat(log, (dyn_str)", ");
            dyn_itoa(&log[dyn_strlen(log)], *((dyn_ushort*) (env->pc+2)));
            break;
        case ENC_INT2:
        case TRY_1:
        case JUMP:
        case FJUMP:
            dyn_itoa(&log[16], *((dyn_short*) (env->pc+1)));
            break;
        case EXIT:
        case IT_GROUP:
        case CALL_FCT:
        case CALL_FCTX:
            dyn_itoa(&log[16], *((dyn_byte*)(env->pc+1)));
        case IT_AS:
        case ENC_INT1:
            dyn_itoa(&log[16], *((dyn_char*)(env->pc+1)));
            break;
        case ENC_DICT:
        case LOC:
        case LOCX:
        case ENC_STRING:
        case LOAD:
        case STORE_LOC:
        case STORE:
        default:
            dyn_itoa(&log[16], *((dyn_byte*) (env->pc+1)));
            break;
#ifdef S2_SET
        case ENC_SET:
#endif
        case ENC_LIST:
            dyn_itoa(&log[16], *((dyn_ushort*) (env->pc+1)));
            break;
        case ENC_INT4:
            dyn_itoa(&log[16], *((dyn_int*) (env->pc+1)));
            break;
        case ENC_FLOAT:
            dyn_ftoa(&log[16], *((dyn_float*) (env->pc+1)));
            break;
        //case CALL_FCT: dyn_itoa(&log[16], *(env->pc+1));
                       //dyn_strcat(log, ", ");
                       //dyn_itoa(&log[dyn_strlen(log)], (dyn_byte)*(env->pc+1));
                       //break;
    }

    log[dyn_strlen(log)]=' ';

    dyn_str str = dyn_get_string(&env->stack);

    if(dyn_strlen(str)<56){
        dyn_strcpy(&log[30], str);
    }
    else{
        dyn_ushort i;
        dyn_byte   x;
        for(i=dyn_strlen(str), x=56; x; i--, x--)
            log[32+x] = str[i];
        log[32]='.';
        log[31]='.';
        log[30]='.';
    }

    log[dyn_strlen(log)]=' ';

    free(str);


    str = dyn_get_string(&env->memory);
    if (dyn_strlen(str) < 30) {
        dyn_strcpy(&log[90], str);
    }
    else{
        dyn_ushort i;
        dyn_byte   x;
        for(i=dyn_strlen(str), x=29; x; --i, --x)
            log[90+x] = str[i];
        log[91]='.';
        log[92]='.';
        log[93]='.';
    }
    free(str);

    vm_printf(log, 1);
}
#endif
