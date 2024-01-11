/*
**  @(#)convert.h
**
**  convert - text file converter utility
**  -------------------------------------
**
**  copyright (c) 1996-2024 Code Construct Systems (CCS)
*/
#ifndef __CONVERT_H
#define __CONVERT_H

/*
** Unwind buffer stack pointer
*/
extern jmp_buf unwind_buffer_sp;

/*
** Define exit application macro (unwind stack to last setjmp)
*/
#ifndef EXIT_APPLICATION
#define EXIT_APPLICATION(_code) longjmp(unwind_buffer_sp, _code)
#else
#error  EXIT_APPLICATION is defined in another include file
#endif

#endif /* __CONVERT_H */
