/**
 * @file opcode.h
 * @author André Dietrich
 * @date 9 April 2016
 * @brief Definition of opcodes used by the Virtual Machine.
 *
 * todo.
 * @see http://todo
 */


#ifndef OPCODE_H__
#define OPCODE_H__

#define RET          0         // unsigned char
#define RET_L        1                               // 0b000 0001 = remove slices
#define RET_P        2                        // 0b000 0010 = restore program counter
                               // 0b000 0100 = increase program counter by one
                               // 0b000 1000 = return from loop
#define SP_SAVE      3       /**< Store current stack-pointer on stack.         */

#define CST_N        4       /**< Push None value to stack.                     */
#define CST_0        5       /**< Push Boolean False to stack.                  */
#define CST_1        6       /**< Push Boolean True to stack.                   */
#define CST_B        7       /**< Push Integer to stack, second element is a
                                char, which is used for small numeric values. */
#define CST_S        8
#define CST_I        9         // int,   push int to stack
#define CST_F       10         // float, push float to stack

#define CST_STR     11         // char* until \0, push string on stack
#define CST_LST     12         // unsigned short, pop number of elements from stack and store to list .... push list on stack
#define CST_SET     13
#define CST_DCT     14

#define LOAD        15        // char* until \0, read var from memory and push to stack
#define ELEM        16

#define STORE       17        // char* until \0, pop stack and store to memory
#define STORE_RF    18
#define STORE_LOC   19

#define CALL_OPX    20
#define CALL_OP     21        // char, operation with 1 or two params

#define CALL_FCTX   22
#define CALL_FCT    23      // unsigned char, char* until \0; of parameters, function name

#define FJUMP       24      // short
#define JUMP        25      // short

#define PROC        26      // unsigned char, unsigned short

#define LOC         27
#define LOCX        28

#define IT_INIT     29
#define IT_NEXT0    30
#define IT_NEXT1    31
#define IT_NEXT2    32
#define IT_NEXT3    33
#define IT_STORE    34
#define IT_LIMIT    35

#define IT_GROUP    36
#define IT_ORDER    37
#define IT_AS       38

#define EXIT        39

#define TRY_1       40
#define TRY_0       41

#define REF         42

#define POP         0b10000000
#define POP_I       0b01111111
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

#define AND   14
#define XOR   15
#define OR    16

#define IN    17

#define B_NOT 18
#define B_AND 19
#define B_OR  20
#define B_XOR 21
#define LEFT  22
#define RIGHT 23

#define EX    24


#endif //OPCODE__H
