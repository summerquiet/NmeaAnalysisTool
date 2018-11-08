/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#include "stdafx.h"

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_CODE_H
#   include "Ant_Code.h"
#endif
#if defined(_WIN32) || defined(_WIN32_WCE)
#   include <tchar.h>
#endif //_WIN32 || _WIN32_WCE

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Functions
Ant_Code::Ant_Code()
{
    m_iCodeSize    = 0;
    m_iMaxCodeSize = ANT_CODE_MAX;
    m_kszOrgCode   = NULL;
    memset(m_szConvCode,0x00,sizeof(m_szConvCode));
}

Ant_Code::Ant_Code(const CHAR* szOrgCode ,const INT iMaxCodeSize)
{
    m_iCodeSize    = 0;
    m_iMaxCodeSize = iMaxCodeSize;
    m_kszOrgCode   = szOrgCode;
    memset(m_szConvCode,0x00,sizeof(m_szConvCode));
}

Ant_Code::~Ant_Code()
{
}

VOID Ant_Code::set(const CHAR* szOrgCode ,const INT iMaxCodeSize)
{
    m_iCodeSize = 0;
    m_iMaxCodeSize = iMaxCodeSize;
    m_kszOrgCode = szOrgCode;
    if (lstrcmp(m_szConvCode,TEXT(""))!=0) {
        memset(m_szConvCode,0x00,sizeof(m_szConvCode));
    }
}

VOID Ant_Code::clear()
{
    m_iCodeSize    = 0;
    m_iMaxCodeSize = 0;
    m_kszOrgCode   = NULL;

    if (lstrcmp(m_szConvCode,TEXT(""))!=0) {
        memset(m_szConvCode,0x00,sizeof(m_szConvCode));
    }
}

BOOL Ant_Code::convert(DWORD dwCodePage)
{
    if (lstrcmp(m_szConvCode,TEXT(""))!=0) {
        memset(m_szConvCode,0x00,sizeof(m_szConvCode));
    }

#if !defined(UNICODE) || !defined(_UNICODE)
    strcpy(m_szConvCode, m_kszOrgCode);
    m_iCodeSize = strlen(m_szConvCode)+1;
    return TRUE;
#else // UNICODE
    // S-JIS -> UNICODE
    //Here is a risk that, if iMaxCodeSize set in is bigger than ANT_CODE_MAX,
    //it may be overflow. Yet, some say, leave it alone. Therefore, I leave it alone.
    m_iCodeSize = ::MultiByteToWideChar(dwCodePage, MB_PRECOMPOSED, m_kszOrgCode, -1, m_szConvCode, m_iMaxCodeSize);

    if (m_iCodeSize) {
       return TRUE;
    }
    else{
        TCHAR szErrorBuf[64] = {0};
        _sntprintf_s(szErrorBuf, 64, 64, TEXT("Convert MultiByte to WideChar NG! ErrorCode:%d\n"), ::GetLastError());
        ::OutputDebugString(szErrorBuf);
        return FALSE;
    }
#endif // UNICODE
}

INT Ant_Code::getSize()
{
    return m_iCodeSize;
}

TCHAR* Ant_Code::get()
{
    return m_szConvCode;
}

Ant_Decode::Ant_Decode()
{
    m_iCodeSize    = 0;
    m_iMaxCodeSize = ANT_CODE_MAX;
    m_kszOrgCode   = NULL;
    memset(m_szConvCode,0x00,sizeof(m_szConvCode));
}

Ant_Decode::Ant_Decode(const TCHAR* szOrgCode ,const INT iMaxCodeSize)
{
    m_iCodeSize    = 0;
    m_iMaxCodeSize = iMaxCodeSize;
    m_kszOrgCode   = szOrgCode;
    memset(m_szConvCode,0x00,sizeof(m_szConvCode));
}

Ant_Decode::~Ant_Decode()
{
    if (strcmp(m_szConvCode,"")!=0) {
        memset(m_szConvCode,0x00,sizeof(m_szConvCode));
    }
}

VOID
Ant_Decode::set(const TCHAR* szOrgCode ,const INT iMaxCodeSize)
{
    m_iCodeSize = 0;
    m_iMaxCodeSize = iMaxCodeSize;
    m_kszOrgCode = szOrgCode;
    if (strcmp(m_szConvCode,"")!=0) {
        memset(m_szConvCode,0x00,sizeof(m_szConvCode));
    }
}

VOID Ant_Decode::clear()
{
    m_iCodeSize    = 0;
    m_iMaxCodeSize = 0;
    m_kszOrgCode   = NULL;

    if (strcmp(m_szConvCode,"")!=0) {
        memset(m_szConvCode,0x00,sizeof(m_szConvCode));
    }
}

BOOL Ant_Decode::convert(DWORD dwCodePage)
{
    if (strcmp(m_szConvCode,"")!=0) {
        memset(m_szConvCode,0x00,sizeof(m_szConvCode));
    }

#if !defined(UNICODE) || !defined(_UNICODE)
    strcpy(m_szConvCode, m_kszOrgCode);
    m_iCodeSize = strlen(m_szConvCode)+1;
    return TRUE;
#else // UNICODE
    // S-JIS -> UNICODE
    //Here is a risk that, if iMaxCodeSize set in is bigger than ANT_CODE_MAX,
    //it may be overflow. Yet, some say, leave it alone. Therefore, I leave it alone.
    m_iCodeSize = ::WideCharToMultiByte(dwCodePage, 0, m_kszOrgCode, -1, m_szConvCode, m_iMaxCodeSize, NULL, NULL);

    if (m_iCodeSize) {
       return TRUE;
    }
    else {
        TCHAR szErrorBuf[64] = {0};
        _sntprintf_s(szErrorBuf, 64, 64, TEXT("Convert MultiByte to WideChar NG! ErrorCode:%d\n"), ::GetLastError());
        ::OutputDebugString(szErrorBuf);
        return FALSE;
    }
#endif // UNICODE
}

INT Ant_Decode::getSize()
{
    return m_iCodeSize;
}

CHAR* Ant_Decode::get()
{
    return m_szConvCode;
}

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
/* EOF */
