/*0000*/ SP_SAVEX,    13, // 13
                      6, 't', 'e', 'm', 'p', 0, //  "temp"
                      6, 'h', 'u', 'm', 'i', 0, //  "humi"
                      7, 'l', 'i', 'g', 'h', 't', 0, //  "light"
                      4, 'd', 'b', 0, //  "db"
                      3, 'i', 0, //  "i"
                      3, 'c', 0, //  "c"
                      12, 'a', 'n', 'a', 'l', 'o', 'g', 'R', 'e', 'a', 'd', 0, //  "analogRead"
                      3, 't', 0, //  "t"
                      3, 'h', 0, //  "h"
                      3, 'l', 0, //  "l"
                      6, '!', 'd', 'g', 'r', 0, //  "!dgr"
                      3, ',', 0, //  ","
                      6, 'd', 'r', 'a', 'w', 0, //  "draw"
                      2, // local variables
/*0068*/ CST_LST,     0, 0, // 0
/*0071*/ CST_LST,     0, 0, // 0
/*0074*/ CST_LST,     0, 0, // 0
/*0077*/ CST_DCT,     3, // 3
                      0, // "temp"
                      1, // "humi"
                      2, // "light"
/*0082*/ STORE|P,     3, // "db"
/*0084*/ CST_B,       0, 
/*0086*/ STORE_LOC|P, 4, // "i"
/*0088*/ CST_B,       0, 
/*0090*/ STORE_LOC|P, 5, // "c"
/*0092*/ LOC,         5, // "c"
/*0094*/ CST_S,       -12, 1, // 500
/*0097*/ OP|EQ,       1, // params: 2
/*0099*/ FJUMP,       -91, 0, // 165
/*0102*/ SP_SAVE,     3, // local variables
/*0104*/ CST_B,       0, 
/*0106*/ STORE_LOC|P, 5, // "c"
/*0108*/ CST_B,       54, 
/*0110*/ LOAD,        6, // "analogRead"
/*0112*/ CALL_FCT,    1, // 1
/*0114*/ STORE_LOC|P, 7, // "t"
/*0116*/ CST_B,       55, 
/*0118*/ LOAD,        6, // "analogRead"
/*0120*/ CALL_FCT,    1, // 1
/*0122*/ STORE_LOC|P, 8, // "h"
/*0124*/ CST_B,       56, 
/*0126*/ LOAD,        6, // "analogRead"
/*0128*/ CALL_FCT,    1, // 1
/*0130*/ STORE_LOC|P, 9, // "l"
/*0132*/ CST_STR,     10, // "!dgr"
/*0134*/ LOC,         8, // "h"
/*0136*/ CST_STR,     11, // ","
/*0138*/ LOC,         7, // "t"
/*0140*/ CST_STR,     11, // ","
/*0142*/ LOC,         9, // "l"
/*0144*/ LOAD,        12, // "draw"
/*0146*/ CALL_FCT|P,  6, // 6
/*0148*/ LOC,         4, // "i"
/*0150*/ CST_B,       49, 
/*0152*/ OP|GE,       1, // params: 2
/*0154*/ FJUMP,       53, 0, // 53
/*0157*/ SP_SAVE,     0, // local variables
/*0159*/ LOAD,        3, // "db"
/*0161*/ CST_STR,     0, // "temp"
/*0163*/ ELEM,        
/*0164*/ LOC,         4, // "i"
/*0166*/ CST_B,       50, 
/*0168*/ OP|SUB,      1, // params: 2
/*0170*/ ELEM,        
/*0171*/ LOC,         7, // "t"
/*0173*/ STORE_RF|P,  
/*0174*/ LOAD,        3, // "db"
/*0176*/ CST_STR,     1, // "humi"
/*0178*/ ELEM,        
/*0179*/ LOC,         4, // "i"
/*0181*/ CST_B,       50, 
/*0183*/ OP|SUB,      1, // params: 2
/*0185*/ ELEM,        
/*0186*/ LOC,         8, // "h"
/*0188*/ STORE_RF|P,  
/*0189*/ LOAD,        3, // "db"
/*0191*/ CST_STR,     2, // "light"
/*0193*/ ELEM,        
/*0194*/ LOC,         4, // "i"
/*0196*/ CST_B,       50, 
/*0198*/ OP|SUB,      1, // params: 2
/*0200*/ ELEM,        
/*0201*/ LOC,         9, // "l"
/*0203*/ STORE_RF,    
/*0204*/ RET|P,       
/*0205*/ JUMP,        32, 0, // 32
/*0208*/ SP_SAVE,     0, // local variables
/*0210*/ LOAD,        3, // "db"
/*0212*/ CST_STR,     0, // "temp"
/*0214*/ ELEM,        
/*0215*/ LOC,         7, // "t"
/*0217*/ OPX|ADD|P,   1, // params: 2
/*0219*/ LOAD,        3, // "db"
/*0221*/ CST_STR,     1, // "humi"
/*0223*/ ELEM,        
/*0224*/ LOC,         8, // "h"
/*0226*/ OPX|ADD|P,   1, // params: 2
/*0228*/ LOAD,        3, // "db"
/*0230*/ CST_STR,     2, // "light"
/*0232*/ ELEM,        
/*0233*/ LOC,         9, // "l"
/*0235*/ OPX|ADD,     1, // params: 2
/*0237*/ RET|P,       
/*0238*/ LOC,         4, // "i"
/*0240*/ CST_B,       1, 
/*0242*/ OPX|ADD|P,   1, // params: 2
/*0244*/ LOC,         4, // "i"
/*0246*/ CST_B,       99, 
/*0248*/ OP|GE,       1, // params: 2
/*0250*/ FJUMP,       9, 0, // 9
/*0253*/ CST_B,       50, 
/*0255*/ STORE_LOC,   4, // "i"
/*0257*/ JUMP,        3, 0, // 3
/*0260*/ CST_0,       
/*0261*/ RET|P,       
/*0262*/ JUMP,        8, 0, // 8
/*0265*/ LOC,         5, // "c"
/*0267*/ CST_B,       1, 
/*0269*/ OPX|ADD|P,   1, // params: 2
/*0271*/ JUMP,        76, -1, // -180
/*0274*/ RET,         
