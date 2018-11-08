/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#include "stdafx.h"

#ifdef _FOR_APPLE_

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_CONVERTER_H
#   include "Ant_Converter.h"
#endif
#ifndef ANT_STRINGDEF_H
#   include "Ant_StringDef.h"
#endif
#include <iconv.h>

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Value define

// Array to contain iconverter.
struct ICONV_CP_INFO
{
    iconv_t _conv;
    DWORD cp_from;
    DWORD cp_to;
};

static ICONV_CP_INFO CODEPAGE_ARRAY[ANT_CP_MAX * ANT_CP_MAX] = {0};

/*---------------------------------------------------------------------------*/
// Functions
static INT convert_len(iconv_t *pcd, const char* src, INT srclen)
{
#define MAX_STRING_BUF_LEN 256
    size_t retlen = 0;

    const char* inptr_b = src;
    size_t inlen_b = srclen;

    char* outptr_b = new char[MAX_STRING_BUF_LEN];
    size_t outlen_b = MAX_STRING_BUF_LEN;

    const char* inptr_a = inptr_b;
    size_t inlen_a = inlen_b;

    char* outptr_a = outptr_b;
    size_t outlen_a = outlen_b;

    while(1) {
        size_t res = iconv(*pcd, (char**)&inptr_a, &inlen_a, &outptr_a, &outlen_a);

        if (res == (size_t)(-1)) {
            if (errno == E2BIG) {
                delete[] outptr_b;
                inptr_b = src;
                inlen_b = srclen;

                outptr_b = new char[MAX_STRING_BUF_LEN*2];
                outlen_b = MAX_STRING_BUF_LEN*2;

                inptr_a = inptr_b;
                inlen_a = inlen_b;

                outptr_a = outptr_b;
                outlen_a = outlen_b;

                iconv(*pcd, NULL,NULL,NULL,NULL);
            }
            else {
                break;
            }
        }
        else {
            retlen = outlen_b - outlen_a;
            break;
        }
    }

    delete[] outptr_b;
    return retlen;

#undef MAX_STRING_BUF_LEN
}

INT convert(iconv_t *pcd, const char* src, INT srclen, char* out, INT outlen)
{
    size_t retlen = 0;

    const char* inptr_b = src;
    size_t inlen_b = srclen;
    char* outptr_b = out;
    size_t outlen_b = outlen;

    const char* inptr_a = inptr_b;
    size_t inlen_a = inlen_b;
    char* outptr_a = outptr_b;
    size_t outlen_a = outlen_b;

    if (srclen >0) {
        size_t res = iconv(*pcd, (char**)&inptr_a, &inlen_a, &outptr_a, &outlen_a);
        if (outptr_a != outptr_b) {
            retlen = outlen_b - outlen_a;
        }

        if (res == (size_t)(-1)) {
            if (errno == E2BIG) {
                printf("iconv out buff have not enought space\n");
            }
#if 0
            if (errno== EILSEQ) {
                iconvctl(cd, ICONV_SET_DISCARD_ILSEQ,&one);
            }
#endif
        }
    }

    return retlen;
}

VOID Ant_Converter::getCPName(const DWORD dwCodePage, CHAR* szCPName, INT nStrLen)
{
    switch (dwCodePage) {

    case ANT_CP_ACP:
        strncpy(szCPName, "ASCII", nStrLen);
        break;
    case ANT_CP_GBK:
        strncpy(szCPName, "GBK", nStrLen);
        break;
    case ANT_CP_GB18030:
        strncpy(szCPName, "GB18030", nStrLen);
        break;
    case ANT_CP_SJIS:
        strncpy(szCPName, "SHIFT_JIS", nStrLen);
        break;
    case ANT_CP_EUC_JP:
        strncpy(szCPName, "EUC-JP", nStrLen);
        break;
    default:
        snprintf(szCPName, nStrLen, "windows%d", static_cast<INT>(dwCodePage));
        break;
    }
}

INT Ant_Converter::convert(const CONV_CP_INFO& cp_info, CHAR* pszDst, const INT& nDstLen, const CHAR* pszSrc, const INT& nSrcLen)
{
    DWORD index = 0;
    for (; index < ANT_CP_MAX * ANT_CP_MAX; index++) {
        if (CODEPAGE_ARRAY[index]._conv == 0) {
            CODEPAGE_ARRAY[index].cp_from = cp_info.dwFromCP;
            CODEPAGE_ARRAY[index].cp_to = cp_info.dwToCP;
            CODEPAGE_ARRAY[index]._conv = iconv_open(cp_info.pszToCP, cp_info.pszFromCP);
            iconv(CODEPAGE_ARRAY[index]._conv, NULL, NULL, NULL, NULL);//initialize
            break;
        }
        else if (CODEPAGE_ARRAY[index].cp_from == cp_info.dwFromCP &&
            CODEPAGE_ARRAY[index].cp_to == cp_info.dwToCP) {
            break;
        }
    }

    iconv_t *pConv = &(CODEPAGE_ARRAY[index]._conv);

    if (pConv != NULL) {
        //size_t res = iconv(CODEPAGE_ARRAY[index]._conv,
        if(NULL == pszSrc && nSrcLen == 0) {
            return convert_len(pConv, pszSrc, nSrcLen);
        }
        else {
            return antsupporter::convert(pConv, pszSrc, nSrcLen, pszDst, nDstLen);
        }
    }

    return 0;
}

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif  // _FOR_APPLE_

/*---------------------------------------------------------------------------*/
/* EOF */
