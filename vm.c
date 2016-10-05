#include <stdio.h>
#include <stdlib.h>

#include "vm.h"
#include "vm_op.h"
#include "vm_opcode.h"
#include "vm_trace.h"


#define VM_STACK_LEN        stack->length
#define VM_STACK_END        &stack->container[VM_STACK_LEN -1]
#define VM_STACK_REF(I)     &stack->container[I]
#define VM_STACK_REF_END(I) &stack->container[VM_STACK_LEN -I]

#define VM_LOCAL    VM_STACK_REF(env->sp-1)
#define IT_DATA     VM_STACK_REF(env->sp+1)
#define IT_LIST     VM_STACK_REF(env->sp+2)
#define IT_COUNT1   VM_STACK_REF(env->sp+3)
#define IT_COUNT2   VM_STACK_REF(env->sp+4)

#ifdef ARDUINO
#include <Arduino.h>
#define putc( C, weg) Serial.write(C)
//#define FreeStatic
//#include "ArduinoFreeRam.h"
#endif

dyn_c* find_local(dyn_list* stack, ss_ushort* start, ss_str id)
{
    ss_ushort pos = *start;
    ss_ushort idx = 0;
    dyn_c* elem   = NULL;

    while (pos) {
        elem = VM_STACK_REF(pos-1);
        if (DYN_NOT_NONE(elem)) {
            if( !id[0] ) {
                *start = pos;
                return DYN_DICT_GET_I_REF(elem, 0);
            } else {
                idx = dyn_dict_has_key(elem, id);
                if(idx--) {
                    *start = pos;
                    return DYN_DICT_GET_I_REF(elem, idx);
                }
            }
        }
        pos = dyn_get_int(VM_STACK_REF(pos));
    }

    return NULL;
}

vm_env* vm_init (ss_ushort memory_size,
                 ss_ushort stack_size,
                 ss_short  execution_steps)
{
    vm_env* env = (vm_env*) malloc(sizeof(vm_env));

    env->pc = NULL;

    DYN_INIT(&env->stack);
    DYN_SET_LIST(&env->stack);

    DYN_INIT(&env->rslt);
    dyn_set_none(&env->rslt);

    DYN_INIT(&env->memory);
    dyn_set_dict(&env->memory, 1);

    DYN_INIT(&env->functions);
    dyn_set_dict(&env->functions, 21);

    vm_add_function(env, (ss_str)"print", (void*)vm_sys_print,    (ss_str) "", 1); //"print: prints out the passed parameters, the last defines the return value ...");
//     vm_add_function(env, (ss_str)"help",  (void*)vm_sys_help,     (ss_str) "", 1); //"general help function ...");
    vm_add_function(env, (ss_str)"mem",   (void*)vm_sys_mem,      (ss_str) "", 1); //"show mem ...");
    vm_add_function(env, (ss_str)"del",   (void*)vm_sys_del,      (ss_str) "", 1); //"delete from memory ...");
    /*------------------------------------------------------------------------*/
    vm_add_function(env, (ss_str)"size",  (void*)ss_size_fct,     (ss_str) "", 0); //"size");
    vm_add_function(env, (ss_str)"float", (void*)ss_float_fct,    (ss_str) "", 0); //"to float");
    vm_add_function(env, (ss_str)"str",   (void*)ss_str_fct,      (ss_str) "", 0); //"to string");
    vm_add_function(env, (ss_str)"int",   (void*)ss_int_fct,      (ss_str) "", 0); //"to int");
    vm_add_function(env, (ss_str)"type",  (void*)ss_type_fct,     (ss_str) "", 0); //"type");
    vm_add_function(env, (ss_str)"len",   (void*)ss_len_fct,      (ss_str) "", 0); //"length");
    vm_add_function(env, (ss_str)"time",  (void*)ss_time_fct,     (ss_str) "", 0); //"time");

    vm_add_function(env, (ss_str)"none?", (void*)ss_is_none_fct,  (ss_str) "", 0);
    vm_add_function(env, (ss_str)"bool?", (void*)ss_is_bool_fct,  (ss_str) "", 0);
    vm_add_function(env, (ss_str)"int?",  (void*)ss_is_int_fct,   (ss_str) "", 0);
    vm_add_function(env, (ss_str)"float?",(void*)ss_is_float_fct, (ss_str) "", 0);
    vm_add_function(env, (ss_str)"str?",  (void*)ss_is_str_fct,   (ss_str) "", 0);
    vm_add_function(env, (ss_str)"list?", (void*)ss_is_list_fct,  (ss_str) "", 0);
    vm_add_function(env, (ss_str)"dict?", (void*)ss_is_dict_fct,  (ss_str) "", 0);
    vm_add_function(env, (ss_str)"proc?", (void*)ss_is_proc_fct,  (ss_str) "", 0);
    vm_add_function(env, (ss_str)"ex?",   (void*)ss_is_ex_fct,    (ss_str) "", 0);

    vm_add_function(env, (ss_str)"insert",(void*)ss_insert,       (ss_str) "", 0);
    vm_add_function(env, (ss_str)"remove",(void*)ss_remove,       (ss_str) "", 0);

    env->new_scope = 1;
    env->loc = NULL;

    env->dp = 0;

    DYN_INIT(&env->data);
    DYN_SET_LIST(&env->data);

    env->status = VM_OK;
    env->execution_steps = execution_steps;

    env->memory_size = memory_size;
    env->stack_size  = stack_size;
    return env;
}

ss_char vm_execute (vm_env* env, ss_char* code, ss_char trace) {

    dyn_c tmp, tmp2;
    DYN_INIT(&tmp);
    DYN_INIT(&tmp2);

    dyn_c none;
    DYN_INIT(&none);

    dyn_list*  stack = env->stack.data.list;
    dyn_c* env_stack = &env->stack;

    ss_ushort us_len  = 0;
    ss_ushort us_i    = 0;
    ss_byte   uc_len  = 0;
    ss_byte   uc_i    = 0;
    ss_str    cp_str  = NULL;

    dyn_c*    dyc_ptr  = NULL;
    dyn_c*    dyc_ptr2 = NULL;

    ss_uint   i_i     = 0;
    ss_uint   i_j     = 0;

    ss_byte   pop     = 0;

    if(env->status == VM_OK){
        env->pc = code;
        env->sp = 0;
        env->dp = (ss_ushort)*env->pc;
        env->status = VM_IDLE;

        // init data
        env->pc+=2;
        for (us_i=0; us_i<env->dp; ++us_i) {
            dyn_set_string(&tmp, env->pc);
            dyn_list_push(&env->data, &tmp);
            env->pc += ss_strlen(env->pc) + 1;
        }
        dyn_free(&tmp);
        /*
        if (trace) {
            ss_str data_string = dyn_get_string(&env->data);
            vm_printf(data_string, 1);
            free(data_string);
        }*/
        env->dp = 0;
    }

    ss_short execution_steps = env->execution_steps;

/*---------------------------------------------------------------------------*/
//DISPACH:
/*---------------------------------------------------------------------------*/
do{
    dyn_free(&tmp);
    dyn_free(&tmp2);

    if (env->status != VM_IDLE) {
        us_i = env->sp;
        dyc_ptr = find_local(stack, &us_i, "\a");
        if (dyc_ptr) {
            env->status = VM_IDLE;
            env->sp = us_i;
            dyn_list_popi(env_stack, VM_STACK_LEN - us_i -1);
            env->pc = (char*) dyn_get_extern(dyc_ptr);
            dyn_dict_remove(VM_LOCAL, "\a");
        }
        else
            return env->status;
    }

    if (pop) {
        if (*env->pc != EXIT) {
            dyn_list_popi(env_stack, 1);
        }

        env->loc = NULL;

        pop = 0;
    }

    if (execution_steps > -1) {
        if (!execution_steps) {
            dyn_free(&tmp);
            dyn_free(&tmp2);
            return 0;
        }
        execution_steps--;
    }
#ifdef S2_DEBUG
    if (trace)
        vm_trace(env, code);
#endif
    if (POP & *env->pc)
        pop = 1;

switch((ss_byte)(POP_I & *env->pc++)){
/*---------------------------------------------------------------------------*/
case RET:
case RET_P:
/*---------------------------------------------------------------------------*/

    dyn_list_pop(env_stack, &tmp); // return value

    us_len = dyn_get_int(VM_STACK_REF(env->sp));

    dyn_list_popi(env_stack, VM_STACK_LEN - env->sp + 1);

    env->sp = us_len;

    if (*(env->pc-1) == RET_P) {
        dyn_list_pop(env_stack, &tmp2);
        env->pc = (ss_char*)dyn_get_extern(&tmp2);
        dyn_list_pop(env_stack, &tmp2);
        env->dp = dyn_get_int(&tmp2);
        dyn_list_pop(env_stack, &tmp2);
        pop = dyn_get_bool(&tmp2);
        dyn_list_pop(env_stack, &tmp2);
        if (DYN_NOT_NONE(&tmp2)) {
            dyn_move(&tmp, (dyn_c*)dyn_get_extern(&tmp2));
            dyn_list_push(env_stack, &none);
            dyn_set_ref(VM_STACK_END,
                        (dyn_c*)dyn_get_extern(&tmp2));
        }
        else
            dyn_list_push(env_stack, &tmp);

        continue;
    }

    if(!env->sp)
        goto GOTO__FINISH;

    dyn_list_push(env_stack, &tmp);

    continue;
/*---------------------------------------------------------------------------*/
case SP_SAVE:
/*---------------------------------------------------------------------------*/
// todo add new sp_save
    if (env->new_scope)
        dyn_list_push(env_stack, &none);
    else
        env->new_scope = 1;

    dyn_set_int(&tmp, env->sp);
    dyn_list_push(env_stack, &tmp);
    env->sp = VM_STACK_LEN-1;

    continue;
/*---------------------------------------------------------------------------*/
case CST_N:
/*---------------------------------------------------------------------------*/
    dyn_list_push(env_stack, &none);
    continue;

/*---------------------------------------------------------------------------*/
case CST_0:
/*---------------------------------------------------------------------------*/
    dyn_set_bool(&tmp, 0);
    goto GOTO__PUSH_TMP;

/*---------------------------------------------------------------------------*/
case CST_1:
/*---------------------------------------------------------------------------*/
    dyn_set_bool(&tmp, 1);
    goto GOTO__PUSH_TMP;

/*---------------------------------------------------------------------------*/
case CST_B:
/*---------------------------------------------------------------------------*/
    dyn_set_int(&tmp, (ss_int)*(env->pc++));
    goto GOTO__PUSH_TMP;

/*---------------------------------------------------------------------------*/
case CST_S:
/*---------------------------------------------------------------------------*/
    dyn_set_int(&tmp, *((ss_short*) env->pc));
    env->pc += 2;
    goto GOTO__PUSH_TMP;

/*---------------------------------------------------------------------------*/
case CST_I:
/*---------------------------------------------------------------------------*/
    dyn_set_int(&tmp, *((ss_int*) env->pc));
    env->pc+=4;
    goto GOTO__PUSH_TMP;

/*---------------------------------------------------------------------------*/
case CST_F:
/*---------------------------------------------------------------------------*/
    dyn_set_float(&tmp, *((ss_float*) env->pc));
    env->pc+=4;

/*---------------------------------------------------------------------------*/
GOTO__PUSH_TMP:
/*---------------------------------------------------------------------------*/
    dyn_list_push(env_stack, &tmp);
    continue;

/*---------------------------------------------------------------------------*/
case CST_STR:
/*---------------------------------------------------------------------------*/
    dyn_list_push(env_stack, &none);
    dyn_set_ref( VM_STACK_END,
                 DYN_LIST_GET_REF(&env->data, env->dp+ (ss_byte)*env->pc++));

    continue;

/*---------------------------------------------------------------------------*/
case CST_LST:
/*---------------------------------------------------------------------------*/
    us_len = *((ss_ushort*) env->pc);
    env->pc+=2;
    dyn_set_list_len(&tmp, us_len);

    for(us_i=0; us_i<us_len; ++us_i) {
        dyn_list_push(&tmp, &none);

        dyn_move(dyn_list_get_ref(env_stack, -us_len+us_i),
                 DYN_LIST_GET_REF(&tmp, us_i));
    }

    dyn_list_popi(env_stack, us_len);
    dyn_list_push(env_stack, &none);
    dyn_move(&tmp, VM_STACK_END);

    continue;

/*---------------------------------------------------------------------------*/
case CST_SET:
/*---------------------------------------------------------------------------*/
#ifdef S2_SET
    us_len = *((ss_ushort*) env->pc);
    env->pc+=2;
    dyn_set_set_len(&tmp, us_len);

    for(us_i=0; us_i<us_len; ++us_i)
        dyn_set_insert(&tmp, dyn_list_get_ref(env_stack, -us_len+us_i));

    dyn_list_popi(env_stack, us_len);
    dyn_list_push(env_stack, &none);
    dyn_move(&tmp, VM_STACK_END);

    continue;
#endif
/*---------------------------------------------------------------------------*/
case CST_DCT:
/*---------------------------------------------------------------------------*/
    uc_len = *((ss_byte*) env->pc++);

    dyn_set_dict(&tmp, uc_len);

    for (uc_i=uc_len; uc_i; --uc_i) {
        cp_str = (DYN_LIST_GET_REF(&env->data, env->dp+(ss_byte)*env->pc++))->data.str;

        dyn_dict_insert(&tmp, cp_str, &none);

        dyn_move(VM_STACK_REF_END(uc_i), dyn_dict_get(&tmp, cp_str));
    }
    dyn_list_popi(env_stack, uc_len);
    dyn_list_push(env_stack, &none);
    dyn_move(&tmp, VM_STACK_END);

    continue;

/*---------------------------------------------------------------------------*/
case LOAD:
/*---------------------------------------------------------------------------*/
    dyn_list_push(env_stack, &none);

    cp_str = (DYN_LIST_GET_REF(&env->data, env->dp+(ss_byte)*env->pc++))->data.str;

    dyc_ptr = IF( (us_i = dyn_dict_has_key(&env->memory, cp_str), us_i),
                  (DYN_DICT_GET_I_REF(&env->memory, us_i-1)),
                  IF( (us_i = dyn_dict_has_key(&env->functions, cp_str), us_i),
                      (DYN_DICT_GET_I_REF(&env->functions, us_i-1)),
                      NULL));
    if (dyc_ptr)
        dyn_set_ref(VM_STACK_END, dyc_ptr);
    else
        env->status = VM_ERROR;
    // us_i = dyn_dict_has_key(&env->memory, cp_str);
    //
    // if (us_i) {
    //     dyc_ptr = DYN_DICT_GET_I_REF(&env->memory, us_i-1);
    // }
    // else {
    //     us_i = dyn_dict_has_key(&env->functions, cp_str);
    //     if (us_i)
    //         dyc_ptr = DYN_DICT_GET_I_REF(&env->functions, us_i-1);
    //     else {
    //         env->status = VM_ERROR;
    //         continue;
    //     }
    // }
    // dyn_set_ref(DYN_LIST_GET_END(env_stack), dyc_ptr);

    continue;

/*---------------------------------------------------------------------------*/
case ELEM:
/*---------------------------------------------------------------------------*/
    dyc_ptr = VM_STACK_REF_END(2);            // get list

    uc_i = 0;
    if (DYN_IS_REFERENCE(dyc_ptr)) {
        dyc_ptr = dyc_ptr->data.ref;
        uc_i = 1;
    }

    if (DYN_TYPE(dyc_ptr) == DICT)
        env->loc = dyc_ptr;

    switch (DYN_TYPE(dyc_ptr)) {
#ifdef S2_SET
        case SET:
#endif
        case LIST:      // get value to change
                        dyc_ptr = dyn_list_get_ref(dyc_ptr, dyn_get_int( VM_STACK_END ));
                        if (dyc_ptr == NULL) {
                            env->status = VM_ERROR;
                            continue;
                        }
                        break;
        case DICT:      cp_str = dyn_get_string( VM_STACK_END );
                        if (!dyn_dict_has_key(dyc_ptr, cp_str))
                            dyn_dict_insert(dyc_ptr, cp_str, &none);
                        dyc_ptr = dyn_dict_get(dyc_ptr, cp_str);
                        free(cp_str);
                        break;
//        case STRING:    i_i = dyn_get_int( DYN_LIST_GET_REF_END(env_stack, 1) ); // get value to change
//                        dyn_set_string(&tmp, " ");
//                        break;
    }
    // delete last element (int)
    dyn_list_popi(env_stack, 1);

    // overwrite list with new value
    if (DYN_IS_REFERENCE(dyc_ptr))
        dyc_ptr = dyc_ptr->data.ref;

    if (uc_i)
        dyn_set_ref(VM_STACK_END, dyc_ptr);
    else {
        dyn_move(dyc_ptr, &tmp);
        dyn_move(&tmp, VM_STACK_END);
    }

    continue;

/*---------------------------------------------------------------------------*/
case STORE:
/*---------------------------------------------------------------------------*/
    cp_str = (DYN_LIST_GET_REF(&env->data, env->dp+ (ss_byte)*env->pc++))->data.str;

    dyn_dict_insert(&env->memory, cp_str, &none);

    dyc_ptr = dyn_dict_get(&env->memory, cp_str);

    dyn_move(VM_STACK_END, dyc_ptr);

    dyn_set_ref(VM_STACK_END, dyc_ptr);

    continue;

/*---------------------------------------------------------------------------*/
case STORE_RF:
/*---------------------------------------------------------------------------*/
    dyc_ptr = VM_STACK_REF_END(2);

    dyn_move(VM_STACK_END, dyc_ptr->data.ref);

    dyn_list_popi(env_stack, 1);

    if (env->loc && DYN_TYPE(dyc_ptr->data.ref)==FUNCTION)
        dyn_dict_set_loc(env->loc);

    continue;
/*---------------------------------------------------------------------------*/
case STORE_LOC:
/*---------------------------------------------------------------------------*/
    cp_str = (DYN_LIST_GET_REF(&env->data,
                               env->dp+ (ss_byte)*env->pc++))->data.str;

    dyc_ptr  = VM_STACK_END;
    //dyc_ptr2 = VM_LOCAL;
    us_i = env->sp;

    dyc_ptr2 = find_local(stack, &us_i, cp_str);

    if (!dyc_ptr2) {
        if (DYN_IS_NONE(VM_LOCAL))
            dyn_set_dict(VM_LOCAL, 1);

        dyn_dict_insert(VM_LOCAL, cp_str, &none);
        dyc_ptr2 = DYN_DICT_GET_I_REF(VM_LOCAL, dyn_length(VM_LOCAL)-1);
    }

    dyn_move(dyc_ptr, DYN_IS_REFERENCE(dyc_ptr2) ? dyc_ptr2->data.ref : dyc_ptr2);

    dyn_set_ref(dyc_ptr, dyc_ptr2);

    dyc_ptr2 = NULL;

    continue;
/*---------------------------------------------------------------------------*/
case CALL_OPX:
/*---------------------------------------------------------------------------*/
    dyc_ptr2 = (VM_STACK_REF_END(*env->pc-1))->data.ref;
    dyn_move(dyc_ptr2, VM_STACK_REF_END(*env->pc-1));
/*---------------------------------------------------------------------------*/
case CALL_OP:
/*---------------------------------------------------------------------------*/
    uc_len = (ss_byte)*env->pc++;

    uc_i = vm_op_dispatch(&tmp,
                          dyn_list_get_ref(env_stack, -uc_len-1),
                          uc_len+1,
                          (ss_byte)*env->pc++);

    dyn_list_popi(env_stack, uc_len);

    if (dyc_ptr2) {
        dyn_move(&tmp, dyc_ptr2);
        dyn_set_ref(VM_STACK_END, dyc_ptr2);
        dyc_ptr2 = NULL;
    }
    else
        dyn_move(&tmp, VM_STACK_END);

    if (uc_i != VM_OK)
        env->status = VM_OPERATION_NOT_PERMITTED;

    continue;
/*---------------------------------------------------------------------------*/
case CALL_FCTX:
/*---------------------------------------------------------------------------*/
    dyc_ptr2 = (VM_STACK_REF_END(*env->pc-1))->data.ref;
    dyn_move(dyc_ptr2, VM_STACK_REF_END(*env->pc-1));
/*---------------------------------------------------------------------------*/
case CALL_FCT:
/*---------------------------------------------------------------------------*/
    uc_len = (ss_byte) *env->pc++;

    dyc_ptr = VM_STACK_END;

    if (DYN_IS_REFERENCE(dyc_ptr))
        dyc_ptr = dyc_ptr->data.ref;

    if (DYN_TYPE(dyc_ptr) == FUNCTION) {
        switch (dyc_ptr->data.fct->type) {
            // PROCEDURE
            case 0: {

                dyn_proc *proc = (dyn_proc*) dyc_ptr->data.fct->ptr;

                dyn_copy(&proc->params, &tmp2);

                us_len = VM_STACK_LEN - uc_len - 1;

                us_i = 0;
                if (DYN_NOT_NONE(&proc->params)) {
                    // todo optimize
                    if (DYN_DICT_GET_I_KEY(&tmp2, 0)[0] == 0) {
                        if (DYN_DICT_LEN((&proc->params)) == uc_len)
                            dyn_set_ref(DYN_DICT_GET_I_REF(&tmp2, 0),
                                        VM_STACK_REF(us_i));
                        us_i = 1;
                    }
                }

                for (uc_i = 0; uc_i<uc_len; ++uc_i) {
                    dyn_move(VM_STACK_REF(us_len+uc_i),
                             DYN_DICT_GET_I_REF(&tmp2, uc_i+us_i));
                }


                dyn_list_popi(env_stack, uc_len+1);

                dyn_list_push(env_stack, &none);
                if (dyc_ptr2)
                    dyn_set_extern(VM_STACK_END, dyc_ptr2);

                dyn_set_bool(&tmp, pop);
                dyn_list_push(env_stack, &tmp);
                pop = 0;
                dyn_set_int(&tmp, env->dp);
                dyn_list_push(env_stack, &tmp);
                //env->pc+=1;
                dyn_set_extern(&tmp, (void*)env->pc);
                dyn_list_push(env_stack, &tmp);
                env->pc = dyn_fct_get_ss(dyc_ptr);

                env->dp = DYN_LIST_LEN(&env->data);

                us_len = (ss_ushort)*env->pc;
                env->new_scope = 0;

                dyn_list_push(env_stack, &none);
                dyn_move(&tmp2, VM_STACK_END);

                // init data
                env->pc+=2;
                for (uc_i=0; uc_i<us_len; ++uc_i) {
                    dyn_set_string(&tmp, env->pc);
                    dyn_list_push(&env->data, &tmp);
                    env->pc += ss_strlen(env->pc) + 1;
                }

                continue;

            }
            // Normal C-function
            case 1: {
                fct f = (fct) dyc_ptr->data.fct->ptr;
                uc_i  = (*f)(&tmp, dyn_list_get_ref(env_stack, -uc_len-1), uc_len);
                break;
            }
            // System C-function
            case 2: {
                sys f = (sys) dyc_ptr->data.fct->ptr;
                uc_i  = (*f)(env, &tmp, dyn_list_get_ref(env_stack, -uc_len-1), uc_len);
            }
        }

        dyn_list_popi(env_stack, uc_len);

        if (dyc_ptr2) {
            dyn_move(&tmp, dyc_ptr2);
            dyn_set_ref(VM_STACK_END, dyc_ptr2);
            dyc_ptr2 = NULL;
        }
        else
            dyn_move(&tmp, VM_STACK_END);

        if (uc_i != VM_OK)
            env->status = VM_FUNCTION_ERROR;
    }

    continue;

/*---------------------------------------------------------------------------*/
case FJUMP:
/*---------------------------------------------------------------------------*/
    dyn_list_pop(env_stack, &tmp);
    if (dyn_get_bool_3(&tmp)>0)
        env->pc += 2;
    else
case JUMP:
        env->pc += *((ss_short*)env->pc);

    continue;

/*---------------------------------------------------------------------------*/
case PROC:
/*---------------------------------------------------------------------------*/
    // info
    cp_str = (DYN_LIST_GET_REF(&env->data, env->dp + (ss_byte)*env->pc++))->data.str;
    us_len = *((ss_ushort*) env->pc);  // bytecode length
    env->pc+=2;

    dyn_list_pop(env_stack, &tmp2);

    dyn_set_fct_ss(&tmp,
                   &tmp2,
                   us_len,
                   env->pc,
                   cp_str);

   env->pc += us_len;

   goto GOTO__PUSH_TMP;
/*---------------------------------------------------------------------------*/
case LOC:
/*---------------------------------------------------------------------------*/
    cp_str = (DYN_LIST_GET_REF(&env->data, env->dp+(ss_byte)*env->pc++))->data.str;

    dyn_list_push(env_stack, &none);

    us_i = env->sp;

    dyc_ptr = find_local(stack, &us_i, cp_str);
    if (dyc_ptr)
        dyn_set_ref(VM_STACK_END, dyc_ptr);
    else
        env->status = VM_ERROR;

    continue;
/*---------------------------------------------------------------------------*/
case LOCX:
/*---------------------------------------------------------------------------*/
    cp_str = (DYN_LIST_GET_REF(&env->data, env->dp+(ss_byte)*env->pc++))->data.str;

    dyn_list_pop(env_stack, &tmp);

    us_i = env->sp;

    while(us_i) {
        if (DYN_TYPE(VM_STACK_REF(us_i+1)) == DICT) {
            if(DYN_TYPE(VM_STACK_REF(us_i+2)) == LIST) {
                break;
            }
        }
        us_i = dyn_get_int(VM_STACK_REF(us_i));
    }

    us_len = dyn_get_int(&tmp) - 1 +
             dyn_get_int(VM_STACK_REF(us_i+3));

    if (us_len >= 0) {
        dyn_set_dict(&tmp, dyn_length( VM_STACK_REF(dyn_get_int( VM_STACK_REF(env->sp)))));

        if( vm_get_iterator (&tmp, VM_STACK_REF(us_i+1), us_len)) {
            dyc_ptr = VM_LOCAL;

            if ( cp_str[0] ) {
                us_i = dyn_dict_has_key(dyc_ptr, cp_str);
                if (us_i) {
                    dyc_ptr = DYN_DICT_GET_I_REF(&tmp, us_i+1);
                }
            } else { // empty loc, then last list element and first dict-ref
                dyc_ptr = DYN_DICT_GET_I_REF(&tmp, 0);
            }
            dyn_list_push(env_stack, dyc_ptr);
        }
        else{
            env->status = VM_ERROR;
        }
    }
    else {
        env->status = VM_ERROR;
    }

    continue;
/*---------------------------------------------------------------------------*/
case IT_INIT:
/*---------------------------------------------------------------------------*/
    dyc_ptr = IT_DATA;

    uc_len = DYN_DICT_LEN(dyc_ptr);
    dyn_set_dict(&tmp, uc_len);

    for (uc_i=0; uc_i<uc_len; ++uc_i)
        dyn_dict_insert(&tmp, dyc_ptr->data.dict->key[uc_i], &none);

    dyn_move(&tmp, VM_LOCAL);

    dyn_set_list_len(&tmp, 10);
    dyn_list_push(env_stack, &tmp);
    dyn_set_int(&tmp, 0);
    dyn_list_push(env_stack, &tmp);
    dyn_list_push(env_stack, &tmp);

    continue;

/*---------------------------------------------------------------------------*/
case IT_NEXT0:
case IT_NEXT1:
case IT_NEXT2:
case IT_NEXT3:
/*---------------------------------------------------------------------------*/
{
    dyc_ptr2 = IT_COUNT1;
    i_i = dyn_get_int(dyc_ptr2);

    dyc_ptr = IT_LIST;

    switch (*(env->pc-1)) {
        case IT_NEXT0: {
            if (vm_get_iterator (VM_LOCAL, IT_DATA, i_i)) {
                dyn_set_int(dyc_ptr2, i_i+1);
                dyn_set_bool(&tmp, 1);
            }
            else {
                dyn_set_int(dyc_ptr2, 0);
                dyn_set_bool(&tmp, 0);
            }
            dyn_list_push(env_stack, &tmp);
            break;
        }
        case IT_NEXT1: {
            if (i_i < DYN_LIST_LEN(dyc_ptr) ) {
                vm_get_iterator(VM_LOCAL, IT_DATA, dyn_get_int( DYN_LIST_GET_REF(dyc_ptr, i_i) ));
                dyn_set_int(dyc_ptr2, i_i+1);
            }

            break;
        }
        case IT_NEXT2: {
            i_i = dyn_get_int( IT_COUNT2 );

            if (i_i < DYN_LIST_LEN(dyc_ptr) ) {
                vm_get_iterator(VM_LOCAL, IT_DATA, dyn_get_int( DYN_LIST_GET_REF(dyc_ptr, i_i) ));
            }

            break;
        }
        case IT_NEXT3: {
            if (i_i < DYN_LIST_LEN(dyc_ptr) ) {
                vm_get_iterator(VM_LOCAL, IT_DATA, dyn_get_int( DYN_LIST_GET_REF(dyc_ptr, i_i) ));
                dyn_set_int(dyc_ptr2, i_i+1);
                dyn_set_bool(&tmp, 1);
            }
            else {
                dyn_set_int(dyc_ptr2, 0);
                dyn_set_bool(&tmp, 0);
            }
            dyn_list_push(env_stack, &tmp);
        }
    }

    dyc_ptr2 = NULL;
    continue;
}
/*---------------------------------------------------------------------------*/
case IT_STORE:
/*---------------------------------------------------------------------------*/
    dyn_set_int(&tmp, dyn_get_int(VM_STACK_REF(env->sp+3))-1);
    dyn_list_push(VM_STACK_REF(env->sp+2), &tmp);

    continue;

/*---------------------------------------------------------------------------*/
case IT_LIMIT:
/*---------------------------------------------------------------------------*/
    dyc_ptr = VM_STACK_END;

    dyn_set_bool(dyc_ptr, IF( (dyn_get_int(dyc_ptr) > DYN_LIST_LEN(IT_LIST)),
                              DYN_TRUE,
                              ( dyn_set_int(IT_COUNT1, 0),
                                DYN_FALSE) ));

/*    if (dyn_get_int(dyc_ptr) > DYN_LIST_LEN(IT_LIST)) {
        dyn_set_bool(dyc_ptr, DYN_TRUE);
    }
    else {
        dyn_set_bool(dyc_ptr, DYN_FALSE);
        dyn_set_int(IT_COUNT1, 0);
    }
*/
    continue;

/*---------------------------------------------------------------------------*/
case IT_GROUP:
/*---------------------------------------------------------------------------*/
    uc_len  = *((ss_byte*)env->pc++);
    cp_str  = dyn_get_string(VM_STACK_END);
    dyc_ptr = VM_STACK_REF(env->sp+6);
    us_i    = dyn_dict_has_key(dyc_ptr, cp_str);

    if (!us_i) {
        dyn_dict_insert(dyc_ptr, cp_str, &none);
        us_i = dyn_dict_has_key(dyc_ptr, cp_str);

        if (DYN_TYPE(VM_STACK_REF(env->sp+5)) == DICT) {
            us_len = dyn_length(VM_STACK_REF(env->sp+5));
            dyn_set_dict(&tmp, us_len);
            dyn_set_list_len(&tmp2, 5);
            for (uc_i=0; uc_i<us_len; ++uc_i)
                dyn_dict_insert(&tmp, DYN_DICT_GET_I_KEY(VM_STACK_REF(env->sp+5), uc_i), &tmp2);
        }
        else
            DYN_SET_LIST(&tmp);
        dyn_move(&tmp, DYN_DICT_GET_I_REF(dyc_ptr, us_i-1));
    }
    free(cp_str);

    us_len  = dyn_get_int(VM_STACK_REF(env->sp+3))-1;
    dyc_ptr = DYN_DICT_GET_I_REF(dyc_ptr, us_i-1);
    if(DYN_TYPE(dyc_ptr) == LIST) {
        for (uc_i=0; uc_i<uc_len; ++uc_i) {
            dyn_list_push(dyc_ptr, &none);
            dyn_move(DYN_LIST_GET_REF(VM_STACK_REF(env->sp+5), us_len*uc_len+uc_i),
                     DYN_LIST_GET_END(dyc_ptr));
        }
    }
    else {
        for (uc_i=0; uc_i<uc_len; ++uc_i) {
            dyn_list_push(DYN_DICT_GET_I_REF(dyc_ptr, uc_i), &none);

            dyn_move( DYN_LIST_GET_REF(DYN_DICT_GET_I_REF(VM_STACK_REF(env->sp+5), uc_i), us_len),
                      DYN_LIST_GET_END(DYN_DICT_GET_I_REF(dyc_ptr, uc_i)));
        }
    }

    dyn_list_popi(env_stack, 1);
    continue;

/*---------------------------------------------------------------------------*/
case IT_ORDER:
/*---------------------------------------------------------------------------*/
    dyc_ptr = IT_LIST;

    i_i = dyn_get_int(IT_COUNT1)-1;
    i_j = dyn_get_int(IT_COUNT2);

    if (dyn_get_bool( VM_STACK_END )) {
        dyn_copy( DYN_LIST_GET_REF(dyc_ptr, i_i), &tmp);
        dyn_copy( DYN_LIST_GET_REF(dyc_ptr, i_j),
                  DYN_LIST_GET_REF(dyc_ptr, i_i));
        dyn_copy( &tmp, DYN_LIST_GET_REF(dyc_ptr, i_j));
    }

    if (i_i == ((ss_uint)DYN_LIST_LEN(dyc_ptr))-1) {
        i_j++;
        dyn_set_int(IT_COUNT1, i_j);
        dyn_set_int(IT_COUNT2, i_j);
    }

    if (i_j == DYN_LIST_LEN(dyc_ptr)) {
        dyn_set_int(IT_COUNT1, 0);
        dyn_set_int(IT_COUNT2, 0);
        dyn_set_int(VM_STACK_END, 0);
    } else {
        dyn_set_int(VM_STACK_END, 1);
    }

    continue;

/*---------------------------------------------------------------------------*/
case IT_AS:
/*---------------------------------------------------------------------------*/
    dyc_ptr = VM_STACK_REF(env->sp + 5);
    switch (*env->pc++) {
        case 0: // as void
                dyn_move(VM_STACK_END, dyc_ptr);
                break;
        case 1: // as value
                dyn_move(VM_STACK_REF(env->sp + 6), dyc_ptr);
                break;
#ifdef S2_SET
        case 4:
#endif
        case 2: // as list
                for (i_i= env->sp + 6; i_i<VM_STACK_LEN; ++i_i) {
                    if(*(env->pc-1) == 2)
                        dyn_list_push(dyc_ptr, VM_STACK_REF(i_i));
#ifdef S2_SET
                    else
                        dyn_set_insert(dyc_ptr, VM_STACK_REF(i_i));
#endif
                }
                break;
        case 3: // as dict
                for (i_i= env->sp + 6, uc_i=0;
                     i_i<VM_STACK_LEN;
                     ++i_i, ++uc_i) {
                    dyn_list_push(DYN_DICT_GET_I_REF(dyc_ptr, uc_i),
                                  VM_STACK_REF(env->sp + 6 + uc_i));
                }
                break;
    }
    dyn_list_popi(env_stack, VM_STACK_LEN - env->sp - 6);
    continue;

/*---------------------------------------------------------------------------*/
case EXIT:
/*---------------------------------------------------------------------------*/
    uc_len = (ss_byte)*env->pc++;
    dyn_list_pop(env_stack, &tmp);

    while(uc_len--) {
        us_len = dyn_get_int(VM_STACK_REF(env->sp));

        dyn_list_popi(env_stack, VM_STACK_LEN - env->sp + 1);

        env->sp = us_len;
    }

    dyn_list_push(env_stack, &tmp);
    continue;

/*---------------------------------------------------------------------------*/
case TRY_1:
/*---------------------------------------------------------------------------*/
    us_len = *((ss_short*) env->pc);
    env->pc += 2;
    dyn_set_dict(VM_LOCAL, 1);
    dyn_set_extern(&tmp, env->pc + us_len);

    dyn_dict_insert(VM_LOCAL, "\a", &tmp);

    continue;
/*---------------------------------------------------------------------------*/
case TRY_0:
/*---------------------------------------------------------------------------*/

    dyn_dict_remove(VM_LOCAL, "\a");

    continue;
/*---------------------------------------------------------------------------*/
case REF:
/*---------------------------------------------------------------------------*/
    if (DYN_IS_REFERENCE(VM_STACK_END))
        DYN_TYPE(VM_STACK_END) = REFERENCE2;
    else
        env->status = VM_ERROR;
    continue;
}

}while(1);
/*---------------------------------------------------------------------------*/
GOTO__FINISH:
/*---------------------------------------------------------------------------*/
    dyn_copy(&tmp, &env->rslt);
    dyn_free(&tmp);
    dyn_free(&tmp2);
    vm_reset(env, 0);

    return 1;
}

ss_ushort dict_heigth (dyn_c* dyn)
{
    ss_ushort len = 0;
    ss_ushort max = 0;
    ss_byte i;
    for ( i=0; i<DYN_DICT_LEN(dyn); ++i ) {
        len = dyn_length( DYN_DICT_GET_I_REF(dyn, i) );
        if (len > max)
            max = len;
    }
    return max;
}

ss_char vm_get_iterator (dyn_c* iter_rslt, dyn_c* iter_dict, ss_int count)
{
    ss_ushort us_len = DYN_DICT_LEN(iter_dict);

    dyn_list* rslt_list = iter_rslt->data.dict->value.data.list;

    dyn_c *ptr;

    ss_int j = count;
    ss_ushort us_i;
    for (us_i=0; us_i<us_len; ++us_i) {
        ptr = DYN_DICT_GET_I_REF(iter_dict, us_i);

        if (DYN_IS_REFERENCE(ptr))
            ptr = ptr->data.ref;

        switch (DYN_TYPE(ptr)) {
#ifdef S2_SET
            case SET:
#endif
            case LIST: {
                dyn_set_ref(&rslt_list->container[us_i],
                            &ptr->data.list->container[j % DYN_LIST_LEN(ptr)]);

                j /= DYN_LIST_LEN(ptr);
                break;
            }
            case DICT: {
                ss_short i;

                if (!count) {
                    dyn_set_dict(&rslt_list->container[us_i], DYN_DICT_LEN(ptr));

                    dyn_c none;
                    DYN_INIT(&none);
                    for (i=0; i<DYN_DICT_LEN(ptr); ++i) {
                        dyn_dict_insert(&rslt_list->container[us_i],
                                         DYN_DICT_GET_I_KEY(ptr, i),
                                         &none);
                    }
                }

                for (i=0; i<DYN_DICT_LEN(ptr); ++i) {
                    dyn_set_ref( DYN_DICT_GET_I_REF(&rslt_list->container[us_i], i),
                                 DYN_LIST_GET_REF(DYN_DICT_GET_I_REF(ptr, i),
                                                  j % dict_heigth(ptr)));
                }

                j /= DYN_LIST_LEN(DYN_DICT_GET_I_REF(ptr, 0));

                break;
            }
            default: {
                dyn_set_ref(&rslt_list->container[us_i], ptr);
                break;
            }
        }
    }

    if (!j) {
        return 1;
    }

    return 0;
}

dyn_c* vm_get_rslt (vm_env* env)
{
    return &env->rslt;
}

ss_int vm_size (vm_env* env)
{
    ss_int bytes = sizeof(vm_env);
    bytes += dyn_size(&env->stack);
    bytes += dyn_size(&env->memory);
    bytes += dyn_size(&env->functions);
    bytes += dyn_size(&env->rslt);
    bytes += dyn_size(&env->data);
    return bytes;
}

void vm_reset (vm_env* env, ss_char hard)
{
    DYN_SET_LIST(&env->stack);
    DYN_SET_LIST(&env->data);

    if (hard) {
        dyn_dict_empty(&env->memory);
        dyn_free(&env->rslt);
    }

    env->status = VM_OK;
}

void vm_result (vm_env* env, dyn_c* rslt)
{
    if (vm_ready(env))
        dyn_copy(&env->rslt, rslt);
}

ss_char vm_ready (vm_env* env)
{
    return env->status;
}


ss_char vm_add_variable (vm_env* env, ss_str key, dyn_c* value)
{
    return dyn_dict_insert(&env->memory, key, value);
}

dyn_c* vm_call_variable (vm_env* env, ss_str key)
{
    return dyn_dict_get(&env->memory, key);
}

ss_char vm_add_function (vm_env* env, ss_str key, void *ptr, const ss_str info, ss_char sys)
{
    dyn_c none;
    DYN_INIT(&none);

    if (dyn_dict_insert(&env->functions, key, &none)) {
        if (dyn_set_fct(DYN_DICT_GET_I_REF(&env->functions,
                                            DYN_DICT_LEN((&env->functions))-1),
                        ptr, sys+1, info))
            return 1;
    }

    return 0;
}

ss_char vm_call_function (vm_env* env, ss_str key, dyn_c* rslt, dyn_c params[], ss_byte len)
{
    ss_ushort pos = dyn_dict_has_key(&env->functions, key);

    if (pos) {
        switch (--pos) {
            case 0: return vm_sys_print(env, rslt, params, len);
            case 1: return vm_sys_help (env, rslt, params, len);
            case 2: return vm_sys_mem  (env, rslt, params, len);
            case 3: return vm_sys_del  (env, rslt, params, len);
            default: {
                dyn_c *ptr = DYN_LIST_GET_REF(DYN_DICT_GET_I_REF(&env->functions, pos), 0);

                if (ptr) {
                  fct f = (fct) ptr->data.ex;

                  if(f)
                      return (*f)(rslt, params, len);
                }
            }
        }
    }

    return 0;
}

void  vm_printf (ss_str str, ss_char newline)
{
    for (; *str!='\0'; putc(*str++, stderr));
    if (newline)
        putc('\n', stderr);
    //if (newline)
    //    Serial.println(str);
    //else
    //    Serial.print(str);
}


void vm_free (vm_env* env)
{
    dyn_free(&env->rslt);
    dyn_list_free(&env->stack);
    dyn_dict_free(&env->memory);
    dyn_dict_free(&env->functions);
    dyn_list_free(&env->data);
    free(env);
    return;
}
