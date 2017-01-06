/*0000*/ SP_SAVEX,    4, // 4
                      12, 'a', 'n', 'a', 'l', 'o', 'g', 'R', 'e', 'a', 'd', 0, //  "analogRead"
                      7, 'v', 'a', 'l', 'u', 'e', 0, //  "value"
                      22, 'M', 'e', 'a', 's', 'u', 'r', 'e', 'm', 'e', 'n', 't', ' ', 'R', 'e', 's', 'u', 'l', 't', ':', ' ', 0, //  "Measurement Result: "
                      7, 'p', 'r', 'i', 'n', 't', 0, //  "print"
                      1, // local variables
/*0051*/ CST_B,       54, 
/*0053*/ LOAD,        0, // "analogRead"
/*0055*/ CALL_FCT,    1, // 1
/*0057*/ STORE_LOC|P, 1, // "value"
/*0059*/ CST_STR,     2, // "Measurement Result: "
/*0061*/ LOC,         1, // "value"
/*0063*/ LOAD,        3, // "print"
/*0065*/ CALL_FCT,    2, // 2
/*0067*/ RET,         
