/**
 * @file vm_opcode.h
 * @author André Dietrich
 * @date 9 April 2016
 * @brief Definition of opcodes used by the Virtual Machine.
 *
 * todo.
 * @see http://todo
 */


#ifndef OPCODE_H__
#define OPCODE_H__

#define SP_SAVEX     0      /**< byte: Initialize vm.data with the follwoing
                                 char array and set initial stack-pointer     */
#define SP_SAVE      1      /**< Store current stack-pointer on stack         */

#define RET          2      /**< Remove last stack-frame                      */
#define RET_P        3      /**< Remove last stack-frame and restore pc/data/sp/...
                                 which are stored also on the stack           */

#define CST_N        4      /**< Push None value to stack.                    */
#define CST_0        5      /**< Push Boolean False to stack.                 */
#define CST_1        6      /**< Push Boolean True to stack.                  */
#define CST_B        7      /**< Push Integer to stack, second element is a
                                 char, which is used for small numeric values.*/
#define CST_S        8
#define CST_I        9         // int,   push int to stack
#define CST_F       10         // float, push float to stack

#define CST_STR     11         // char* until \0, push string on stack
#define CST_LST     12         // unsigned short, pop number of elements from stack and store to list .... push list on stack
#define CST_SET     13
#define CST_DCT     14

#define LOC         15
#define LOCX        16
#define LOAD        17        // char* until \0, read var from memory and push to stack
#define ELEM        18

#define STORE       19        // char* until \0, pop stack and store to memory
#define STORE_RF    20
#define STORE_LOC   21

#define CALL_FCTX   22
#define CALL_FCT    23      // unsigned char, char* until \0; of parameters, function name

#define FJUMP       24      // short
#define JUMP        25      // short

#define PROC        26      // unsigned char, unsigned short

#define IT_INIT     27
#define IT_NEXT0    28
#define IT_NEXT1    29
#define IT_NEXT2    30
#define IT_NEXT3    31
#define IT_STORE    32
#define IT_LIMIT    33

#define IT_GROUP    34
#define IT_ORDER    35
#define IT_AS       36

#define EXIT        37
#define REC_SET     38

#define TRY_1       39
#define TRY_0       40

#define REF         41

#define IT_INITX    42
#define IT_STOREX   43
#define CHK_FIRST   44
#define RETX        45
#define IT_STOREX2  46

#define LOC_STEP    47
#define LOC_COUNT   48
#define IT_CYCLE    49
#define IT_UNIQUE   50

#define YIELD       51

#define OP          (char) 0x40 //0b01000000
#define OPX         (char) 0x60 //0b01100000
#define OP_I        (char) 0x1f //0b00011111
#define OPERATION   (char) 0x9f //0b10011111

#define POP         (char) 0x80 //0b10000000
#define POP_I       (char) 0x7f //0b01111111
#define P           POP


#define NOT    0
#define NEG    1

#define ADD    2
#define SUB    3

#define MUL    4
#define DIV    5
#define MOD    6
#define POW    7

#define LT     8
#define LE     9
#define GE    10
#define GT    11
#define NE    12
#define EQ    13
#define ID    14

#define AND   15
#define XOR   16
#define OR    17

#define IN    18

#define B_NOT 19
#define B_AND 20
#define B_OR  21
#define B_XOR 22
#define LEFT  23
#define RIGHT 24

#define EX    25


#endif //OPCODE__H
