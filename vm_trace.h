#ifndef VM_TRACE_C_H
#define VM_TRACE_C_H

#ifdef S2_DEBUG

static const ss_str opcodes[] = {
  (ss_str)"RET",
  (ss_str)"SP_SAVEX",
  (ss_str)"RET_P",
  (ss_str)"SP_SAVE",

  (ss_str)"CST_N",
  (ss_str)"CST_0",
  (ss_str)"CST_1",
  (ss_str)"CST_B",
  (ss_str)"CST_S",
  (ss_str)"CST_I",
  (ss_str)"CST_F",
  (ss_str)"CST_STR",
  (ss_str)"CST_LST",
  (ss_str)"CST_SET",
  (ss_str)"CST_DCT",

  (ss_str)"LOAD",
  (ss_str)"ELEM",
  (ss_str)"STORE",
  (ss_str)"STORE_RF",
  (ss_str)"STORE_LOC",

  (ss_str)"CALL_OPX",
  (ss_str)"CALL_OP",

  (ss_str)"CALL_FCTX",
  (ss_str)"CALL_FCT",

  (ss_str)"FJUMP",
  (ss_str)"JUMP",

  (ss_str)"PROC",

  (ss_str)"LOC",
  (ss_str)"LOCX",

  (ss_str)"IT_INIT",

  (ss_str)"IT_NEXT0",
  (ss_str)"IT_NEXT1",
  (ss_str)"IT_NEXT2",
  (ss_str)"IT_NEXT3",

  (ss_str)"IT_STORE",
  (ss_str)"IT_LIMIT",
  (ss_str)"IT_GROUP",
  (ss_str)"IT_ORDER",
  (ss_str)"IT_AS",

  (ss_str)"EXIT",
  (ss_str)"TRY_1",
  (ss_str)"TRY_0",
  (ss_str)"REF",
  (ss_str)"REC_SET"};

static const ss_str operators[] = {
  (ss_str)"!",
  (ss_str)"--",
  (ss_str)"+",
  (ss_str)"-",
  (ss_str)"*",
  (ss_str)"/",
  (ss_str)"%",
  (ss_str)"**",

  (ss_str)"<",
  (ss_str)"<=",
  (ss_str)">",
  (ss_str)">=",
  (ss_str)"!=",
  (ss_str)"==",
  (ss_str)"and",
  (ss_str)"xor",
  (ss_str)"or",
  (ss_str)"in",

  (ss_str)"~",
  (ss_str)"&",
  (ss_str)"|",
  (ss_str)"^",
  (ss_str)"<<",
  (ss_str)">>",

  (ss_str)"EX" };


void vm_trace (vm_env* env, ss_char* code);

#endif // S2_DEBUG

#endif // VM_TRACE_C_H
