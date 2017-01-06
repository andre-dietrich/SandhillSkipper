/*0000*/ SP_SAVEX,    4, // 4
                      7, 'c', 'o', 'u', 'n', 't', 0, //  "count"
                      6, 'm', 'o', 'd', 'e', 0, //  "mode"
                      5, 'p', 'i', 'n', 0, //  "pin"
                      14, 'd', 'i', 'g', 'i', 't', 'a', 'l', 'W', 'r', 'i', 't', 'e', 0, //  "digitalWrite"
                      0, // local variables
/*0035*/ CST_B,       0, 
/*0037*/ STORE|P,     0, // "count"
/*0039*/ CST_1,       
/*0040*/ STORE|P,     1, // "mode"
/*0042*/ CST_B,       13, 
/*0044*/ STORE|P,     2, // "pin"
/*0046*/ LOAD,        0, // "count"
/*0048*/ CST_S,       -12, 1, // 500
/*0051*/ OP|GT,       1, // params: 2
/*0053*/ FJUMP,       24, 0, // 24
/*0056*/ SP_SAVE,     0, // local variables
/*0058*/ CST_B,       0, 
/*0060*/ STORE|P,     0, // "count"
/*0062*/ LOAD,        2, // "pin"
/*0064*/ LOAD,        1, // "mode"
/*0066*/ OP|NOT,      0, // params: 1
/*0068*/ STORE,       1, // "mode"
/*0070*/ LOAD,        3, // "digitalWrite"
/*0072*/ CALL_FCT,    2, // 2
/*0074*/ RET|P,       
/*0075*/ JUMP,        8, 0, // 8
/*0078*/ LOAD,        0, // "count"
/*0080*/ CST_B,       1, 
/*0082*/ OPX|ADD|P,   1, // params: 2
/*0084*/ JUMP,        -39, -1, // -39
/*0087*/ RET,         
