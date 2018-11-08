/**
 * Copyright @ 2014 - 2017 Suntec Software(Shanghai) Co., Ltd.
 * All Rights Reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are NOT permitted except as agreed by
 * Suntec Software(Shanghai) Co., Ltd.
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 */

#include "stdafx.h"

#ifdef _FOR_ANDROID_

#ifndef CXX_CL_CONVERTER_H
#   include "CL_Converter.h"
#endif
#ifndef CXX_CL_STRING_DEF_H
#   include "CL_String_DEF.h"
#endif
#ifndef CXX_CL_DEBUG_H
#   include "CL_Debug.h"
#endif
#include <android/log.h>
#include <dlfcn.h>

/**
 * Creates a UConverter object with the name
 * of a coded character set specified as a C string.
 */
typedef void* (*ucnv_open_t)(const char* codepage, int *err);

/**
 * Deletes the unicode converter and releases resources
 * associated with just this instance.
 */
typedef void (*ucnv_close_t)(void* converter);

/**
 * Convert from one external charset to another
 * using two existing UConverters.
 * Internally, two conversions - ucnv_toUnicode()
 * and ucnv_fromUnicode() - are used, "pivoting"
 * through 16-bit Unicode.
 * Important: For streaming conversion (multiple
 * function calls for successive parts of a text stream),
 * the caller must provide a pivot buffer explicitly,
 * and must preserve the pivot buffer and associated
 * pointers from one call to another.
 * (The buffer may be moved if its contents and
 * the relative pointer positions are preserved.)
 */
typedef void (*ucnv_convertEx_t)(void* targetCnv, void* sourceCnv,
                                char **target, const char *targetLimit,
                                const char **source, const char *sourceLimit,
                                int *pivotStart, int **pivotSource,
                                int **pivotTarget, const int *pivotLimit,
                                char reset, char flush, int *pErrorCode);


typedef int (*ucnv_convert_t)(const char* destName, const char* srcName,
                                char* dest, int destLen,
                                const char* src, int srcLen,
                                int* pErrorCode);

static int initialize(void);
static void finalize(void);

static void* s_pDL = 0;
#define DECL_FUNC(name) static name##_t name = 0
//DECL_FUNC(ucnv_open);
//DECL_FUNC(ucnv_close);
//DECL_FUNC(ucnv_convertEx);
DECL_FUNC(ucnv_convert);
#undef DECL_FUNC

static int initialize(void)
{
    if(0 != s_pDL) {
        return 1;
    }
    //memset(s_pcConverter, 0, sizeof(s_pcConverter));
    if(0 != (s_pDL = dlopen("/system/lib/libicuuc.so", RTLD_LAZY)))
    {
#define LD_FUNC(name) name = (name##_t)dlsym(s_pDL, #name"_48");
        LD_FUNC(ucnv_convert);
#undef LD_FUNC
        if(0 != ucnv_convert) {
            return 1;
        }

#define LD_FUNC(name) name = (name##_t)dlsym(s_pDL, #name"_46");
        LD_FUNC(ucnv_convert);
#undef LD_FUNC
        if(0 != ucnv_convert) {
            return 1;
        }

#define LD_FUNC(name) name = (name##_t)dlsym(s_pDL, #name"_44");
        LD_FUNC(ucnv_convert);
#undef LD_FUNC
        if(0 != ucnv_convert) {
            return 1;
        }

#define LD_FUNC(name) name = (name##_t)dlsym(s_pDL, #name"_4_2");
        LD_FUNC(ucnv_convert);
#undef LD_FUNC
        if(0 != ucnv_convert) {
            return 1;
        }

#define LD_FUNC(name) name = (name##_t)dlsym(s_pDL, #name"_3_8");
        //LD_FUNC(ucnv_open);
        //LD_FUNC(ucnv_close);
        //LD_FUNC(ucnv_convertEx);
        LD_FUNC(ucnv_convert);
#undef LD_FUNC
        if(0 != ucnv_convert) {
            return 1;
        }
        else {
            CL_PERROR("load function ucnv_convert failed!\n");
        }
    }
    else {
        CL_PERROR("dlopen libicuuc.so failed!\n");
    }
    finalize();
    return 0;
}

static void finalize(void)
{
    ucnv_convert = 0;
    if(0 != s_pDL) {
        dlclose(s_pDL);
        s_pDL = 0;
    }
}

VOID
CL_Converter::GetCPName(const DWORD dwCodePage, CHAR* szCPName, INT nStrLen)
{
    switch (dwCodePage) {
        case CL_CP_ACP:
            strncpy(szCPName, "ASCII", nStrLen);
            break;
        case CL_CP_GBK:
            strncpy(szCPName, "GBK", nStrLen);
            break;
        case CL_CP_GB18030:
            strncpy(szCPName, "GB18030", nStrLen);
            break;
        case CL_CP_SJIS:
            strncpy(szCPName, "SHIFT_JIS", nStrLen);
            break;
        case CL_CP_EUC_JP:
            strncpy(szCPName, "EUC_JP", nStrLen);
            break;
        default:
            snprintf(szCPName, nStrLen, "windows%hu", static_cast<INT>(dwCodePage));
            break;
    }
}

INT
CL_Converter::Convert(    const CONV_CP_INFO& cp_info,
                       CHAR* pszDst,
                       const INT& nDstLen,
                       const CHAR* pszSrc,
                       const INT& nSrcLen)
{
    int eErrorCode = 0;
    if(0 != initialize()) {
        int32_t length = ucnv_convert(cp_info.pszToCP,
                                    cp_info.pszFromCP,
                                    pszDst,
                                    nDstLen,
                                    pszSrc,
                                    nSrcLen,
                                    &eErrorCode);
        return length;
    }
    return 0;
}

#endif

/* EOF */
