#include "vm_op.h"

ss_char op_NOT(dyn_c *rslt, dyn_c op[], ss_byte len)
{
    dyn_set_ref(rslt, op);
    return dyn_op_not(rslt);
};


ss_char op_NEG(dyn_c *rslt, dyn_c op[], ss_byte len)
{
    dyn_set_ref(rslt, op);
    return dyn_op_neg(rslt);
};


ss_char op_ADD(dyn_c *rslt, dyn_c op[], ss_byte len)
{
    ss_char status = VM_OK;
    if (DYN_TYPE(op) == MISCELLANEOUS) goto GOTO_REC;
    if (DYN_TYPE(rslt))                goto GOTO_NRM;

    dyn_set_ref(rslt, op);
    while (--len && status) {
        if (DYN_TYPE(++op) == MISCELLANEOUS)
GOTO_REC:   status = op_ADD(rslt, op->data.ref->data.list->container, DYN_LIST_LEN(op->data.ref));
        else
GOTO_NRM:   status = dyn_op_add(rslt, op);
    }
    return status;
};


ss_char op_SUB(dyn_c *rslt, dyn_c op[], ss_byte len)
{
    ss_char status = VM_OK;
    if (DYN_TYPE(op) == MISCELLANEOUS) goto GOTO_REC;
    if (DYN_TYPE(rslt))                goto GOTO_NRM;

    dyn_set_ref(rslt, op);
    while (--len && status) {
        if (DYN_TYPE(++op) == MISCELLANEOUS)
GOTO_REC:   status = op_SUB(rslt, op->data.ref->data.list->container, DYN_LIST_LEN(op->data.ref));
        else
GOTO_NRM:   status = dyn_op_sub(rslt, op);
    }
    return status;
};


ss_char op_MUL(dyn_c *rslt, dyn_c op[], ss_byte len)
{
    ss_char status = VM_OK;
    if (DYN_TYPE(op) == MISCELLANEOUS) goto GOTO_REC;
    if (DYN_TYPE(rslt))                goto GOTO_NRM;

    dyn_set_ref(rslt, op);
    while (--len && status) {
        if (DYN_TYPE(++op) == MISCELLANEOUS)
GOTO_REC:   status = op_MUL(rslt, op->data.ref->data.list->container, DYN_LIST_LEN(op->data.ref));
        else
GOTO_NRM:   status = dyn_op_mul(rslt, op);
    }
    return status;
};


ss_char op_DIV(dyn_c *rslt, dyn_c op[], ss_byte len)
{
    ss_char status = VM_OK;
    if (DYN_TYPE(op) == MISCELLANEOUS) goto GOTO_REC;
    if (DYN_TYPE(rslt))                goto GOTO_NRM;

    dyn_set_ref(rslt, op);
    while (--len && status) {
        if (DYN_TYPE(++op) == MISCELLANEOUS)
GOTO_REC:   status = op_DIV(rslt, op->data.ref->data.list->container, DYN_LIST_LEN(op->data.ref));
        else
GOTO_NRM:   status = dyn_op_div(rslt, op);
    }
    return status;
};


ss_char op_MOD(dyn_c *rslt, dyn_c op[], ss_byte len)
{
    ss_char status = VM_OK;
    if (DYN_TYPE(op) == MISCELLANEOUS) goto GOTO_REC;
    if (DYN_TYPE(rslt))                goto GOTO_NRM;

    dyn_set_ref(rslt, op);
    while (--len && status) {
        if (DYN_TYPE(++op) == MISCELLANEOUS)
GOTO_REC:   status = op_MOD(rslt, op->data.ref->data.list->container, DYN_LIST_LEN(op->data.ref));
        else
GOTO_NRM:   status = dyn_op_mod(rslt, op);
    }
    return status;
};


ss_char op_POW(dyn_c *rslt, dyn_c op[], ss_byte len)
{
    ss_char status = VM_OK;
    if (DYN_TYPE(op) == MISCELLANEOUS) goto GOTO_REC;
    if (DYN_TYPE(rslt))                goto GOTO_NRM;

    dyn_set_ref(rslt, op);
    while (--len && status) {
        if (DYN_TYPE(++op) == MISCELLANEOUS)
GOTO_REC:   status = op_POW(rslt, op->data.ref->data.list->container, DYN_LIST_LEN(op->data.ref));
        else
GOTO_NRM:   status = dyn_op_pow(rslt, op);
    }
    return status;
};


ss_char op_LT(dyn_c *rslt, dyn_c op[], ss_byte len)
{
    ss_char status = VM_OK;
    if (DYN_TYPE(op) == MISCELLANEOUS) goto GOTO_REC;
    if (DYN_TYPE(rslt))                goto GOTO_NRM;

    while (--len && status) {
        dyn_set_ref(rslt, DYN_TYPE(op) == MISCELLANEOUS ? DYN_LIST_GET_END(op->data.ref) : op);
        if (DYN_TYPE(++op) == MISCELLANEOUS)
GOTO_REC:   status = op_LT(rslt, op->data.ref->data.list->container, DYN_LIST_LEN(op->data.ref));
        else
GOTO_NRM:   status = dyn_op_lt(rslt, op);

        if (rslt->data.b != DYN_TRUE)
            break;
    }
    return status;
};


ss_char op_LE(dyn_c *rslt, dyn_c op[], ss_byte len)
{
    ss_char status = VM_OK;
    if (DYN_TYPE(op) == MISCELLANEOUS) goto GOTO_REC;
    if (DYN_TYPE(rslt))                goto GOTO_NRM;

    while (--len && status) {
        dyn_set_ref(rslt, DYN_TYPE(op) == MISCELLANEOUS ? DYN_LIST_GET_END(op->data.ref) : op);

        if (DYN_TYPE(++op) == MISCELLANEOUS)
GOTO_REC:   status = op_LE(rslt, op->data.ref->data.list->container, DYN_LIST_LEN(op->data.ref));
        else
GOTO_NRM:   status = dyn_op_le(rslt, op);

        if (rslt->data.b != DYN_TRUE)
            break;
    }
    return status;
 };


ss_char op_GT(dyn_c *rslt, dyn_c op[], ss_byte len)
{
    ss_char status = VM_OK;
    if (DYN_TYPE(op) == MISCELLANEOUS) goto GOTO_REC;
    if (DYN_TYPE(rslt))                goto GOTO_NRM;

    while (--len && status) {
        dyn_set_ref(rslt, DYN_TYPE(op) == MISCELLANEOUS ? DYN_LIST_GET_END(op->data.ref) : op);

        if (DYN_TYPE(++op) == MISCELLANEOUS)
GOTO_REC:   status = op_GT(rslt, op->data.ref->data.list->container, DYN_LIST_LEN(op->data.ref));
        else
GOTO_NRM:   status = dyn_op_gt(rslt, op);

        if (rslt->data.b != DYN_TRUE)
            break;
    }
    return status;
};


ss_char op_GE(dyn_c *rslt, dyn_c op[], ss_byte len)
{
    ss_char status = VM_OK;
    if (DYN_TYPE(op) == MISCELLANEOUS) goto GOTO_REC;
    if (DYN_TYPE(rslt))                goto GOTO_NRM;

    while (--len && status) {
        dyn_set_ref(rslt, DYN_TYPE(op) == MISCELLANEOUS ? DYN_LIST_GET_END(op->data.ref) : op);

        if (DYN_TYPE(++op) == MISCELLANEOUS)
GOTO_REC:   status = op_GE(rslt, op->data.ref->data.list->container, DYN_LIST_LEN(op->data.ref));
        else
GOTO_NRM:   status = dyn_op_ge(rslt, op);

        if (rslt->data.b != DYN_TRUE)
            break;
    }
    return status;
};

/*
ss_char op_NE(dyn_c *rslt, dyn_c op[], ss_byte len)
{
    ss_char status = VM_OK;
    ss_short i;

    dyn_c *first;


    while (len && status) {
        first = op;
        if (DYN_TYPE(first) == MISCELLANEOUS) {
            status = op_NE( rslt,
                            first->data.ref->data.list->container,
                            DYN_LIST_LEN(first->data.ref));
            if (!rslt->data.b)
                break;

            for (i=0; i<DYN_LIST_LEN(first->data.ref) && status; ++i) {
                dyn_set_ref(rslt, DYN_LIST_GET_REF(first->data.ref, i));
                for (j=0; j<=len && status; ++j) {
                    status = dyn_op_ne()
                }
            }
        }
    }
    while (--len && status) {
        first = op;
        for(i=0; i<=len && status; ++i) {

            dyn_set_ref(rslt, first);
            if (DYN_TYPE(++op) == MISCELLANEOUS)
GOTO_REC:       status = op_NE(rslt, op->data.ref->data.list->container,
                                     DYN_LIST_LEN(op->data.ref));
            else
GOTO_NRM:       status = dyn_op_ne(rslt, op);

            if (!rslt->data.b)
                return status;
        }
        op = first+1;
    }

    return status;
};
*/
/*
ss_char op_NE(dyn_c *rslt, dyn_c op[], ss_byte len)
{
    ss_char status = VM_OK;
    ss_short i;

    dyn_c *first;

    while (--len && status) {
        first = op;
        if (DYN_TYPE(first) == MISCELLANEOUS) {
            status = op_NE( rslt,
                            first->data.ref->data.list->container,
                            DYN_LIST_LEN(first->data.ref));

            if (!rslt->data.b)
                return status;

            for (j=0; j<DYN_LIST_LEN(first->data.ref); ++j) {
                for(i=)
            }
        }
        for(i=0; i<=len && status; ++i) {

            dyn_set_ref(rslt, first);
            if (DYN_TYPE(++op) == MISCELLANEOUS)
GOTO_REC:       status = op_NE(rslt, op->data.ref->data.list->container,
                                     DYN_LIST_LEN(op->data.ref));
            else
                status = dyn_op_ne(rslt, op);

            if (!rslt->data.b)
                return status;
        }
        op = first+1;
    }

    return status;
};
*/

ss_char op_NE(dyn_c *rslt, dyn_c op[], ss_byte len)
{
    ss_char status = VM_OK;
    ss_short i;

    dyn_c *first = op;

    if (DYN_TYPE(op) == MISCELLANEOUS) goto GOTO_REC;
    if (DYN_NOT_NONE(rslt)) {
        first = rslt;
        goto GOTO_NRM;
    }

    while (--len && status) {
        first = DYN_TYPE(op) == MISCELLANEOUS ? DYN_LIST_GET_END(op->data.ref) : op;
        for(i=0; i<=len && status; ++i) {

            dyn_set_ref(rslt, first);
            if (DYN_TYPE(++op) == MISCELLANEOUS)
GOTO_REC:       status = op_NE(rslt, op->data.ref->data.list->container,
                                     DYN_LIST_LEN(op->data.ref));
            else
GOTO_NRM:       status = dyn_op_ne(rslt, op);

            if (!rslt->data.b)
                return status;
        }
        op = first+1;
    }

    return status;
};


/*
ss_char op_NE(dyn_c *rslt, dyn_c op[], ss_byte len)
{
    ss_char status = VM_OK;
    ss_short i;

    dyn_c *first = op;

    if (DYN_TYPE(op) == MISCELLANEOUS) goto GOTO_REC;
    if (DYN_NOT_NONE(rslt)) {
        first = rslt;
        goto GOTO_NRM;
    }

    while (--len && status) {
        first = DYN_TYPE(op) == MISCELLANEOUS ? DYN_LIST_GET_END(op->data.ref) : op;
        for(i=0; i<len && status; ++i) {

            dyn_set_ref(rslt, first);
            if (DYN_TYPE(++op) == MISCELLANEOUS)
GOTO_REC:       status = op_NE(rslt, op->data.ref->data.list->container,
                                     DYN_LIST_LEN(op->data.ref));
            else
GOTO_NRM:       status = dyn_op_ne(rslt, op);

            if (!rslt->data.b)
                return status;
        }
        op = first+1;
    }

    return status;
};
*/

ss_char op_EQ(dyn_c *rslt, dyn_c op[], ss_byte len)
{
    ss_char status = VM_OK;
    if (DYN_TYPE(op) == MISCELLANEOUS) goto GOTO_REC;
    if (DYN_NOT_NONE(rslt))            goto GOTO_NRM;

    while (--len && status) {
        dyn_set_ref(rslt, DYN_TYPE(op) == MISCELLANEOUS ? DYN_LIST_GET_END(op->data.ref) : op);

        if (DYN_TYPE(++op) == MISCELLANEOUS)
GOTO_REC:   status = op_EQ(rslt, op->data.ref->data.list->container, DYN_LIST_LEN(op->data.ref));
        else
GOTO_NRM:   status = dyn_op_eq(rslt, op);

        if (rslt->data.b != DYN_TRUE)
              break;
    }
    return status;
};


ss_char op_AND(dyn_c *rslt, dyn_c op[], ss_byte len)
{
    ss_char status = VM_OK;
    if (DYN_TYPE(op) == MISCELLANEOUS) goto GOTO_REC;
    if (DYN_TYPE(rslt))                goto GOTO_NRM;

    dyn_set_ref(rslt, op);
    while (--len && status) {
        if (DYN_TYPE(++op) == MISCELLANEOUS)
GOTO_REC:   status = op_AND(rslt, op->data.ref->data.list->container, DYN_LIST_LEN(op->data.ref));
        else
GOTO_NRM:   status = dyn_op_and(rslt, op);

        if (rslt->data.b != DYN_TRUE)
            break;
    }
    return status;
};


ss_char op_OR (dyn_c *rslt, dyn_c op[], ss_byte len)
{
    ss_char status = VM_OK;
    if (DYN_TYPE(op) == MISCELLANEOUS) goto GOTO_REC;
    if (DYN_TYPE(rslt))                goto GOTO_NRM;

    dyn_set_ref(rslt, op);
    while (--len && status) {
        if (DYN_TYPE(++op) == MISCELLANEOUS)
GOTO_REC:   status = op_OR(rslt, op->data.ref->data.list->container, DYN_LIST_LEN(op->data.ref));
        else
GOTO_NRM:   status = dyn_op_or(rslt, op);

        if (rslt->data.b == DYN_TRUE)
            break;
    }
    return status;
};

ss_char op_XOR(dyn_c *rslt, dyn_c op[], ss_byte len)
{
    ss_char status = VM_OK;
    if (DYN_TYPE(op) == MISCELLANEOUS) goto GOTO_REC;
    if (DYN_NOT_NONE(rslt))            goto GOTO_NRM;

    dyn_set_ref(rslt, op);
    while (--len && status) {
        if (DYN_TYPE(++op) == MISCELLANEOUS)
GOTO_REC:   status = op_XOR(rslt, op->data.ref->data.list->container, DYN_LIST_LEN(op->data.ref));
        else
GOTO_NRM:   status = dyn_op_xor(rslt, op);

        if (dyn_get_bool_3(rslt) == DYN_NONE)
            break;
    }
    return status;
};

ss_char op_IN (dyn_c *rslt, dyn_c op[], ss_byte len)
{
    ss_char status = VM_OK;

    dyn_c *first = DYN_TYPE(op) == MISCELLANEOUS ? DYN_LIST_GET_REF(op->data.ref, 0) : op;

    if (DYN_TYPE(op) == MISCELLANEOUS) goto GOTO_REC;
    if (DYN_NOT_NONE(rslt)) {
        first = rslt;
        goto GOTO_NRM;
    }

    while (--len && status) {
        dyn_set_ref(rslt, first);

        if (DYN_TYPE(++op) == MISCELLANEOUS)
GOTO_REC:   status = op_IN(rslt, op->data.ref->data.list->container, DYN_LIST_LEN(op->data.ref));
        else
GOTO_NRM:   status = dyn_op_in(rslt, op);

        if (rslt->data.b != DYN_TRUE)
            break;
    }
    return status;
};

ss_char op_B_NOT(dyn_c *rslt, dyn_c op[], ss_byte len)
{
    dyn_set_ref(rslt, op);

    return dyn_op_b_not(rslt);
};

ss_char op_B_AND(dyn_c *rslt, dyn_c op[], ss_byte len)
{
    ss_char status = VM_OK;
    if (DYN_TYPE(op) == MISCELLANEOUS) goto GOTO_REC;
    if (DYN_TYPE(rslt))                goto GOTO_NRM;

    dyn_set_ref(rslt, op);
    while (--len && status) {
        if (DYN_TYPE(++op) == MISCELLANEOUS)
GOTO_REC:   status = op_B_AND(rslt, op->data.ref->data.list->container, DYN_LIST_LEN(op->data.ref));
        else
GOTO_NRM:   status = dyn_op_b_and(rslt, op);
    }
    return status;
};


ss_char op_B_OR (dyn_c *rslt, dyn_c op[], ss_byte len)
{
    ss_char status = VM_OK;
    if (DYN_TYPE(op) == MISCELLANEOUS) goto GOTO_REC;
    if (DYN_TYPE(rslt))                goto GOTO_NRM;

    dyn_set_ref(rslt, op);
    while (--len && status) {
        if (DYN_TYPE(++op) == MISCELLANEOUS)
GOTO_REC:   status = op_B_OR(rslt, op->data.ref->data.list->container, DYN_LIST_LEN(op->data.ref));
        else
GOTO_NRM:   status = dyn_op_b_or(rslt, op);
    }
    return status;
};


ss_char op_B_XOR (dyn_c *rslt, dyn_c op[], ss_byte len)
{
    ss_char status = VM_OK;
    if (DYN_TYPE(op) == MISCELLANEOUS) goto GOTO_REC;
    if (DYN_TYPE(rslt))                goto GOTO_NRM;

    dyn_set_ref(rslt, op);
    while (--len && status) {
        if (DYN_TYPE(++op) == MISCELLANEOUS)
GOTO_REC:   status = op_B_XOR(rslt, op->data.ref->data.list->container, DYN_LIST_LEN(op->data.ref));
        else
GOTO_NRM:   status = dyn_op_b_xor(rslt, op);
    }
    return status;
};

ss_char op_B_SHIFT_L (dyn_c *rslt, dyn_c op[], ss_byte len)
{
    ss_char status = VM_OK;
    if (DYN_TYPE(op) == MISCELLANEOUS) goto GOTO_REC;
    if (DYN_TYPE(rslt))                goto GOTO_NRM;

    dyn_set_ref(rslt, op);
    while (--len && status) {
        if (DYN_TYPE(++op) == MISCELLANEOUS)
GOTO_REC:   status = op_B_SHIFT_L(rslt, op->data.ref->data.list->container, DYN_LIST_LEN(op->data.ref));
        else
GOTO_NRM:   status = dyn_op_b_shift_l(rslt, op);
    }
    return status;
};


ss_char op_B_SHIFT_R (dyn_c *rslt, dyn_c op[], ss_byte len)
{
    ss_char status = VM_OK;
    if (DYN_TYPE(op) == MISCELLANEOUS) goto GOTO_REC;
    if (DYN_TYPE(rslt))                goto GOTO_NRM;

    dyn_set_ref(rslt, op);
    while (--len && status) {
        if (DYN_TYPE(++op) == MISCELLANEOUS)
GOTO_REC:   status = op_B_SHIFT_R(rslt, op->data.ref->data.list->container, DYN_LIST_LEN(op->data.ref));
        else
GOTO_NRM:   status = dyn_op_b_shift_r(rslt, op);
    }
    return status;
};


ss_char op_EX (dyn_c *rslt, dyn_c op[], ss_byte len)
{
    if (DYN_IS_REFERENCE(op)) {
        if (DYN_TYPE(op->data.ref) == LIST) {
            dyn_set_ref(rslt,op);
            rslt->type = MISCELLANEOUS;
            return VM_OK;
        }
    }
    return VM_ERROR;
}


ss_char vm_sys_help (vm_env* env, dyn_c* rslt, dyn_c params [], ss_byte len)
{
    ss_ushort i;
    dyn_c none;
    DYN_INIT(&none);

    dyn_c *ptr[2] = {&env->functions, &env->memory};

    // function defined
    if (len == 0) {
        dyn_set_list_len(rslt, DYN_DICT_LENGTH(env->functions.data.dict));

        for (; len<2; ++len) {
            for (i=0; i<DYN_DICT_LENGTH(ptr[len]->data.dict); ++i) {
                if (DYN_TYPE(DYN_DICT_GET_I_REF(ptr[len], i)) == FUNCTION) {
                    dyn_list_push(rslt, &none);
                    dyn_set_string(DYN_LIST_GET_END(rslt),
                                    DYN_DICT_GET_I_KEY(ptr[len], i));
                }
            }
        }
    }
    else if (len == 1) {
        dyn_c *fct;
        ss_str fct_name = NULL;

        if (DYN_TYPE(params->data.ref) == STRING) {
            fct_name = dyn_get_string(&params[0]);

            for (len=0; len<2; ++len) {
                i = dyn_dict_has_key(ptr[len], fct_name);
                if (i) {
                    fct = DYN_DICT_GET_I_REF(ptr[len], i-1);
GOTO_FCT:
                    i = 1;
                    if (DYN_TYPE(fct) == FUNCTION) {
                        switch (fct->data.fct->type) {
                            case 0: dyn_set_string(rslt, "procedure");
                                    break;
                            case 1: dyn_set_string(rslt, "function");
                                    break;
                            case 2: dyn_set_string(rslt, "sytem-function");
                        }
                        if (fct->data.fct->info != NULL) {
                            ss_strcat2(rslt->data.str, (ss_str)"\n");
                            ss_strcat2(rslt->data.str, fct->data.fct->info);
                        }
                    }
                    break;
                }
            }

            if (fct_name)
                free(fct_name);

            if (!i)
                dyn_set_string(rslt, "unknown");
        }
        else {
            fct = DYN_IS_REFERENCE(params) ? params->data.ref : params;
            goto GOTO_FCT;
        }
    }
    else
    {
        dyn_set_list_len(rslt, len);

        for(i=0; i<len; ++i) {
            dyn_list_push(rslt, &none);

            vm_sys_help (env,
                         DYN_LIST_GET_REF(rslt, i),
                         &params[i],
                         1);
        }
    }

    return VM_OK;
}

ss_char vm_sys_print (vm_env* env, dyn_c* rslt, dyn_c params [], ss_byte len)
{
    // store last element
    if (len)
        dyn_copy(&params[len-1], rslt);
    else {
        vm_printf((ss_str)"", 1);
        return VM_OK;
    }

    ss_str str = NULL;
    ss_byte i;
    for (i=0; i<len; ++i) {
        str = dyn_get_string(&params[i]);
        vm_printf(str, 0);
        free(str);
        vm_printf((ss_str)" ", 0);
    }
    vm_printf((ss_str)"", 1);
    return VM_OK;
}

ss_char vm_sys_mem (vm_env* env, dyn_c* rslt, dyn_c params [], ss_byte len)
{
    if (len) {
        ss_str id = dyn_get_string(&params[0]);

        if (dyn_dict_has_key(&env->memory, id)) {
            dyn_set_bool(rslt, 1);
        } else {
            dyn_set_bool(rslt, 0);
        }
        free(id);

    } else {
        dyn_set_list_len(rslt, dyn_length(&env->memory));

        dyn_c none;
        DYN_INIT(&none);

        ss_ushort len = DYN_DICT_SPACE(env->memory.data.dict);
        ss_ushort i;
        for (i=0; i<len; ++i) {
            if(DYN_DICT_GET_I_KEY(&env->memory, i)) {
                dyn_list_push(rslt, &none);
                dyn_set_string(dyn_list_get_ref(rslt, -1),
                               DYN_DICT_GET_I_KEY(&env->memory, i));
            }
        }
    }

    return VM_OK;
}

ss_char vm_sys_del (vm_env* env, dyn_c* rslt, dyn_c params [], ss_byte len)
{
    if (len) {
        ss_char status = VM_OK;
        ss_byte i;


        ss_str str = dyn_get_string(&params[len-1]);

        i = dyn_dict_has_key(&env->memory, str);
        free(str);

        if (i)
            dyn_move(DYN_DICT_GET_I_REF(&env->memory, i-1), rslt);
        else
            return VM_ERROR;

        for (i=0; i<len && status; ++i) {
            str = dyn_get_string(&params[i]);
            status = dyn_dict_remove(&env->memory, str);
            free(str);
        }

        return status;
    }

    dyn_free(rslt);
    dyn_dict_empty(&env->memory);
    return VM_OK;
}

char ss_size_fct   (dyn_c* rslt, dyn_c params[1], ss_byte len)  { dyn_set_int  (rslt, dyn_size(&params[0]));      return 1; }
char ss_float_fct  (dyn_c* rslt, dyn_c params[1], ss_byte len)  { dyn_set_float(rslt, dyn_get_float(&params[0])); return 1; }
char ss_int_fct    (dyn_c* rslt, dyn_c params[1], ss_byte len)  { dyn_set_int  (rslt, dyn_get_int(&params[0]));   return 1; }
char ss_type_fct   (dyn_c* rslt, dyn_c params[1], ss_byte len)  { dyn_set_int  (rslt, dyn_type(&params[0]));      return 1; }
char ss_len_fct    (dyn_c* rslt, dyn_c params[1], ss_byte len)  { dyn_set_int  (rslt, dyn_length(&params[0]));    return 1; }
char ss_time_fct   (dyn_c* rslt, dyn_c params[1], ss_byte len)  { dyn_set_int  (rslt, 0);                         return 1; }
char ss_str_fct    (dyn_c* rslt, dyn_c params[1], ss_byte len)  {

    ss_str str = dyn_get_string(&params[0]);
    dyn_set_string(rslt, str);
    free(str);
    return VM_OK;
}

#define VM_TYPE(X)   (DYN_IS_REFERENCE(X) ? DYN_TYPE((X)->data.ref) : DYN_TYPE(X))

char ss_is_none_fct  (dyn_c* rslt, dyn_c params[1], ss_byte len)  { dyn_set_bool (rslt, VM_TYPE(&params[0]) == NONE);      return 1; }
char ss_is_bool_fct  (dyn_c* rslt, dyn_c params[1], ss_byte len)  { dyn_set_bool (rslt, VM_TYPE(&params[0]) == BOOL);      return 1; }
char ss_is_int_fct   (dyn_c* rslt, dyn_c params[1], ss_byte len)  { dyn_set_bool (rslt, VM_TYPE(&params[0]) == INTEGER);   return 1; }
char ss_is_float_fct (dyn_c* rslt, dyn_c params[1], ss_byte len)  { dyn_set_bool (rslt, VM_TYPE(&params[0]) == FLOAT);     return 1; }
char ss_is_str_fct   (dyn_c* rslt, dyn_c params[1], ss_byte len)  { dyn_set_bool (rslt, VM_TYPE(&params[0]) == STRING);    return 1; }
char ss_is_list_fct  (dyn_c* rslt, dyn_c params[1], ss_byte len)  { dyn_set_bool (rslt, VM_TYPE(&params[0]) == LIST);      return 1; }
char ss_is_dict_fct  (dyn_c* rslt, dyn_c params[1], ss_byte len)  { dyn_set_bool (rslt, VM_TYPE(&params[0]) == DICT);      return 1; }
char ss_is_proc_fct  (dyn_c* rslt, dyn_c params[1], ss_byte len)  { dyn_set_bool (rslt, VM_TYPE(&params[0]) == FUNCTION);  return 1; }
char ss_is_ex_fct    (dyn_c* rslt, dyn_c params[1], ss_byte len)  { dyn_set_bool (rslt, VM_TYPE(&params[0]) == EXTERN);    return 1; }

char ss_insert       (dyn_c* rslt, dyn_c params[3], ss_byte len)  {
    if (len == 3)
    {
        dyn_move(&params[0], rslt);
        dyn_list_insert(rslt, &params[1], dyn_get_int(&params[2]));

        return VM_OK;
    }
    return VM_ERROR;
}

char ss_remove       (dyn_c* rslt, dyn_c params[2], ss_byte len)  {
    if (len == 2)
    {
        dyn_move(&params[0], rslt);
        dyn_list_remove(rslt, dyn_get_int(&params[1]));

        return VM_OK;
    }
    return VM_ERROR;
}
