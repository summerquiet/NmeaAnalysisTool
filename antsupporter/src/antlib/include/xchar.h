/**
 * Copyright @ 2014 - 2017 Suntec Software(Shanghai) Co., Ltd.
 * All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are NOT permitted except as agreed by
 * Suntec Software(Shanghai) Co., Ltd.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 */
/**
 * @file xchar.h
 * @brief Declare XCHAR related macros.
 *
 * This file includes support of the XCHAR, which could be switched between
 * wchar_t & char according to compile time configuration.
 */

#ifndef CXX_XCHAR_H
#define CXX_XCHAR_H

/*---------------------------------------------------------------------------*/
// Include files

#include <wchar.h>
#include <stdarg.h>

/*---------------------------------------------------------------------------*/
// Value define

#ifdef ANT_STRING_UNICODE
typedef WCHAR   XCHAR;
#else
typedef CHAR    XCHAR;
#endif

#ifdef ANT_STRING_UNICODE

// WORKAROUND: swprintf does not accept similar argumets than sprintf
inline int _swprintf(wchar_t *str, const wchar_t *format, ...)
{
    // Hack: assume all buffers are equal or greater than 256 bytes.
    const static int DUMMY_BUFFER_SIZE = 256;

    va_list va;
    va_start(va, format);
    int ret = vswprintf(str, DUMMY_BUFFER_SIZE, format, va);
    va_end(va);

    return ret;
}

// WORKAROUND: vswprintf does not accept similar argumets than vsprintf
inline int _vswprintf(char *str, const char *format, va_list ap)
{
    // Hack: assume all buffers are equal or greater than 256 bytes.
    const static int DUMMY_BUFFER_SIZE = 256;
    return vswprintf(str, DUMMY_BUFFER_SIZE, format, va);
}

#endif

#ifdef ANT_STRING_UNICODE
#   define XTEXT(C)     L##C
#   define xcscat       wcscat
#   define xcschr       wcschr
#   define xcscmp       wcscmp
#   define xcscpy       wcscpy
#   define xcscspn      wcscspn
#   define xcslen       wcslen
#   define xcsncat      wcsncat
#   define xcsncmp      wcsncmp
#   define xcsncpy      wcsncpy
#   define xcspbrk      wcspbrk
#   define xcsrchr      wcsrchr
#   define xcsspn       wcsspn
#   define xcsstr       wcsstr
#   define xcstok       wcstok
#   define xisalnum     iswalnum
#   define xisalpha     iswalpha
#   define xiscntrl     iswcntrl
#   define xisdigit     iswdigit
#   define xisgraph     iswgraph
#   define xislower     iswlower
#   define xisprint     iswprint
#   define xispunct     iswpunct
#   define xisspace     iswspace
#   define xisupper     iswupper
#   define xisxdigit    iswxdigit
#   define xtolower     towlower
#   define xtoupper     towupper
#   define xtoi         _wtoi
#   define xtol         _wtol
#   define xcstod       wcstod
#   define xcstol       wcstol
#   define xcstoul      wcstoul
#   define xcslwr       _wcslwr
#   define xcsupr       _wcsupr
#   define xcscasecmp   _wcsicmp
#   define xcsncasecmp  _wcsnicmp
#   define xprintf      wprintf
#   define xfprintf     fwprintf
#   define xsprintf     _swprintf
#   define xsnprintf    swprintf    // NOTICE: return value differs from snprintf
#   define xvprintf     vwprintf
#   define xvfprintf    vfwprintf
#   define xvsprintf    _vswprintf
#   define xvsnprintf   vswprintf   // NOTICE: return value differs from vsnprintf
#   define xsscanf      swscanf
#else // ANT_STRING_UNICODE
#   define XTEXT(C)     C
#   define xcscat       strcat
#   define xcschr       strchr
#   define xcscmp       strcmp
#   define xcscpy       strcpy
#   define xcscspn      strcspn
#   define xcslen       strlen
#   define xcsncat      strncat
#   define xcsncmp      strncmp
#   define xcsncpy      strncpy
#   define xcspbrk      strpbrk
#   define xcsrchr      strrchr
#   define xcsspn       strspn
#   define xcsstr       strstr
#   define xcstok       strtok
#   define xisalnum     isalnum
#   define xisalpha     isalpha
#   define xiscntrl     iscntrl
#   define xisdigit     isdigit
#   define xisgraph     isgraph
#   define xislower     islower
#   define xisprint     isprint
#   define xispunct     ispunct
#   define xisspace     isspace
#   define xisupper     isupper
#   define xisxdigit    isxdigit
#   define xtolower     tolower
#   define xtoupper     toupper
#   define xtoi         atoi
#   define xtol         atol
#   define xcstod       strtod
#   define xcstol       strtol
#   define xcstoul      strtoul
#   define xcslwr       _strlwr
#   define xcsupr       _strupr
#   define xcscasecmp   _stricmp
#   define xcsncasecmp  _strnicmp
#   define xprintf      printf
#   define xfprintf     fprintf
#   define xsprintf     sprintf
#   define xsnprintf    snprintf
#   define xvprintf     vprintf
#   define xvfprintf    vfprintf
#   define xvsprintf    vsprintf
#   define xvsnprintf   vsnprintf
#   define xsscanf      sscanf
#endif // ANT_STRING_UNICODE

/*---------------------------------------------------------------------------*/
#endif  // CXX_XCHAR_H

/*---------------------------------------------------------------------------*/
/* EOF */
