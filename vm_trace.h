/**
 * @file vm_trace.h
 * @author André Dietrich
 * @date 9 April 2016
 *
 * @brief Implementation of stack-trace functionality for debug purposes.
 *
 * todo.
 * @see http://todo
 */

#ifndef VM_TRACE_C_H
#define VM_TRACE_C_H

#ifdef S2_DEBUG

void vm_trace (vm_env* env, dyn_char* code);

#endif // S2_DEBUG

#endif // VM_TRACE_C_H
