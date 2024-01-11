/*
**  @(#)version.h
**
**  convert - version information
**  -----------------------------
**
**  copyright (c) 1996-2024 Code Construct Systems (CCS)
*/
#ifndef __VERSION_H
#define __VERSION_H

/*
** Check if version product and string are defined in another include file
*/
#ifdef  _VERSION_PRODUCT
#error  _VERSION_PRODUCT is defined in another include file
#endif
#ifdef  _VERSION_RELEASE
#error  _VERSION_RELEASE is defined in another include file
#endif

/*
** Define product and version strings
*/
#ifdef _LTOD
#define _VERSION_PRODUCT "ltod"
#define _VERSION_RELEASE "ltod 5.3.0 release 1"
#else
#define _VERSION_PRODUCT "dtol"
#define _VERSION_RELEASE "dtol 5.3.0 release 1"
#endif

#endif /* __VERSION_H */
