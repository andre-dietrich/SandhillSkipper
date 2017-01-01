#ifndef VM_TRACE_C_H
#define VM_TRACE_C_H

#ifdef S2_DEBUG

static const dyn_str opcodes[] = {
    (dyn_str)"SP_SAVEX",
    (dyn_str)"SP_SAVE",

    (dyn_str)"RET",
    (dyn_str)"RET_P",

    (dyn_str)"CST_N",
    (dyn_str)"CST_0",
    (dyn_str)"CST_1",
    (dyn_str)"CST_B",
    (dyn_str)"CST_S",
    (dyn_str)"CST_I",
    (dyn_str)"CST_F",

    (dyn_str)"CST_STR",
    (dyn_str)"CST_LST",
    (dyn_str)"CST_SET",
    (dyn_str)"CST_DCT",

    (dyn_str)"LOC",
    (dyn_str)"LOCX",
    (dyn_str)"LOAD",
    (dyn_str)"ELEM",

    (dyn_str)"STORE",
    (dyn_str)"STORE_RF",
    (dyn_str)"STORE_LOC",

    (dyn_str)"CALL_FCTX",
    (dyn_str)"CALL_FCT",

    (dyn_str)"FJUMP",
    (dyn_str)"JUMP",

    (dyn_str)"PROC",

    (dyn_str)"IT_INIT",
    (dyn_str)"IT_NEXT0",
    (dyn_str)"IT_NEXT1",
    (dyn_str)"IT_NEXT2",
    (dyn_str)"IT_NEXT3",
    (dyn_str)"IT_STORE",
    (dyn_str)"IT_LIMIT",

    (dyn_str)"IT_GROUP",
    (dyn_str)"IT_ORDER",
    (dyn_str)"IT_AS",

    (dyn_str)"EXIT",
    (dyn_str)"REC_SET",

    (dyn_str)"TRY_1",
    (dyn_str)"TRY_0",

    (dyn_str)"REF",

    (dyn_str)"IT_INITX",
    (dyn_str)"IT_STOREX",
    (dyn_str)"CHK_FIRST",
    (dyn_str)"RET_X",
    (dyn_str)"IT_STOREX2",

    (dyn_str)"LOC_STEP",
    (dyn_str)"LOC_COUNT",

    (dyn_str)"IT_CYCLE",
    (dyn_str)"IT_UNIQUE",
};

static const dyn_str operators[] = {
  (dyn_str)"!",
  (dyn_str)"--",
  (dyn_str)"+",
  (dyn_str)"-",
  (dyn_str)"*",
  (dyn_str)"/",
  (dyn_str)"%",
  (dyn_str)"**",

  (dyn_str)"<",
  (dyn_str)"<=",
  (dyn_str)">",
  (dyn_str)">=",
  (dyn_str)"!=",
  (dyn_str)"==",
  (dyn_str)"and",
  (dyn_str)"xor",
  (dyn_str)"or",
  (dyn_str)"in",

  (dyn_str)"~",
  (dyn_str)"&",
  (dyn_str)"|",
  (dyn_str)"^",
  (dyn_str)"<<",
  (dyn_str)">>",

  (dyn_str)"EX" };


void vm_trace (vm_env* env, dyn_char* code);

#endif // S2_DEBUG

#endif // VM_TRACE_C_H
