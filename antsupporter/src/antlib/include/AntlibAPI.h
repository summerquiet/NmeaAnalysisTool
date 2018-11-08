/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANTLIBAPI_H
#define ANTLIBAPI_H

/*---------------------------------------------------------------------------*/
// Macro define

#if defined(_MSC_VER)
#   ifdef ANTLIB_EXPORTS
#       define ANTLIB_API __declspec(dllexport)
#       define ANTLIB_DATA __declspec(dllexport)
#   else
#       ifdef ANTLIB_IS_SINGLEPROC
#           define ANTLIB_API
#           define ANTLIB_DATA
#       else
#           define ANTLIB_API __declspec(dllimport)
#           define ANTLIB_DATA __declspec(dllimport)
#       endif
#   endif
#elif defined(__GNUC__)
#   define ANTLIB_API __attribute__((visibility("default")))
#   define ANTLIB_DATA
#else
#   error "Unsupported Platform!!!"
#endif

/*---------------------------------------------------------------------------*/
#endif // ANTLIBAPI_H

/*---------------------------------------------------------------------------*/
/* EOF */
