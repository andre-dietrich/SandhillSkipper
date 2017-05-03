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
/*0053*/ FJUMP,       19, 0, // 19
/*0056*/ SP_SAVE,     0, // local variables
/*0058*/ CST_B,       0, 
/*0060*/ STORE|P,     0, // "count"
/*0062*/ LOAD,        2, // "pin"
/*0064*/ LOAD,        1, // "mode"
/*0066*/ OPX|NOT,     0, // params: 1
/*0068*/ LOAD,        3, // "digitalWrite"
/*0070*/ CALL_FCT,    2, // 2
/*0072*/ RET|P,       
/*0073*/ LOAD,        0, // "count"
/*0075*/ CST_B,       1, 
/*0077*/ OPX|ADD|P,   1, // params: 2
/*0079*/ JUMP,        -34, -1, // -34
/*0082*/ RET,         
