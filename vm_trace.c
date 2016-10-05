#include "vm.h"
#include "vm_trace.h"

#ifdef S2_DEBUG
void vm_trace (vm_env* env, ss_char* code)
{
    ss_char log[140] = {'0','0','0','0',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
                        ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
                        ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
                        ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
                        ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',
                        ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};

    // print and command
    if( (ss_int)(env->pc - code)>=0 && (ss_int)(env->pc - code)<9999){
        if ((ss_int)(env->pc - code) >= 1000) {
            ss_itoa(&log[0], (ss_int)(env->pc - code));
        } else if ((ss_int)(env->pc - code) >= 100) {
            ss_itoa(&log[1], (ss_int)(env->pc - code));
        } else if ((ss_int)(env->pc - code) >= 10) {
            ss_itoa(&log[2], (ss_int)(env->pc - code));
        } else {
            ss_itoa(&log[3], (ss_int)(env->pc - code));
        }
    } else {
        ss_strcpy(&log[0], (ss_str)"XXXX");
    }
    ss_strcat(log, (ss_str)": ");
    ss_strcat(log, opcodes[(ss_int)*env->pc & POP_I]);
    if (*env->pc & POP)
        ss_strcat(log, (ss_str)"|P");

    log[ss_strlen(log)]=' ';

    switch ( *env->pc & POP_I ) {
        case CALL_OP:
        case CALL_OPX: ss_itoa(&log[16], *((ss_byte*) (env->pc+1)));
                       ss_strcat(log, (ss_str)", ");
                       ss_strcpy(&log[ss_strlen(log)], operators[(ss_int)*(env->pc+2)]);

                       break;
        case PROC    : ss_itoa(&log[16], *((ss_byte*) env->pc+1));
                       ss_strcat(log, (ss_str)", ");
                       ss_itoa(&log[ss_strlen(log)], *((ss_ushort*) (env->pc+2)));
                       break;
        case CST_S   :
        case TRY_1   :
        case JUMP    :
        case FJUMP   : ss_itoa(&log[16], *((ss_short*) (env->pc+1)));
                       break;
        case EXIT    :
        case IT_GROUP:
        case CALL_FCT:
        case CALL_FCTX: ss_itoa(&log[16], *((ss_byte*)(env->pc+1)));
        case IT_AS   :
        case CST_B   : ss_itoa(&log[16], *((ss_char*)(env->pc+1)));
                       break;
        case CST_DCT :
        case LOC     :
        case LOCX    :
        case CST_STR :
        case LOAD    :
        case STORE_LOC:
        case STORE   : ss_itoa(&log[16], *((ss_byte*) (env->pc+1)));
                       break;
#ifdef S2_SET
        case CST_SET :
#endif
        case CST_LST : ss_itoa(&log[16], *((ss_ushort*) (env->pc+1)));
                       break;
        case CST_I   : ss_itoa(&log[16], *((ss_int*) (env->pc+1)));
                       break;
        case CST_F   : ss_ftoa(&log[16], *((ss_float*) (env->pc+1)));
                       break;
        //case CALL_FCT: ss_itoa(&log[16], *(env->pc+1));
                       //ss_strcat(log, ", ");
                       //ss_itoa(&log[ss_strlen(log)], (ss_byte)*(env->pc+1));
                       //break;
    }

    log[ss_strlen(log)]=' ';

    ss_str str = dyn_get_string(&env->stack);

    if(ss_strlen(str)<56){
        ss_strcpy(&log[30], str);
    }
    else{
        ss_ushort i;
        ss_byte   x;
        for(i=ss_strlen(str), x=56; x; i--, x--)
            log[32+x] = str[i];
        log[32]='.';
        log[31]='.';
        log[30]='.';
    }

    log[ss_strlen(log)]=' ';

    free(str);


    str = dyn_get_string(&env->memory);
    if (ss_strlen(str) < 30) {
        ss_strcpy(&log[90], str);
    }
    else{
        ss_ushort i;
        ss_byte   x;
        for(i=ss_strlen(str), x=29; x; --i, --x)
            log[90+x] = str[i];
        log[91]='.';
        log[92]='.';
        log[93]='.';
    }
    free(str);

    vm_printf(log, 1);
}
#endif
