/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#include "stdafx.h"

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_CONVERTER_H
#   include "Ant_Converter.h"
#endif
#ifndef ANT_STRINGDEF_H
#   include "Ant_StringDef.h"
#endif

#ifdef _LINUX
#include <unicode/utypes.h>   /* Basic ICU data types */
#include <unicode/ucnv.h>     /* C   Converter API    */
#include <unicode/ustring.h>  /* some more string fcns*/
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Value define

#define ANT_CONVERTER_LOG   0

const DWORD CODE_PAGE_NAME_LEN = 20;

/*---------------------------------------------------------------------------*/
// Functions
#ifdef _LINUX
INT Ant_Converter::convert(const CONV_CP_INFO& cp_info, CHAR* pszDst, const INT& nDstLen, const CHAR* pszSrc, const INT& nSrcLen)
{
// TEST by summer
#if 0
    UErrorCode eErrorCode = U_ZERO_ERROR;
    int32_t length = ::ucnv_convert(cp_info.pszToCP,
                                    cp_info.pszFromCP,
                                    pszDst,
                                    nDstLen,
                                    pszSrc,
                                    nSrcLen,
                                    &eErrorCode);
    if (ANT_CONVERTER_LOG) printf("[%s]To[%s] err: %d, len: (src:%d -> dst:%d) = %d (src [%s] -> dst: [%s])\n", cp_info.pszFromCP, cp_info.pszToCP,
                                    eErrorCode, nSrcLen, nDstLen, length, pszSrc?pszSrc:"NULL", pszDst?pszDst:"NULL");
    return length;
#else
    return 0;
#endif
}

VOID Ant_Converter::getCPName(const DWORD dwCodePage, CHAR* szCPName, INT nStrLen)
{
    snprintf(szCPName, nStrLen, "windows%d", static_cast<INT>(dwCodePage));
}
#endif

INT Ant_Converter::utf8ToChar(const DWORD& dwCodePage, const CHAR* pszSrc, const INT& nSrcLen, CHAR* pszDst, const INT& nDstLen)
{
#if defined(_LINUX) || defined(_FOR_ANDROID_) || defined(_FOR_APPLE_)
    if (NULL == pszSrc || nDstLen < 0) {
        return 0;
    }
    char szToName[CODE_PAGE_NAME_LEN];
    memset(szToName, 0, sizeof(szToName));
    getCPName(dwCodePage, szToName, CODE_PAGE_NAME_LEN);

    char szFromName[] = "UTF-8";
    CONV_CP_INFO cp_info = {dwCodePage, szToName, ANT_CP_UTF8, szFromName};
    INT length = convert(cp_info,
                        pszDst,
                        nDstLen,
                        pszSrc,
                        nSrcLen);
    return length;
#elif defined(_WIN32) || defined(_WIN32_WCE)
    //TODO: for windows
    return 0;
#else
    return 0;
#endif
}

INT Ant_Converter::utf8ToWChar( const CHAR* pszSrc, const INT& nSrcLen, WCHAR* pszDst, const INT& nDstLen)
{
#if defined(_LINUX) || defined(_FOR_ANDROID_) || defined(_FOR_APPLE_)
    if (NULL == pszSrc || nDstLen < 0) {
        return 0;
    }
#ifdef COMP_OPT_LITTLE_ENDIAN
    char szToName[] = "UTF-32LE";
    DWORD dwToCP = ANT_CP_UTF32LE;
#else
    char szToName[] = "UTF-32BE";
    DWORD dwToCP = ANT_CP_UTF32BE;
#endif

    char szFromName[] = "UTF-8";
    CONV_CP_INFO cp_info = {dwToCP, szToName, ANT_CP_UTF8, szFromName};
    INT length = convert(cp_info,
                        (CHAR*)pszDst,
                        nDstLen*sizeof(WCHAR),
                        pszSrc,
                        nSrcLen);
    return length;
#elif defined(_WIN32) || defined(_WIN32_WCE)
    //TODO: for windows
    return 0;
#else
    return 0;
#endif
}

INT Ant_Converter::charToUTF8( const DWORD& dwCodePage, const CHAR* pszSrc, const INT& nSrcLen, CHAR* pszDst, const INT& nDstLen)
{
#if defined(_LINUX) || defined(_FOR_ANDROID_) || defined(_FOR_APPLE_)
    if (NULL == pszSrc || nDstLen < 0) {
        return 0;
    }
    char szFromName[CODE_PAGE_NAME_LEN];
    memset(szFromName, 0, sizeof(szFromName));
    getCPName(dwCodePage, szFromName, CODE_PAGE_NAME_LEN);

    char szToName[] = "UTF-8";
    CONV_CP_INFO cp_info = {ANT_CP_UTF8, szToName, dwCodePage, szFromName};
    INT length = convert(cp_info,
                        pszDst,
                        nDstLen,
                        pszSrc,
                        nSrcLen);
    return length;
#elif defined(_WIN32) || defined(_WIN32_WCE)
    //TODO: for windows
    return 0;
#else
    return 0;
#endif
}

INT Ant_Converter::wcharToUTF8(const WCHAR* pszSrc, const INT& nSrcLen, CHAR* pszDst, const INT& nDstLen)
{
#if defined(_LINUX) || defined(_FOR_ANDROID_) || defined(_FOR_APPLE_)
    if (NULL == pszSrc || nDstLen < 0) {
        return 0;
    }
#ifdef COMP_OPT_LITTLE_ENDIAN
    char szFromName[] = "UTF-32LE";
    DWORD dwFromCP = ANT_CP_UTF32LE;
#else
    char szFromName[] = "UTF-32BE";
    DWORD dwFromCP = ANT_CP_UTF32BE;
#endif

    char szToName[] = "UTF-8";
    CONV_CP_INFO cp_info = {ANT_CP_UTF8, szToName, dwFromCP, szFromName};
    INT length = convert(cp_info,
                        pszDst,
                        nDstLen,
                        (const CHAR*)pszSrc,
                        nSrcLen*sizeof(WCHAR));
    return length;
#elif defined(_WIN32) || defined(_WIN32_WCE)
    //TODO: for windows
    return 0;
#else
    return 0;
#endif
}

INT Ant_Converter::utf16BEToUTF8(const UCHAR16* pszSrc, const INT& nSrcLen, CHAR* pszDst, const INT& nDstLen)
{
#if defined(_LINUX) || defined(_FOR_ANDROID_) || defined(_FOR_APPLE_)
    if (NULL == pszSrc || nDstLen < 0) {
        return 0;
    }
    char szFromName[] = "UTF-16BE";
    char szToName[] = "UTF-8";
    CONV_CP_INFO cp_info = {ANT_CP_UTF8, szToName, ANT_CP_UTF16BE, szFromName};
    INT length = convert(cp_info,
                        pszDst,
                        nDstLen,
                        (const CHAR*)pszSrc,
                        nSrcLen*sizeof(UCHAR16));
    return length;
#elif defined(_WIN32) || defined(_WIN32_WCE)
    //TODO: for windows
    return 0;
#else
    return 0;
#endif
}

INT Ant_Converter::utf16LEToUTF8(const UCHAR16* pszSrc, const INT& nSrcLen, CHAR* pszDst, const INT& nDstLen)
{
#if defined(_LINUX) || defined(_FOR_ANDROID_) || defined(_FOR_APPLE_)
    if (NULL == pszSrc || nDstLen < 0) {
        return 0;
    }
    char szFromName[] = "UTF-16LE";
    char szToName[] = "UTF-8";
    CONV_CP_INFO cp_info = {ANT_CP_UTF8, szToName, ANT_CP_UTF16LE, szFromName};
    INT length = convert(cp_info,
                        pszDst,
                        nDstLen,
                        (const CHAR*)pszSrc,
                        nSrcLen*sizeof(UCHAR16));
    return length;
#elif defined(_WIN32) || defined(_WIN32_WCE)
    //TODO: for windows
    return 0;
#else
    return 0;
#endif
}

INT Ant_Converter::utf8ToUTF16BE(const CHAR* pszSrc, const INT& nSrcLen, UCHAR16* pszDst, const INT& nDstLen)
{
#if defined(_LINUX) || defined(_FOR_ANDROID_) || defined(_FOR_APPLE_)
    if (NULL == pszSrc || nDstLen < 0) {
        return 0;
    }
    char szFromName[] = "UTF-8";
    char szToName[] = "UTF-16BE";
    CONV_CP_INFO cp_info = {ANT_CP_UTF16BE, szToName, ANT_CP_UTF8, szFromName};
    INT length = convert(cp_info,
                        (CHAR*)pszDst,
                        nDstLen*sizeof(UCHAR16),
                        pszSrc,
                        nSrcLen);
    return length;
#elif defined(_WIN32) || defined(_WIN32_WCE)
    //TODO: for windows
    pszDst[0] = 0;
    return 0;
#else
    return 0;
#endif
}

INT Ant_Converter::utf8ToUTF16LE(const CHAR* pszSrc, const INT& nSrcLen, UCHAR16* pszDst, const INT& nDstLen)
{
#if defined(_LINUX) || defined(_FOR_ANDROID_) || defined(_FOR_APPLE_)
    if (NULL == pszSrc || nDstLen < 0) {
        return 0;
    }
    char szFromName[] = "UTF-8";
    char szToName[] = "UTF-16LE";
    CONV_CP_INFO cp_info = {ANT_CP_UTF16LE, szToName, ANT_CP_UTF8, szFromName};
    INT length = convert(cp_info,
                        (CHAR*)pszDst,
                        nDstLen*sizeof(UCHAR16),
                        pszSrc,
                        nSrcLen);
    return length;
#elif defined(_WIN32) || defined(_WIN32_WCE)
    //TODO: for windows
    pszDst[0] = 0;
    return 0;
#else
    return 0;
#endif
}

INT Ant_Converter::utf16ToChar(const DWORD& dwCodePage, const UCHAR16* pszSrc, const INT& nSrcLen, CHAR* pszDst, const INT& nDstLen)
{
#if defined(_LINUX) || defined(_FOR_ANDROID_) || defined(_FOR_APPLE_)
    if (NULL == pszSrc || nDstLen < 0) {
        return 0;
    }
    char szToName[CODE_PAGE_NAME_LEN];
    memset(szToName, 0, sizeof(szToName));
    getCPName(dwCodePage, szToName, CODE_PAGE_NAME_LEN);

#ifdef COMP_OPT_LITTLE_ENDIAN
    char szFromName[] = "UTF-16LE";
    DWORD dwFromCP = ANT_CP_UTF16LE;
#else
    char szFromName[] = "UTF-16BE";
    DWORD dwFromCP = ANT_CP_UTF16BE;
#endif

    CONV_CP_INFO cp_info = {dwCodePage, szToName, dwFromCP, szFromName};
    INT length = convert(cp_info,
                        pszDst,
                        nDstLen,
                        (const CHAR*)pszSrc,
                         nSrcLen*sizeof(UCHAR16));
    return length;
#elif defined(_WIN32) || defined(_WIN32_WCE)
    INT length = ::WideCharToMultiByte(dwCodePage, 0, pszSrc, nSrcLen, pszDst, nDstLen, 0 ,0);
    return length;
#else
    return 0;
#endif
}

INT Ant_Converter::charToUTF16(const DWORD& dwCodePage, const CHAR* pszSrc, const INT& nSrcLen, UCHAR16* pszDst, const INT& nDstLen)
{
#if defined(_LINUX) || defined(_FOR_ANDROID_) || defined(_FOR_APPLE_)
    if (NULL == pszSrc || nDstLen < 0) {
        return 0;
    }
    char szFromName[CODE_PAGE_NAME_LEN];
    memset(szFromName, 0, sizeof(szFromName));
    getCPName(dwCodePage, szFromName, CODE_PAGE_NAME_LEN);

#ifdef COMP_OPT_LITTLE_ENDIAN
    char szToName[] = "UTF-16LE";
    DWORD dwToCP = ANT_CP_UTF16LE;
#else
    char szToName[] = "UTF-16BE";
    DWORD dwToCP = ANT_CP_UTF16BE;
#endif

    CONV_CP_INFO cp_info = {dwToCP, szToName, dwCodePage, szFromName};
    INT length = convert(cp_info,
                        (CHAR*)pszDst,
                        nDstLen*sizeof(UCHAR16),
                        pszSrc,
                        nSrcLen);
    return length;
#elif defined(_WIN32) || defined(_WIN32_WCE)
    INT length = ::MultiByteToWideChar(dwCodePage, 0, pszSrc, nSrcLen, pszDst, nDstLen);
    return length;
#else
    return 0;
#endif
}

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
/* EOF */
