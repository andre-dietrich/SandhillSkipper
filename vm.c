#include <stdio.h>
#include <stdlib.h>

#include "vm.h"
#include "vm_op.h"
#include "vm_trace.h"


vm_env* vm_init (ss_ushort memory_size,
                 ss_ushort functions_size,
                 ss_short execution_steps)
{
    vm_env* env = (vm_env*) malloc(sizeof(vm_env));

    env->pc = NULL;

    DYN_INIT(&env->exceptions);
    DYN_SET_LIST(&env->exceptions);

    DYN_INIT(&env->stack);
    DYN_SET_LIST(&env->stack);

    DYN_INIT(&env->rslt);
    dyn_set_none(&env->rslt);

    DYN_INIT(&env->memory);
    dyn_set_dict(&env->memory, memory_size);

    DYN_INIT(&env->functions);
    dyn_set_dict(&env->functions, functions_size+4);

    env->new_scope = 1;
    env->loc = NULL;
    DYN_INIT(&env->local);
    DYN_SET_LIST(&env->local);

    env->dp = 0;
    DYN_INIT(&env->data);
    DYN_SET_LIST(&env->data);

    env->status = VM_OK;
    env->execution_steps = execution_steps;

    env->operator_list[NOT] = op_NOT;
    env->operator_list[NEG] = op_NEG;

    env->operator_list[ADD] = op_ADD;
    env->operator_list[SUB] = op_SUB;
    env->operator_list[MUL] = op_MUL;
    env->operator_list[DIV] = op_DIV;
    env->operator_list[MOD] = op_MOD;
    env->operator_list[POW] = op_POW;

    env->operator_list[LT]  = op_LT;
    env->operator_list[LE]  = op_LE;
    env->operator_list[GE]  = op_GE;
    env->operator_list[GT]  = op_GT;
    env->operator_list[NE]  = op_NE;
    env->operator_list[EQ]  = op_EQ;

    env->operator_list[AND] = op_AND;
    env->operator_list[OR]  = op_OR;
    env->operator_list[XOR] = op_XOR;
    env->operator_list[IN]  = op_IN;

    env->operator_list[B_NOT] = op_B_NOT;
    env->operator_list[B_AND] = op_B_AND;
    env->operator_list[B_OR]  = op_B_OR;
    env->operator_list[B_XOR] = op_B_XOR;

    env->operator_list[B_SHIFT_L] = op_B_SHIFT_L;
    env->operator_list[B_SHIFT_R] = op_B_SHIFT_R;

    env->operator_list[EX] = op_EX;


    vm_add_function(env, (ss_str)"print", (void*)vm_sys_print,    (ss_str) "", 1); //"print: prints out the passed parameters, the last defines the return value ...");
    vm_add_function(env, (ss_str)"help",  (void*)vm_sys_help,     (ss_str) "", 1); //"general help function ...");
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

    return env;
}

ss_char vm_execute (vm_env* env, ss_char* code, ss_char trace) {

    dyn_c tmp, tmp2;
    DYN_INIT(&tmp);
    DYN_INIT(&tmp2);

    dyn_c none;
    DYN_INIT(&none);

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
    ss_byte   _exit_  = 0;


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

    static void* dispatch_table[] = {
          &&GOTO__RET, &&GOTO__RET_L, &&GOTO__RET_P,
          &&GOTO__SP_SAVE,

          &&GOTO__CST_N,    &&GOTO__CST_0,   &&GOTO__CST_1,
          &&GOTO__CST_B,    &&GOTO__CST_S,
          &&GOTO__CST_I,    &&GOTO__CST_F,

          &&GOTO__CST_STR,  &&GOTO__CST_LST,
          &&GOTO__CST_SET,  &&GOTO__CST_DCT,

          &&GOTO__LOAD,     &&GOTO__ELEM,
          &&GOTO__STORE,    &&GOTO__STORE_RF, &&GOTO__STORE_LOC,

          &&GOTO__CALL_OPX,
          &&GOTO__CALL_OP,

          &&GOTO__CALL_FCTX,
          &&GOTO__CALL_FCT,

          &&GOTO__FJUMP,    &&GOTO__JUMP,

          &&GOTO__PROC,

          &&GOTO__LOC,      &&GOTO__LOCX,

          &&GOTO__IT_INIT,
          &&GOTO__IT_NEXT0, &&GOTO__IT_NEXT1,
          &&GOTO__IT_NEXT2, &&GOTO__IT_NEXT3,

          &&GOTO__IT_STORE, &&GOTO__IT_LIMIT,

          &&GOTO__IT_GROUP, &&GOTO__IT_ORDER,
          &&GOTO__IT_AS,

          &&GOTO__EXIT,

          &&GOTO__TRY,

          &&GOTO__REF
	      };
/*---------------------------------------------------------------------------*/
GOTO__DISPACH:
/*---------------------------------------------------------------------------*/
    dyn_free(&tmp);
    dyn_free(&tmp2);

    if (env->status != VM_IDLE) {
        if (DYN_LIST_LEN(&env->exceptions) > 0){
            env->status = VM_IDLE;
            dyn_list_pop(&env->exceptions, &tmp);
            env->pc = (char*)dyn_get_extern(&tmp);

            dyn_list_pop(&env->exceptions, &tmp);
            env->sp = dyn_get_int(&tmp);

            dyn_list_popi(&env->stack, DYN_LIST_LEN(&env->stack) - env->sp);
        }
        else
            return env->status;
    }

    if (pop) {
        if (*env->pc != EXIT) {
            dyn_list_popi(&env->stack, 1);
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
    if (trace) {
        vm_trace(env, code);

        //ss_str dict = dyn_get_string(&env->local);
        //vm_printf(dict, 1);
        //free(dict);
    }
#endif
    if (POP & *env->pc)
        pop = 1;

    //printf("DICT: %s\n", dyn_get_string(&env->local));

    goto *dispatch_table[(ss_byte)(POP_I & *env->pc++)];
/*---------------------------------------------------------------------------*/
GOTO__RET:
GOTO__RET_L:
GOTO__RET_P:
/*---------------------------------------------------------------------------*/

    dyn_list_pop(&env->stack, &tmp); // return value

    us_len = dyn_get_int(DYN_LIST_GET_REF(&env->stack, env->sp-1));
    dyn_list_popi(&env->stack, DYN_LIST_LEN(&env->stack) - env->sp + 1);

    env->sp = us_len;

    dyn_list_popi(&env->local, 1);

    if (*(env->pc-1) == RET_P) {
        dyn_list_pop(&env->stack, &tmp2);
        env->pc = (ss_char*)dyn_get_extern(&tmp2);
        dyn_list_pop(&env->stack, &tmp2);
        env->dp = dyn_get_int(&tmp2);
        dyn_list_pop(&env->stack, &tmp2);
        pop = dyn_get_bool(&tmp2);
        dyn_list_pop(&env->stack, &tmp2);
        if (DYN_NOT_NONE(&tmp2)) {
            dyn_move(&tmp, (dyn_c*)dyn_get_extern(&tmp2));
            dyn_list_push(&env->stack, &none);
            dyn_set_ref(DYN_LIST_GET_END(&env->stack),
                        (dyn_c*)dyn_get_extern(&tmp2));
        }
        else
            dyn_list_push(&env->stack, &tmp);

        goto GOTO__DISPACH;
    }

    if (_exit_)
        goto GOTO__EXIT;

    if(!env->sp)
        goto GOTO__FINISH;

    dyn_list_push(&env->stack, &tmp);

    goto GOTO__DISPACH;
/*---------------------------------------------------------------------------*/
GOTO__SP_SAVE:
/*---------------------------------------------------------------------------*/
    dyn_set_int(&tmp, env->sp);
    dyn_list_push(&env->stack, &tmp);
    env->sp = DYN_LIST_LEN(&env->stack);

    if (env->new_scope)
        dyn_list_push(&env->local, &none);
    else
        env->new_scope = 1;

    goto GOTO__DISPACH;
/*---------------------------------------------------------------------------*/
GOTO__CST_N:
/*---------------------------------------------------------------------------*/
    dyn_list_push(&env->stack, &none);
    goto GOTO__DISPACH;

/*---------------------------------------------------------------------------*/
GOTO__CST_0:
/*---------------------------------------------------------------------------*/
    dyn_set_bool(&tmp, 0);
    goto GOTO__PUSH_TMP;

/*---------------------------------------------------------------------------*/
GOTO__CST_1:
/*---------------------------------------------------------------------------*/
    dyn_set_bool(&tmp, 1);
    goto GOTO__PUSH_TMP;

/*---------------------------------------------------------------------------*/
GOTO__CST_B:
/*---------------------------------------------------------------------------*/
    dyn_set_int(&tmp, (ss_int)*(env->pc++));
    goto GOTO__PUSH_TMP;

/*---------------------------------------------------------------------------*/
GOTO__CST_S:
/*---------------------------------------------------------------------------*/
    dyn_set_int(&tmp, *((ss_short*) env->pc));
    env->pc += 2;
    goto GOTO__PUSH_TMP;

/*---------------------------------------------------------------------------*/
GOTO__CST_I:
/*---------------------------------------------------------------------------*/
    dyn_set_int(&tmp, *((ss_int*) env->pc));
    env->pc+=4;
    goto GOTO__PUSH_TMP;

/*---------------------------------------------------------------------------*/
GOTO__CST_F:
/*---------------------------------------------------------------------------*/
    dyn_set_float(&tmp, *((ss_float*) env->pc));
    env->pc+=4;

/*---------------------------------------------------------------------------*/
GOTO__PUSH_TMP:
/*---------------------------------------------------------------------------*/
    dyn_list_push(&env->stack, &none);
    dyn_move(&tmp, DYN_LIST_GET_END(&env->stack));
    goto GOTO__DISPACH;

/*---------------------------------------------------------------------------*/
GOTO__CST_STR:
/*---------------------------------------------------------------------------*/
    dyn_list_push(&env->stack, &none);
    dyn_set_ref( DYN_LIST_GET_REF_END(&env->stack, 1),
                 DYN_LIST_GET_REF    (&env->data, env->dp+ (ss_byte)*env->pc++));

    goto GOTO__DISPACH;

/*---------------------------------------------------------------------------*/
GOTO__CST_LST:
/*---------------------------------------------------------------------------*/
    us_len = *((ss_ushort*) env->pc);
    env->pc+=2;
    dyn_set_list_len(&tmp, us_len);

    for(us_i=0; us_i<us_len; ++us_i) {
        dyn_list_push(&tmp, &none);

        dyn_move(dyn_list_get_ref(&env->stack, -us_len+us_i),
                 DYN_LIST_GET_REF(&tmp, us_i));
    }

    dyn_list_popi(&env->stack, us_len);

    goto GOTO__PUSH_TMP;;

/*---------------------------------------------------------------------------*/
GOTO__CST_SET:
/*---------------------------------------------------------------------------*/
//#ifdef S2_SET
    us_len = *((ss_ushort*) env->pc);
    env->pc+=2;
    dyn_set_set_len(&tmp, us_len);

    for(us_i=0; us_i<us_len; ++us_i)
        dyn_set_insert(&tmp, dyn_list_get_ref(&env->stack, -us_len+us_i));

    dyn_list_popi(&env->stack, us_len);

    goto GOTO__PUSH_TMP;
//#endif
/*---------------------------------------------------------------------------*/
GOTO__CST_DCT:
/*---------------------------------------------------------------------------*/
    uc_len = *((ss_byte*) env->pc++);

    dyn_set_dict(&tmp, uc_len);

    for (uc_i=uc_len; uc_i; --uc_i) {
        cp_str = (DYN_LIST_GET_REF(&env->data, env->dp+(ss_byte)*env->pc++))->data.str;

        dyn_dict_insert(&tmp, cp_str, &none);

        dyn_move(DYN_LIST_GET_REF_END(&env->stack, uc_i),
                 dyn_dict_get(&tmp, cp_str));
    }
    dyn_list_popi(&env->stack, uc_len);

    goto GOTO__PUSH_TMP;

/*---------------------------------------------------------------------------*/
GOTO__LOAD:
/*---------------------------------------------------------------------------*/
    dyn_list_push(&env->stack, &none);

    cp_str = (DYN_LIST_GET_REF(&env->data, env->dp+(ss_byte)*env->pc++))->data.str;

    us_i = dyn_dict_has_key(&env->memory, cp_str);

    if (us_i) {
        dyc_ptr = DYN_DICT_GET_I_REF(&env->memory, us_i-1);
    }
    else {
        us_i = dyn_dict_has_key(&env->functions, cp_str);
        if (us_i)
            dyc_ptr = DYN_DICT_GET_I_REF(&env->functions, us_i-1);
        else {
            env->status = VM_ERROR;
            goto GOTO__DISPACH;
        }
    }

    dyn_set_ref(DYN_LIST_GET_END(&env->stack), dyc_ptr);

    goto GOTO__DISPACH;

/*---------------------------------------------------------------------------*/
GOTO__ELEM:
/*---------------------------------------------------------------------------*/
    dyc_ptr = DYN_LIST_GET_REF_END(&env->stack, 2);            // get list

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
        case LIST:      i_i = dyn_get_int( DYN_LIST_GET_END(&env->stack) ); // get value to change
                        dyc_ptr = dyn_list_get_ref(dyc_ptr, i_i);
                        if (dyc_ptr == NULL) {
                            env->status = VM_ERROR;
                            goto GOTO__DISPACH;
                        }
                        break;
        case DICT:     cp_str = dyn_get_string( DYN_LIST_GET_END(&env->stack) );
                        if (!dyn_dict_has_key(dyc_ptr, cp_str))
                            dyn_dict_insert(dyc_ptr, cp_str, &none);
                        dyc_ptr = dyn_dict_get(dyc_ptr, cp_str);
                        free(cp_str);
                        break;
//        case STRING:    i_i = dyn_get_int( DYN_LIST_GET_REF_END(&env->stack, 1) ); // get value to change
//                        dyn_set_string(&tmp, " ");
//                        break;
    }
    // delete last element (int)
    dyn_list_popi(&env->stack, 1);

    // overwrite list with new value
    if (DYN_IS_REFERENCE(dyc_ptr))
        dyc_ptr = dyc_ptr->data.ref;

    if (uc_i)
        dyn_set_ref(DYN_LIST_GET_END(&env->stack), dyc_ptr);
    else {
        dyn_move(dyc_ptr, &tmp);
        dyn_move(&tmp, DYN_LIST_GET_END(&env->stack));
    }

    goto GOTO__DISPACH;

/*---------------------------------------------------------------------------*/
GOTO__STORE:
/*---------------------------------------------------------------------------*/
    cp_str = (DYN_LIST_GET_REF(&env->data, env->dp+ (ss_byte)*env->pc++))->data.str;

    dyn_dict_insert(&env->memory, cp_str, &none);

    dyc_ptr = dyn_dict_get(&env->memory, cp_str);

    dyn_move(DYN_LIST_GET_END(&env->stack), dyc_ptr);

    dyn_set_ref(DYN_LIST_GET_END(&env->stack), dyc_ptr);

    goto GOTO__DISPACH;

/*---------------------------------------------------------------------------*/
GOTO__STORE_RF:
/*---------------------------------------------------------------------------*/
    dyc_ptr = DYN_LIST_GET_REF_END(&env->stack, 2);

    dyn_move(DYN_LIST_GET_END(&env->stack), dyc_ptr->data.ref);

    dyn_list_popi(&env->stack, 1);

    if (env->loc && DYN_TYPE(dyc_ptr->data.ref)==FUNCTION)
        dyn_dict_set_loc(env->loc);

    goto GOTO__DISPACH;
/*---------------------------------------------------------------------------*/
GOTO__STORE_LOC:
/*---------------------------------------------------------------------------*/
    cp_str = (DYN_LIST_GET_REF(&env->data, env->dp+ (ss_byte)*env->pc++))->data.str;

    dyc_ptr = dyn_list_get_ref(&env->stack, -1);

    if (DYN_IS_NONE(DYN_LIST_GET_END(&env->local)))
        dyn_set_dict(DYN_LIST_GET_END(&env->local),1);

    us_i = dyn_dict_has_key(DYN_LIST_GET_END(&env->local), cp_str);

    if (!us_i) {
        us_i = dyn_length(DYN_LIST_GET_END(&env->local));
        dyn_dict_insert(DYN_LIST_GET_END(&env->local), cp_str, &none);
    }
    else
        --us_i;

    if ( DYN_IS_REFERENCE(DYN_DICT_GET_I_REF(DYN_LIST_GET_END(&env->local), us_i)) ){
        dyn_move(dyc_ptr, (DYN_DICT_GET_I_REF(DYN_LIST_GET_END(&env->local), us_i))->data.ref);
    }
    else
        dyn_move(dyc_ptr, DYN_DICT_GET_I_REF(DYN_LIST_GET_END(&env->local), us_i));

    dyn_set_ref(dyc_ptr, DYN_DICT_GET_I_REF(DYN_LIST_GET_END(&env->local), us_i));

    goto GOTO__DISPACH;
/*---------------------------------------------------------------------------*/
GOTO__CALL_OPX:
/*---------------------------------------------------------------------------*/
    dyc_ptr2 = (DYN_LIST_GET_REF_END(&env->stack, *env->pc-1))->data.ref;
    dyn_move(dyc_ptr2, DYN_LIST_GET_REF_END(&env->stack, *env->pc-1));
/*---------------------------------------------------------------------------*/
GOTO__CALL_OP:
/*---------------------------------------------------------------------------*/
    uc_len = (ss_byte)*env->pc++;

    uc_i = (env->operator_list[(ss_byte)*env->pc++])(&tmp, dyn_list_get_ref(&env->stack, -uc_len-1), uc_len+1);

    dyn_list_popi(&env->stack, uc_len);

    if (dyc_ptr2) {
        dyn_move(&tmp, dyc_ptr2);
        dyn_set_ref(DYN_LIST_GET_END(&env->stack), dyc_ptr2);
        dyc_ptr2 = NULL;
    }
    else
        dyn_move(&tmp, DYN_LIST_GET_END(&env->stack));

    if (uc_i != VM_OK)
        env->status = VM_OPERATION_NOT_PERMITTED;

    goto GOTO__DISPACH;
/*---------------------------------------------------------------------------*/
GOTO__CALL_FCTX:
/*---------------------------------------------------------------------------*/
    dyc_ptr2 = (DYN_LIST_GET_REF_END(&env->stack, *env->pc-1))->data.ref;
    dyn_move(dyc_ptr2, DYN_LIST_GET_REF_END(&env->stack, *env->pc-1));
/*---------------------------------------------------------------------------*/
GOTO__CALL_FCT:
/*---------------------------------------------------------------------------*/
    uc_len = (ss_byte) *env->pc++;

    dyc_ptr = DYN_LIST_GET_END(&env->stack);

    if (DYN_IS_REFERENCE(dyc_ptr))
        dyc_ptr = dyc_ptr->data.ref;

    if (DYN_TYPE(dyc_ptr) == FUNCTION) {
        switch (dyc_ptr->data.fct->type) {
            // PROCEDURE
            case 0: {

                dyn_proc *proc = (dyn_proc*) dyc_ptr->data.fct->ptr;

                dyn_list_push(&env->local, &proc->params);

                us_len = DYN_LIST_LEN(&env->stack) - uc_len - 1;

                us_i = 0;
                if (DYN_NOT_NONE(&proc->params)) {
                    if (!ss_strcmp(DYN_DICT_GET_I_KEY(&proc->params, 0), (char*) "")) {
                        if (dyn_length(&proc->params) == uc_len)
                            dyn_set_ref(DYN_DICT_GET_I_REF(DYN_LIST_GET_END(&env->local), 0),
                                        DYN_LIST_GET_REF(&env->stack, us_i));
                        us_i = 1;
                    }
                }

                for (uc_i = 0; uc_i<uc_len; ++uc_i) {
                    dyn_move(DYN_LIST_GET_REF(&env->stack, us_len+uc_i),
                             DYN_DICT_GET_I_REF(DYN_LIST_GET_END(&env->local), uc_i+us_i));
                }
                dyn_list_popi(&env->stack, uc_len+1);
                goto GOTO__EXEC_PROC;
            }
            // Normal C-function
            case 1: {
                fct f = (fct) dyc_ptr->data.fct->ptr;
                uc_i  = (*f)(&tmp, dyn_list_get_ref(&env->stack, -uc_len-1), uc_len);
                break;
            }
            // System C-function
            case 2: {
                sys f = (sys) dyc_ptr->data.fct->ptr;
                uc_i  = (*f)(env, &tmp, dyn_list_get_ref(&env->stack, -uc_len-1), uc_len);
            }
        }

        dyn_list_popi(&env->stack, uc_len);

        if (dyc_ptr2) {
            dyn_move(&tmp, dyc_ptr2);
            dyn_set_ref(DYN_LIST_GET_END(&env->stack), dyc_ptr2);
            dyc_ptr2 = NULL;
        }
        else
            dyn_move(&tmp, DYN_LIST_GET_END(&env->stack));

        if (uc_i != VM_OK)
            env->status = VM_FUNCTION_ERROR;
    }

    goto GOTO__DISPACH;

/*---------------------------------------------------------------------------*/
GOTO__FJUMP:
/*---------------------------------------------------------------------------*/
    dyn_list_pop(&env->stack, &tmp);
    if (dyn_get_bool_3(&tmp)>0)
        env->pc += 2;
    else
GOTO__JUMP:
        env->pc += *((ss_short*)env->pc);

    goto GOTO__DISPACH;

/*---------------------------------------------------------------------------*/
GOTO__PROC:
/*---------------------------------------------------------------------------*/
    // info
    cp_str = (DYN_LIST_GET_REF(&env->data, env->dp + (ss_byte)*env->pc++))->data.str;
    us_len = *((ss_ushort*) env->pc);  // bytecode length
    env->pc+=2;

    dyn_list_pop(&env->stack, &tmp2);

    dyn_set_fct_ss(&tmp,
                   &tmp2,
                   us_len,
                   env->pc,
                   cp_str);

    env->pc += us_len;

    goto GOTO__PUSH_TMP;
/*---------------------------------------------------------------------------*/
GOTO__LOC:
/*---------------------------------------------------------------------------*/
    cp_str = (DYN_LIST_GET_REF(&env->data, env->dp+(ss_byte)*env->pc++))->data.str;

    us_i = DYN_LIST_LEN(&env->local);
    while (us_i--) {

        dyc_ptr = DYN_LIST_GET_REF(&env->local, us_i);

        if (DYN_IS_NONE(dyc_ptr))
            continue;

        if ( cp_str[0] ) {
            us_len = dyn_dict_has_key(dyc_ptr, cp_str);
            if (us_len) {
                dyc_ptr = DYN_DICT_GET_I_REF(dyc_ptr, --us_len);
                break;
            }
        } else {
            dyc_ptr = DYN_DICT_GET_I_REF(dyc_ptr, 0);
            break;
        }
    }

    dyn_list_push(&env->stack, &none);
    dyn_set_ref(DYN_LIST_GET_END(&env->stack), dyc_ptr);

    goto GOTO__DISPACH;
/*---------------------------------------------------------------------------*/
GOTO__LOCX:
/*---------------------------------------------------------------------------*/
    cp_str = (DYN_LIST_GET_REF(&env->data, env->dp+(ss_byte)*env->pc++))->data.str;

    dyn_list_pop(&env->stack, &tmp);

    us_i = env->sp;

    while(us_i) {
        if (DYN_TYPE(DYN_LIST_GET_REF(&env->stack, us_i)) == DICT) {
            if(DYN_TYPE(DYN_LIST_GET_REF(&env->stack, us_i+1)) == LIST) {
                break;
            }
        }
        us_i = dyn_get_int(DYN_LIST_GET_REF(&env->stack, us_i-1));
    }

    us_len = dyn_get_int(&tmp) - 1 +
             dyn_get_int(DYN_LIST_GET_REF(&env->stack, us_i+2));

    if (us_len >= 0) {
        dyn_set_dict(&tmp, dyn_length(DYN_LIST_GET_REF_END(&env->local, -1)));

        if( vm_get_iterator (&tmp, DYN_LIST_GET_REF(&env->stack, us_i), us_len)) {
            dyc_ptr = DYN_LIST_GET_END(&env->local);

            if ( cp_str[0] ) {
                us_i = dyn_dict_has_key(dyc_ptr, cp_str);
                if (us_i) {
                    dyc_ptr = DYN_DICT_GET_I_REF(&tmp, us_i-1);
                }
            } else { // empty loc, then last list element and first dict-ref
                dyc_ptr = DYN_DICT_GET_I_REF(&tmp, 0);
            }
            dyn_list_push(&env->stack, dyc_ptr);
        }
        else
            env->status = VM_ERROR;
    }
    else {
        env->status = VM_ERROR;
    }

    goto GOTO__DISPACH;
/*---------------------------------------------------------------------------*/
GOTO__IT_INIT:
/*---------------------------------------------------------------------------*/
    dyc_ptr = DYN_LIST_GET_REF(&env->stack, env->sp);

    uc_len = dyn_length(dyc_ptr);
    dyn_set_dict(&tmp, uc_len);

    for (uc_i=0; uc_i<uc_len; ++uc_i)
        dyn_dict_insert(&tmp, dyc_ptr->data.dict->key[uc_i], &none);

    dyn_move(&tmp, DYN_LIST_GET_END(&env->local));

    dyn_set_list_len(&tmp, 0);
    dyn_list_push(&env->stack, &tmp);
    dyn_set_int(&tmp, 0);
    dyn_list_push(&env->stack, &tmp);
    dyn_list_push(&env->stack, &tmp);

    goto GOTO__DISPACH;

/*---------------------------------------------------------------------------*/
GOTO__IT_NEXT0:
GOTO__IT_NEXT1:
GOTO__IT_NEXT2:
GOTO__IT_NEXT3:
/*---------------------------------------------------------------------------*/
{
    dyc_ptr2 = DYN_LIST_GET_REF(&env->stack, env->sp+2);
    i_i = dyn_get_int(dyc_ptr2);

    dyc_ptr = DYN_LIST_GET_REF(&env->stack, env->sp+1);

    switch (*(env->pc-1)) {
        case IT_NEXT0: {
            if (vm_get_iterator (DYN_LIST_GET_END(&env->local), DYN_LIST_GET_REF(&env->stack, env->sp), i_i)) {
                dyn_set_int(dyc_ptr2, i_i+1);
                dyn_set_bool(&tmp, 1);
            }
            else {
                dyn_set_int(dyc_ptr2, 0);
                dyn_set_bool(&tmp, 0);
            }
            dyn_list_push(&env->stack, &tmp);
            break;
        }
        case IT_NEXT1: {
            if (i_i < DYN_LIST_LEN(dyc_ptr) ) {
                vm_get_iterator(DYN_LIST_GET_END(&env->local), DYN_LIST_GET_REF(&env->stack, env->sp), dyn_get_int( DYN_LIST_GET_REF(dyc_ptr, i_i) ));
                dyn_set_int(dyc_ptr2, i_i+1);
            }

            break;
        }
        case IT_NEXT2: {
            i_i = dyn_get_int( DYN_LIST_GET_REF(&env->stack, env->sp+3) );

            if (i_i < DYN_LIST_LEN(dyc_ptr) ) {
                vm_get_iterator(DYN_LIST_GET_END(&env->local), DYN_LIST_GET_REF(&env->stack, env->sp), dyn_get_int( DYN_LIST_GET_REF(dyc_ptr, i_i) ));
            }

            break;
        }
        case IT_NEXT3: {
            if (i_i < DYN_LIST_LEN(dyc_ptr) ) {
                vm_get_iterator(DYN_LIST_GET_END(&env->local), DYN_LIST_GET_REF(&env->stack, env->sp), dyn_get_int( DYN_LIST_GET_REF(dyc_ptr, i_i) ));
                dyn_set_int(dyc_ptr2, i_i+1);
                dyn_set_bool(&tmp, 1);
            }
            else {
                dyn_set_int(dyc_ptr2, 0);
                dyn_set_bool(&tmp, 0);
            }
            dyn_list_push(&env->stack, &tmp);
        }
    }

    dyc_ptr2 = NULL;
    goto GOTO__DISPACH;
}
/*---------------------------------------------------------------------------*/
GOTO__IT_STORE:
/*---------------------------------------------------------------------------*/
    dyn_set_int(&tmp, dyn_get_int(DYN_LIST_GET_REF(&env->stack, env->sp+2))-1);
    dyn_list_push(DYN_LIST_GET_REF(&env->stack, env->sp+1), &tmp);

    goto GOTO__DISPACH;

/*---------------------------------------------------------------------------*/
GOTO__IT_LIMIT:
/*---------------------------------------------------------------------------*/
    dyc_ptr = DYN_LIST_GET_END(&env->stack);

    if (dyn_get_int(dyc_ptr) > DYN_LIST_LEN(DYN_LIST_GET_REF(&env->stack, env->sp+1))) {
        dyn_set_bool(dyc_ptr, DYN_TRUE);
    }
    else {
        dyn_set_bool(dyc_ptr, DYN_FALSE);
        dyn_set_int(DYN_LIST_GET_REF(&env->stack, env->sp+2), 0);
    }

    goto GOTO__DISPACH;

/*---------------------------------------------------------------------------*/
GOTO__IT_GROUP:
/*---------------------------------------------------------------------------*/
    uc_len  = *((ss_byte*)env->pc++);
    cp_str  = dyn_get_string(DYN_LIST_GET_END(&env->stack));
    dyc_ptr = DYN_LIST_GET_REF(&env->stack, env->sp+5);
    us_i    = dyn_dict_has_key(dyc_ptr, cp_str);

    if (!us_i) {
        dyn_dict_insert(dyc_ptr, cp_str, &none);
        us_i = dyn_dict_has_key(dyc_ptr, cp_str);

        if (DYN_TYPE(DYN_LIST_GET_REF(&env->stack, env->sp+4)) == DICT) {
            us_len = dyn_length(DYN_LIST_GET_REF(&env->stack, env->sp+4));
            dyn_set_dict(&tmp, us_len);
            dyn_set_list_len(&tmp2, 5);
            for (uc_i=0; uc_i<us_len; ++uc_i)
                dyn_dict_insert(&tmp, DYN_DICT_GET_I_KEY(DYN_LIST_GET_REF(&env->stack, env->sp+4), uc_i), &tmp2);
        }
        else
            DYN_SET_LIST(&tmp);
        dyn_move(&tmp, DYN_DICT_GET_I_REF(dyc_ptr, us_i-1));
    }
    free(cp_str);

    us_len  = dyn_get_int(DYN_LIST_GET_REF(&env->stack, env->sp+2))-1;
    dyc_ptr = DYN_DICT_GET_I_REF(dyc_ptr, us_i-1);
    if(DYN_TYPE(dyc_ptr) == LIST) {
        for (uc_i=0; uc_i<uc_len; ++uc_i) {
            dyn_list_push(dyc_ptr, &none);
            dyn_move(DYN_LIST_GET_REF(DYN_LIST_GET_REF(&env->stack, env->sp+4), us_len*uc_len+uc_i),
                     DYN_LIST_GET_END(dyc_ptr));
        }
    }
    else {
        for (uc_i=0; uc_i<uc_len; ++uc_i) {
            dyn_list_push(DYN_DICT_GET_I_REF(dyc_ptr, uc_i), &none);

            dyn_move( DYN_LIST_GET_REF(DYN_DICT_GET_I_REF(DYN_LIST_GET_REF(&env->stack, env->sp+4), uc_i), us_len),
                      DYN_LIST_GET_END(DYN_DICT_GET_I_REF(dyc_ptr, uc_i)));
        }
    }

    dyn_list_popi(&env->stack, 1);
    goto GOTO__DISPACH;

/*---------------------------------------------------------------------------*/
GOTO__IT_ORDER:
/*---------------------------------------------------------------------------*/
    dyc_ptr = DYN_LIST_GET_REF(&env->stack, env->sp+1);

    i_i = dyn_get_int(DYN_LIST_GET_REF(&env->stack, env->sp+2))-1;
    i_j = dyn_get_int(DYN_LIST_GET_REF(&env->stack, env->sp+3));

    if (dyn_get_bool( dyn_list_get_ref(&env->stack, -1))) {
        dyn_copy( DYN_LIST_GET_REF(dyc_ptr, i_i), &tmp);
        dyn_copy( DYN_LIST_GET_REF(dyc_ptr, i_j),
                  DYN_LIST_GET_REF(dyc_ptr, i_i));
        dyn_copy( &tmp, DYN_LIST_GET_REF(dyc_ptr, i_j));
    }

    if (i_i == ((ss_uint)DYN_LIST_LEN(dyc_ptr))-1) {
        i_j++;
        dyn_set_int(DYN_LIST_GET_REF(&env->stack, env->sp+2), i_j);
        dyn_set_int(DYN_LIST_GET_REF(&env->stack, env->sp+3), i_j);
    }

    if (i_j == DYN_LIST_LEN(dyc_ptr)) {
        dyn_set_int(DYN_LIST_GET_REF(&env->stack, env->sp+2), 0);
        dyn_set_int(DYN_LIST_GET_REF(&env->stack, env->sp+3), 0);
        dyn_set_int(dyn_list_get_ref(&env->stack, -1), 0);
    } else {
        dyn_set_int(dyn_list_get_ref(&env->stack, -1), 1);
    }

    goto GOTO__DISPACH;

/*---------------------------------------------------------------------------*/
GOTO__IT_AS:
/*---------------------------------------------------------------------------*/
    dyc_ptr = DYN_LIST_GET_REF(&env->stack, env->sp + 4);
    switch (*env->pc++) {
        case 0: // as void
                dyn_move(DYN_LIST_GET_END(&env->stack), dyc_ptr);
                break;
        case 1: // as value
                dyn_move(DYN_LIST_GET_REF(&env->stack, env->sp + 5), dyc_ptr);
                break;
#ifdef S2_SET
        case 4:
#endif
        case 2: // as list
                for (i_i= env->sp + 5; i_i<DYN_LIST_LEN(&env->stack); ++i_i) {
                    if(*(env->pc-1) == 2)
                        dyn_list_push(dyc_ptr, DYN_LIST_GET_REF(&env->stack, i_i));
#ifdef S2_SET
                    else
                        dyn_set_insert(dyc_ptr, DYN_LIST_GET_REF(&env->stack, i_i));
#endif
                }
                break;
        case 3: // as dict
                for (i_i= env->sp + 5, uc_i=0;
                     i_i<DYN_LIST_LEN(&env->stack);
                     ++i_i, ++uc_i) {
                    dyn_list_push(DYN_DICT_GET_I_REF(dyc_ptr, uc_i),
                                  DYN_LIST_GET_REF(&env->stack, (env->sp + 5 + uc_i)));
                }
                break;
    }
    dyn_list_popi(&env->stack, DYN_LIST_LEN(&env->stack) - env->sp - 5);
    goto GOTO__DISPACH;

/*---------------------------------------------------------------------------*/
GOTO__EXIT:
/*---------------------------------------------------------------------------*/
    uc_i   = 0;
    _exit_ = 1;
    //++env->pc;
    while (1) {
        switch (*env->pc++ & POP_I) {
            case SP_SAVE  : ++uc_i;
                            break;
            case CALL_FCT :
            case CST_B    :
            case CST_STR  :
            case IT_AS    :
            case LOAD     :
            case STORE    :
            case STORE_LOC:
            case LOC      : env->pc++;
                            break;
            case CALL_OP  :
            case CST_LST  :
            case CST_S    :
            case CST_SET  :
            case JUMP     :
            case FJUMP    : env->pc += 2;
                            break;
            case CST_I    :
            case CST_F    : env->pc += 4;
                            break;
            case CST_DCT  : env->pc += *env->pc * 2 + 1;
                            break;
            case PROC     : //env->pc += *env->pc * 2;  // parameter count
                            //env->pc += *((ss_ushort*) env->pc)*; // bytecode length
                            //env->pc++;
                            break;
            case RET      :
            case RET_P    : if ( !uc_i )
                                goto GOTO__RET;
                            else
                                --uc_i;
                            break;
            case RET_L    : if ( !uc_i ) {
                                //dyn_list_push(&env->stack, &none);
                                pop = 0;
                                _exit_ = 0;
                                --env->pc;
                                goto GOTO__DISPACH;
                            }
        }
    }
/*---------------------------------------------------------------------------*/
GOTO__TRY:
/*---------------------------------------------------------------------------*/
    if (*env->pc++) {
        us_len = *((ss_int*) env->pc);
        env->pc += 2;

        dyn_set_int(&tmp, env->sp);
        dyn_list_push(&env->exceptions, &tmp);

        dyn_set_extern(&tmp, env->pc + us_len);
        dyn_list_push(&env->exceptions, &tmp);
    }
    else
        dyn_list_popi(&env->exceptions, 2);

    goto GOTO__DISPACH;
/*---------------------------------------------------------------------------*/
GOTO__REF:
/*---------------------------------------------------------------------------*/
    if (DYN_IS_REFERENCE(DYN_LIST_GET_END(&env->stack)))
        DYN_TYPE(DYN_LIST_GET_END(&env->stack)) = REFERENCE2;
    else
        env->status = VM_ERROR;
    goto GOTO__DISPACH;
/*---------------------------------------------------------------------------*/
GOTO__EXEC_PROC:
/*---------------------------------------------------------------------------*/
    dyn_list_push(&env->stack, &none);
    if (dyc_ptr2)
        dyn_set_extern(DYN_LIST_GET_END(&env->stack), dyc_ptr2);

    dyn_set_bool(&tmp, pop);
    dyn_list_push(&env->stack, &tmp);
    pop = 0;
    dyn_set_int(&tmp, env->dp);
    dyn_list_push(&env->stack, &tmp);
    //env->pc+=1;
    dyn_set_extern(&tmp, (void*)env->pc);
    dyn_list_push(&env->stack, &tmp);
    env->pc = dyn_fct_get_ss(dyc_ptr);

    env->dp = DYN_LIST_LEN(&env->data);

    us_len = (ss_ushort)*env->pc;
    env->new_scope = 0;

    // init data
    env->pc+=2;
    for (uc_i=0; uc_i<us_len; ++uc_i) {
        dyn_set_string(&tmp, env->pc);
        dyn_list_push(&env->data, &tmp);
        env->pc += ss_strlen(env->pc) + 1;
    }

    goto GOTO__DISPACH;
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
    for ( i=0; i<DYN_DICT_LENGTH(dyn->data.dict); ++i ) {
        len = dyn_length( DYN_DICT_GET_I_REF(dyn, i) );
        if (len > max)
            max = len;
    }
    return max;
}

ss_char vm_get_iterator (dyn_c* iter_rslt, dyn_c* iter_dict, ss_int count)
{
    ss_ushort us_len = dyn_length(iter_dict);

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
                    dyn_set_dict(&rslt_list->container[us_i], dyn_length(ptr));

                    dyn_c none;
                    DYN_INIT(&none);
                    for (i=0; i<dyn_length(ptr); ++i) {
                        dyn_dict_insert(&rslt_list->container[us_i],
                                         DYN_DICT_GET_I_KEY(ptr, i),
                                         &none);
                    }
                }

                for (i=0; i<dyn_length(ptr); ++i) {
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
    bytes += dyn_size(&env->local);
    bytes += dyn_size(&env->functions);
    bytes += dyn_size(&env->exceptions);
    bytes += dyn_size(&env->rslt);
    bytes += dyn_size(&env->data);
    return bytes;
}

void vm_reset (vm_env* env, ss_char hard)
{
    DYN_SET_LIST(&env->stack);
    DYN_SET_LIST(&env->data);
    DYN_SET_LIST(&env->exceptions);

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
                                            dyn_length(&env->functions)-1),
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
}


void vm_free (vm_env* env)
{
    dyn_free(&env->rslt);
    dyn_list_free(&env->stack);
    dyn_list_free(&env->local);
    dyn_list_free(&env->exceptions);
    dyn_dict_free(&env->memory);
    dyn_dict_free(&env->functions);
    dyn_list_free(&env->data);
    free(env);
    return;
}
