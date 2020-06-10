/*
**  @(#)platform.h
**
**  convert - platform dependencies
**  -------------------------------
**
**  copyright (c) 1996-2020 Code Construct Systems (CCS)
*/
#ifndef __PLATFORM_H
#define __PLATFORM_H

/*
** System default values (platform dependent)
*/
#ifdef  _ARCH_32
#define _IO_BUFFER_SIZE 2048    /* 32-bit */
#endif
#ifdef  _ARCH_64
#define _IO_BUFFER_SIZE 32768   /* 64-bit */
#endif

#endif /* __PLATFORM_H */
