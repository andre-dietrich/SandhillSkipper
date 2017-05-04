/**
 * @file vm_error_codes.h
 * @author André Dietrich
 * @date 9 April 2016
 *
 * @brief Definition of error-codes used by the type system and the vm.
 *
 * todo.
 * @see http://todo
 */


#define VM_IDLE   0
#define VM_OK     1
#define VM_ERROR  2

#define VM_LOAD_VAR_FCT_ERROR   3
#define VM_ELEM_LOAD_ERROR      4
#define VM_NOT_A_FUNCTION       5
#define VM_NO_SUCH_LOCAL_VARIABLE   4
#define VM_NO_SUCH_FUNCTION         5



#define VM_HEAP_ALLOCATION_ERROR    10
#define VM_OPERATION_NOT_PERMITTED  11
#define VM_FUNCTION_ERROR           12
