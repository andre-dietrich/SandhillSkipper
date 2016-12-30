#include "vm_op.h"

typedef trilean (*unary)  (dyn_c*);
typedef trilean (*binary) (dyn_c*, dyn_c*);

trilean op_unary( dyn_c *rslt, dyn_c op[], unary fct )
{
    dyn_set_ref(rslt, op);
    return (*fct)(rslt);
};

trilean op_binary(dyn_c *rslt, dyn_c op[], dyn_byte len, binary fct, dyn_byte shortcut)
{
    trilean status = DYN_TRUE;
    if (DYN_TYPE(op) == MISCELLANEOUS) goto GOTO_REC;
    if (DYN_TYPE(rslt))                goto GOTO_NRM;

    dyn_set_ref(rslt, op);
    while (--len && status) {
        if (DYN_TYPE(++op) == MISCELLANEOUS)
GOTO_REC:   status = op_binary(rslt, op->data.ref->data.list->container, DYN_LIST_LEN(op->data.ref), fct, shortcut);
        else
GOTO_NRM:   status = (*fct)(rslt, op);

        if (shortcut) {
            switch (shortcut) {
                case 1: if (rslt->data.b != DYN_TRUE) len = 1;  // AND
                        break;
                case 2: if (rslt->data.b == DYN_TRUE) len = 1;  // OR
                        break;
                case 3: if (dyn_get_bool_3(rslt) == DYN_NONE) len = 1; // XOR
                        break;
            }
        }
    }
    return status;
}

trilean op_binary2(dyn_c *rslt, dyn_c op[], dyn_byte len, binary fct)
{
    trilean status = DYN_TRUE;
    if (DYN_TYPE(op) == MISCELLANEOUS) goto GOTO_REC;
    if (DYN_TYPE(rslt))                goto GOTO_NRM;

    while (--len && status) {
        dyn_set_ref(rslt, DYN_TYPE(op) == MISCELLANEOUS ? DYN_LIST_GET_END(op->data.ref) : op);
        if (DYN_TYPE(++op) == MISCELLANEOUS)
GOTO_REC:   status = op_binary2(rslt, op->data.ref->data.list->container, DYN_LIST_LEN(op->data.ref), fct);
        else
GOTO_NRM:   status = (*fct)(rslt, op);

        if (rslt->data.b != DYN_TRUE)
            break;
    }
    return status;
};


trilean op_IN (dyn_c *rslt, dyn_c op[], dyn_byte len)
{
    trilean status = DYN_TRUE;

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


/*
trilean op_NE(dyn_c *rslt, dyn_c op[], dyn_byte len)
{
    trilean status = DYN_TRUE;
    dyn_short i = -1;
    dyn_short j = 0;

    dyn_c *first = op;

    if (DYN_TYPE(op) == MISCELLANEOUS) goto GOTO_REC;
    if (DYN_NOT_NONE(rslt)) {
        first = rslt;
        goto GOTO_NRM;
    }

    while (++i < len && status) {
        first = DYN_TYPE(op) == MISCELLANEOUS ? DYN_LIST_GET_END(op->data.ref) : op;

        for(j=i; j<=len && status; ++j) {

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


trilean op_NE(dyn_c *rslt, dyn_c op[], dyn_byte len)
{
    trilean status = DYN_TRUE;
    dyn_short i;

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


trilean vm_op_dispatch (dyn_c *rslt, dyn_c op[], dyn_byte len, dyn_byte op_id)
{
    switch (op_id) {
        case ADD:   return op_binary(rslt, op, len, &dyn_op_add, 0);
        case SUB:   return op_binary(rslt, op, len, &dyn_op_sub, 0);
        case MUL:   return op_binary(rslt, op, len, &dyn_op_mul, 0);
        case DIV:   return op_binary(rslt, op, len, &dyn_op_div, 0);
        case MOD:   return op_binary(rslt, op, len, &dyn_op_mod, 0);
        case POW:   return op_binary(rslt, op, len, &dyn_op_pow, 0);
        case B_AND: return op_binary(rslt, op, len, &dyn_op_b_and, 0);
        case B_OR:  return op_binary(rslt, op, len, &dyn_op_b_or,  0);
        case B_XOR: return op_binary(rslt, op, len, &dyn_op_b_xor, 0);
        case LEFT:  return op_binary(rslt, op, len, &dyn_op_b_shift_l, 0);
        case RIGHT: return op_binary(rslt, op, len, &dyn_op_b_shift_r, 0);

        case AND:   return op_binary(rslt, op, len, &dyn_op_and, 1);
        case OR:    return op_binary(rslt, op, len, &dyn_op_or,  2);
        case XOR:   return op_binary(rslt, op, len, &dyn_op_xor, 3);

        case LE:    return op_binary2(rslt, op, len, &dyn_op_le);
        case LT:    return op_binary2(rslt, op, len, &dyn_op_lt);
        case GT:    return op_binary2(rslt, op, len, &dyn_op_gt);
        case GE:    return op_binary2(rslt, op, len, &dyn_op_ge);
        case EQ:    return op_binary2(rslt, op, len, &dyn_op_eq);

        case NE:    return op_NE(rslt, op, len);
        case IN:    return op_IN(rslt, op, len);

        case NOT:   return op_unary (rslt, op, &dyn_op_not);
        case NEG:   return op_unary (rslt, op, &dyn_op_neg);
        case B_NOT: return op_unary (rslt, op, &dyn_op_b_not);

        case EX: {
            if (DYN_IS_REFERENCE(op)) {
                if (DYN_TYPE(op->data.ref) == LIST) {
                    dyn_set_ref(rslt,op);
                    rslt->type = MISCELLANEOUS;
                    return DYN_TRUE;
                }
            }
            return DYN_FALSE;
        }
    }

    dyn_free(rslt);
    return DYN_FALSE;
};


trilean vm_sys_help (vm_env* env, dyn_c* rslt, dyn_c params [], dyn_byte len)
{
    dyn_ushort i;
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
    else {
        dyn_c *fct;
        dyn_str fct_name = NULL;

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
                            dyn_strcat2(rslt->data.str, (dyn_str)"\n");
                            dyn_strcat2(rslt->data.str, fct->data.fct->info);
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
/*    else
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
*/
    return DYN_TRUE;
}

trilean vm_sys_print (vm_env* env, dyn_c* rslt, dyn_c params [], dyn_byte len)
{
    // store last element
    if (len)
        dyn_copy(&params[len-1], rslt);
    else {
        dyn_free(rslt);
        goto LABEL_OK;
    }
    {
        dyn_str str = NULL;
        dyn_byte i;
        for (i=0; i<len; ++i) {
            str = dyn_get_string(&params[i]);
            vm_printf(str, 0);
            free(str);
            vm_printf((dyn_str)" ", 0);
        }
    }
LABEL_OK:
    vm_printf((dyn_str)"", 1);
    return DYN_TRUE;
}

trilean vm_sys_mem (vm_env* env, dyn_c* rslt, dyn_c params [], dyn_byte len)
{
    if (len) {
        dyn_str id = dyn_get_string(&params[0]);

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

        dyn_ushort len = DYN_DICT_SPACE(env->memory.data.dict);
        dyn_ushort i;
        for (i=0; i<len; ++i) {
            if(DYN_DICT_GET_I_KEY(&env->memory, i)) {
                dyn_list_push(rslt, &none);
                dyn_set_string(dyn_list_get_ref(rslt, -1),
                               DYN_DICT_GET_I_KEY(&env->memory, i));
            }
        }
    }

    return DYN_TRUE;
}

trilean vm_sys_del (vm_env* env, dyn_c* rslt, dyn_c params [], dyn_byte len)
{
    if (len) {
        trilean status = DYN_TRUE;
        dyn_byte i;


        dyn_str str = dyn_get_string(&params[len-1]);

        i = dyn_dict_has_key(&env->memory, str);
        free(str);

        if (i)
            dyn_move(DYN_DICT_GET_I_REF(&env->memory, i-1), rslt);
        else
            return DYN_FALSE;

        for (i=0; i<len && status; ++i) {
            str = dyn_get_string(&params[i]);
            status = dyn_dict_remove(&env->memory, str);
            free(str);
        }

        return status;
    }

    dyn_free(rslt);
    dyn_dict_empty(&env->memory);
    return DYN_TRUE;
}

trilean fct_size   (dyn_c* rslt, dyn_c params[1], dyn_byte len)  { dyn_set_int  (rslt, dyn_size(&params[0]));      return 1; }
trilean fct_float  (dyn_c* rslt, dyn_c params[1], dyn_byte len)  { dyn_set_float(rslt, dyn_get_float(&params[0])); return 1; }
trilean fct_int    (dyn_c* rslt, dyn_c params[1], dyn_byte len)  { dyn_set_int  (rslt, dyn_get_int(&params[0]));   return 1; }
trilean fct_type   (dyn_c* rslt, dyn_c params[1], dyn_byte len)  { dyn_set_int  (rslt, dyn_type(&params[0]));      return 1; }
trilean fct_len    (dyn_c* rslt, dyn_c params[1], dyn_byte len)  { dyn_set_int  (rslt, dyn_length(&params[0]));    return 1; }
trilean fct_str    (dyn_c* rslt, dyn_c params[1], dyn_byte len)  {

    dyn_str str = dyn_get_string(&params[0]);
    dyn_set_string(rslt, str);
    free(str);
    return DYN_TRUE;
}

#define VM_TYPE(X)   (DYN_IS_REFERENCE(X) ? DYN_TYPE((X)->data.ref) : DYN_TYPE(X))

trilean fct_is_none  (dyn_c* rslt, dyn_c params[1], dyn_byte len)  { dyn_set_bool (rslt, VM_TYPE(&params[0]) == NONE);      return 1; }
trilean fct_is_bool  (dyn_c* rslt, dyn_c params[1], dyn_byte len)  { dyn_set_bool (rslt, VM_TYPE(&params[0]) == BOOL);      return 1; }
trilean fct_is_int   (dyn_c* rslt, dyn_c params[1], dyn_byte len)  { dyn_set_bool (rslt, VM_TYPE(&params[0]) == INTEGER);   return 1; }
trilean fct_is_float (dyn_c* rslt, dyn_c params[1], dyn_byte len)  { dyn_set_bool (rslt, VM_TYPE(&params[0]) == FLOAT);     return 1; }
trilean fct_is_str   (dyn_c* rslt, dyn_c params[1], dyn_byte len)  { dyn_set_bool (rslt, VM_TYPE(&params[0]) == STRING);    return 1; }
trilean fct_is_list  (dyn_c* rslt, dyn_c params[1], dyn_byte len)  { dyn_set_bool (rslt, VM_TYPE(&params[0]) == LIST);      return 1; }
trilean fct_is_dict  (dyn_c* rslt, dyn_c params[1], dyn_byte len)  { dyn_set_bool (rslt, VM_TYPE(&params[0]) == DICT);      return 1; }
trilean fct_is_proc  (dyn_c* rslt, dyn_c params[1], dyn_byte len)  { dyn_set_bool (rslt, VM_TYPE(&params[0]) == FUNCTION);  return 1; }
trilean fct_is_ex    (dyn_c* rslt, dyn_c params[1], dyn_byte len)  { dyn_set_bool (rslt, VM_TYPE(&params[0]) == EXTERN);    return 1; }

trilean fct_insert       (dyn_c* rslt, dyn_c params[3], dyn_byte len)  {
    if (len == 3)
    {
        dyn_move(&params[0], rslt);
        dyn_list_insert(rslt, &params[1], dyn_get_int(&params[2]));

        return DYN_TRUE;
    }
    return DYN_FALSE;
}

trilean fct_remove       (dyn_c* rslt, dyn_c params[2], dyn_byte len)  {
    if (len == 2)
    {
        dyn_move(&params[0], rslt);
        dyn_list_remove(rslt, dyn_get_int(&params[1]));

        return DYN_TRUE;
    }
    return DYN_FALSE;
}

trilean fct_pop       (dyn_c* rslt, dyn_c params[2], dyn_byte len)  {
    if (len)
    {
        dyn_copy(params, rslt);
        dyn_list_popi(rslt,1);

        return DYN_TRUE;
    }
    return DYN_FALSE;
}

#ifdef ARDUNINO
    #include <Arduino.h>
#else
    #include <time.h>
#endif

trilean fct_time (dyn_c* rslt, dyn_c params[1], dyn_byte len)  {

#ifdef ARDUNINO
    dyn_set_float(rslt, (float) millis());
#else
    dyn_set_float(rslt, ((float) clock()) / CLOCKS_PER_SEC);
#endif

    return 1;
}

dyn_uint hash(char *val, dyn_ushort init) {
    dyn_uint has = 5381;
    while(*val)
        has = 33 * has + *val++;

    return has+init;
}

trilean fct_hash (dyn_c* rslt, dyn_c params[2], dyn_byte len)  {

    if (len) {
        dyn_str  val = dyn_get_string(params);
        dyn_uint has = hash(val, DYN_TYPE(params));
        free(val);

        if(len == 2) {
            has = has % (dyn_uint) dyn_get_int(&params[1]);
        }
        dyn_set_int(rslt, has);
        return DYN_TRUE;
    }

    return DYN_FALSE;
}
