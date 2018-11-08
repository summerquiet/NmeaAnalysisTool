/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#include "stdafx.h"

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_STRING_H
#   include "Ant_String.h"
#endif
#ifndef ANT_CONVERTER_H
#   include "Ant_Converter.h"
#endif
#include <stdio.h>
#include <stdarg.h>

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Value define

#ifndef MIN
#    define MIN(X,Y) (((X)<(Y))? (X):(Y))
#endif
static const WCHAR BOM_SAME_ENDIAN = 0xfeff;
static const WCHAR BOM_ANTI_ENDIAN = 0xfffe;

static const XCHAR ANT_NULL_STRING[] = XTEXT("");

/*---------------------------------------------------------------------------*/
// Functions
BOOL judgeSpace_Forward(const XCHAR* chVal, USHORT& chLen)
{
    if (chVal == NULL) {
        return FALSE;
    }

    chLen = 1;
    if (xisspace(*chVal)) {
        return TRUE;
    }
    /*if (*chVal == 'x' && *(chVal+1) == 'y') {
        chLen = 2;
        return TRUE;
    }*/
    return FALSE;
}

BOOL judgeSpace_Backward(const XCHAR* chVal, USHORT& chLen)
{
    if (chVal == NULL) {
        return FALSE;
    }

    chLen = 1;
    if (xisspace(*chVal)) {
        return TRUE;
    }
    /*if (*chVal == 'y' && *(chVal-1) == 'x') {
        chLen = 2;
        return TRUE;
    }*/
    return FALSE;
}

VOID word2big(unsigned char* buff, unsigned short x)
{
    buff[1] = x&0xFF;
    buff[0] = x>>8;
}

XCHAR* Ant_String::strAlloc(size_t len)
{
    if (len < 1) return NULL;
    XCHAR* buf = new XCHAR[len];
    if (NULL == buf)
    {
        return NULL;
    }
    buf[0] = XTEXT('\0');

    //TCHAR szBuf[56];
    //memset(szBuf, 0, 56*sizeof(TCHAR));
    //_stprintf(szBuf, L"new - %p - size %d\n", buf, len);
    //OutputDebugString(szBuf);

    return buf;
}

VOID Ant_String::strFree(XCHAR* p, size_t len)
{
    if (ANT_NULL_STRING == p) return;
    //TCHAR szBuf[56];
    //memset(szBuf, 0, 56*sizeof(TCHAR));
    //_stprintf(szBuf, L"del - %p - size %d\n", p, len);
    //OutputDebugString(szBuf);
    delete[] p;
}

XCHAR* Ant_String::strRealloc(XCHAR* p, size_t org_len, size_t new_len)
{
    strFree(p, org_len);
    return strAlloc(new_len);
}

Ant_String::Ant_String()
: m_szStrBuff(NULL), m_nStrLen(0)
{
    releaseStrBuff();
}

Ant_String::Ant_String(const Ant_String& rhs)
: m_szStrBuff(NULL), m_nStrLen(0)
{
    if (rhs.isNullBuffer()) {
        releaseStrBuff();
    }
    else {
        set(rhs.getString(), rhs.getLength());
    }
}

Ant_String::Ant_String(const XCHAR* p)
: m_szStrBuff(NULL), m_nStrLen(0)
{
    releaseStrBuff();
    set(p);
}

Ant_String::Ant_String(const XCHAR* p, INT l)
: m_szStrBuff(NULL), m_nStrLen(0)
{
    releaseStrBuff();
    set(p, l);
}

#ifndef ANT_STRING_UNICODE
Ant_String::Ant_String(const WCHAR* p)
: m_szStrBuff(NULL), m_nStrLen(0)
{
    releaseStrBuff();
    set(p);
}

Ant_String::Ant_String(const WCHAR* p, INT l)
: m_szStrBuff(NULL), m_nStrLen(0)
{
    releaseStrBuff();
    set(p, l);
}
#endif

Ant_String::Ant_String(DWORD dwCodePage, const CHAR* p)
: m_szStrBuff(NULL), m_nStrLen(0)
{
    releaseStrBuff();
    set(dwCodePage, p);
}

Ant_String::Ant_String(DWORD dwCodepage, const CHAR* p, INT l)
: m_szStrBuff(NULL), m_nStrLen(0)
{
    releaseStrBuff();
    set(dwCodepage, p, l);
}

Ant_String::~Ant_String()
{
    strFree(m_szStrBuff, m_nStrLen+1);
    m_szStrBuff = NULL;
    m_nStrLen = 0;
}

BOOL Ant_String::isNullBuffer() const
{
    if (m_szStrBuff == NULL || m_szStrBuff == ANT_NULL_STRING) {
        return TRUE;
    }
    return FALSE;
}

VOID Ant_String::releaseStrBuff()
{
    strFree(m_szStrBuff, m_nStrLen+1);
    m_szStrBuff = const_cast<XCHAR*>(ANT_NULL_STRING);
    m_nStrLen = 0;
}

Ant_String& Ant_String::operator = (const Ant_String& rhs)
{
    if (this != &rhs) {
        if (rhs.isNullBuffer()) {
            releaseStrBuff();
        }
        else {
            set(rhs.getString(), rhs.getLength());
        }
    }
    else {
        // NOP
    }
    return *this;
}

Ant_String& Ant_String::operator += (const Ant_String& rhs)
{
    add(rhs.getString());
    return *this;
}

BOOL Ant_String::set(const XCHAR* p)
{
    if (p == NULL) {
        releaseStrBuff();
        return TRUE;
    }
    return set(p, xcslen(p));
}

BOOL Ant_String::set(const XCHAR* p, INT l)
{
    if (p == NULL || l < 0) {
        releaseStrBuff();
        return TRUE;
    }

    if (l == m_nStrLen && !isNullBuffer() && m_szStrBuff != p) {
        memcpy(m_szStrBuff, p, m_nStrLen*sizeof(XCHAR));
        m_szStrBuff[m_nStrLen] = XTEXT('\0');
    }
    else {
        if ((size_t)l > xcslen(p)) {
            l = xcslen(p);
        }
        XCHAR* szBuff = strAlloc(l + 1);
        if (NULL == szBuff) {
            return FALSE;
        }
        memcpy(szBuff, p, l*sizeof(XCHAR));
        szBuff[l] = XTEXT('\0');
        releaseStrBuff();
        m_szStrBuff = szBuff;
        m_nStrLen = l;
    }

    return TRUE;
}

#ifndef ANT_STRING_UNICODE
BOOL Ant_String::set(const WCHAR* p)
{
    if (p == NULL) {
        return set((XCHAR*)NULL);
    }
    return set(p, wcslen(p));
}

BOOL Ant_String::set(const WCHAR* p, INT l)
{
    if (p == NULL || l < 0) {
        return set((XCHAR*)NULL, l);
    }
    //transfer WCHAR to UTF8
    XCHAR* szTemp = strAlloc(MAX_TRANSLATE_SIZE);
    if (NULL == szTemp) return FALSE;

    Ant_Converter cConverter;
    INT len = cConverter.wcharToUTF8(p, l, szTemp, MAX_TRANSLATE_SIZE);

    if (MAX_TRANSLATE_SIZE <= (DWORD)len) {
        szTemp = strRealloc(szTemp, MAX_TRANSLATE_SIZE, len+1);
        if (NULL == szTemp) return FALSE;

        cConverter.wcharToUTF8(p, l, szTemp, len+1);

        szTemp[len] = XTEXT('\0');
        BOOL bRes = set(szTemp);
        strFree(szTemp, len+1);
        return bRes;
    }
    else {
        szTemp[len] = XTEXT('\0');
        BOOL bRes = set(szTemp);
        strFree(szTemp, MAX_TRANSLATE_SIZE);
        return bRes;
    }
}
#endif // ANT_STRING_UNICODE

BOOL Ant_String::set(DWORD dwCodePage, const CHAR* p)
{
    if (p == NULL) {
        return set((XCHAR*)NULL);
    }
    if (dwCodePage == ANT_CP_UTF16) {
        return setUTF16((UCHAR16*)p);
    }
#ifndef ANT_STRING_UNICODE // UTF-8
    if (dwCodePage == ANT_CP_UTF8) {
        return set(p);
    }
#endif
    return set(dwCodePage, p, strlen(p));
}

BOOL Ant_String::set(DWORD dwCodePage, const CHAR* p, INT l)
{
    if (p == NULL) {
        return set((XCHAR*)NULL, l);
    }
    else if (l <= 0) {
        return set(XTEXT(""), l);
    }

    if (dwCodePage == ANT_CP_UTF16) {
        return setUTF16((UCHAR16*)p, l/sizeof(UCHAR16));
    }

    // Normally, strings are not that long. We choose a fast way to deal with it.
    // But we also deal with long strings. It may slower than ordinary way.
    // Yet, totally speaking, this could be faster.

#ifdef ANT_STRING_UNICODE  // UTF-16
    XCHAR* szTemp = strAlloc(MAX_TRANSLATE_SIZE);
    if (NULL == szTemp) return FALSE;

    Ant_Converter cConverter;
    INT len = cConverter.charToUTF16(dwCodePage, p, l, szTemp, MAX_TRANSLATE_SIZE);

    if (len == 0 || MAX_TRANSLATE_SIZE <= (DWORD)len) {
        len = cConverter.charToUTF16(dwCodePage, p, l, 0, 0);
        if (0 == len) return FALSE;
        szTemp = strRealloc(szTemp, MAX_TRANSLATE_SIZE, len + 1);
        if (NULL == szTemp) return FALSE;

        cConverter.charToUTF16(dwCodePage, p, l, szTemp, len+1);

        szTemp[len] = XTEXT('\0');
        BOOL bRes = set(szTemp);
        strFree(szTemp, len+1);
        return bRes;
    }
    else{
        szTemp[len] = XTEXT('\0');
        BOOL bRes = set(szTemp);
        strFree(szTemp, MAX_TRANSLATE_SIZE);
        return bRes;
    }

#else  //transfer appointed codepage to UTF-8
    if (dwCodePage == ANT_CP_UTF8) {
        return set(p, l);
    }
    XCHAR* szTemp = strAlloc(MAX_TRANSLATE_SIZE);
    if (NULL == szTemp) return FALSE;

    Ant_Converter cConverter;
    INT len = cConverter.charToUTF8(dwCodePage, p, l, szTemp, MAX_TRANSLATE_SIZE);

    if (MAX_TRANSLATE_SIZE <= (DWORD)len) {
        szTemp = strRealloc(szTemp, MAX_TRANSLATE_SIZE, len+1);
        if (NULL == szTemp) return FALSE;

        cConverter.charToUTF8(dwCodePage, p, l, szTemp, len+1);

        szTemp[len] = XTEXT('\0');
        BOOL bRes = set(szTemp);
        strFree(szTemp, len+1);
        return bRes;
    }
    else {
        szTemp[len] = XTEXT('\0');
        BOOL bRes = set(szTemp);
        strFree(szTemp, MAX_TRANSLATE_SIZE);
        return bRes;
    }
#endif // ANT_STRING_UNICODE
}

BOOL Ant_String::setUTF16(const UCHAR16* p)
{
#ifdef COMP_OPT_LITTLE_ENDIAN
    return setUTF16LE(p);
#else
    return setUTF16BE(p);
#endif
}

BOOL Ant_String::setUTF16(const UCHAR16* p, INT l)
{
#ifdef COMP_OPT_LITTLE_ENDIAN
    return setUTF16LE(p, l);
#else
    return setUTF16BE(p, l);
#endif
}

INT Ant_String::utf16Strlen(const UCHAR16* p) const
{
    if (NULL == p) {
        return 0;
    }
#if defined(_WIN32) || defined(_WIN32_WCE)
    return wcslen(p);
#else
    const UCHAR16* pCounter = p;
    INT l = 0;
    while (0 != *pCounter)    {
        l++;
        pCounter++;
    }
    return l;
#endif
}

VOID Ant_String::switchEndian(UCHAR16 *pBuffer, INT nLength) const
{
    if (NULL == pBuffer) {
        return;
    }
    for(INT nCount = 0; nCount < nLength; ++nCount) {
        word2big((unsigned char*)(pBuffer+nCount), ((unsigned short*)pBuffer)[nCount]);
    }
}

BOOL Ant_String::setUTF16BE(const UCHAR16* p)
{
    if (NULL == p) {
        return set((XCHAR*)NULL);
    }
    return setUTF16BE(p, utf16Strlen(p));
}

BOOL Ant_String::setUTF16BE(const UCHAR16* p, INT l)
{
    if (p == NULL || l < 0) {
        return set((XCHAR*)NULL, l);
    }
#ifdef ANT_STRING_UNICODE
    set(p, l);

#ifdef COMP_OPT_LITTLE_ENDIAN
    SwitchEndian(m_szStrBuff, l);
#endif

    return TRUE;

#else // UTF_16BE -> UTF8
    XCHAR* szTemp = strAlloc(MAX_TRANSLATE_SIZE);
    if (NULL == szTemp) {
        return FALSE;
    }

    Ant_Converter cConverter;
    INT len = cConverter.utf16BEToUTF8(p, l, szTemp, MAX_TRANSLATE_SIZE);

    if (MAX_TRANSLATE_SIZE <= (DWORD)len) {
        szTemp = strRealloc(szTemp, MAX_TRANSLATE_SIZE, len+1);
        if (NULL == szTemp) return FALSE;

        cConverter.utf16BEToUTF8(p, l, szTemp, len+1);
        szTemp[len] = XTEXT('\0');
        BOOL bRes = set(szTemp);
        strFree(szTemp, MAX_TRANSLATE_SIZE);
        return bRes;
    }
    else {
        szTemp[len] = XTEXT('\0');
        BOOL bRes = set(szTemp);
        strFree(szTemp, MAX_TRANSLATE_SIZE);
        return bRes;
    }
#endif // ANT_STRING_UNICODE
}

BOOL Ant_String::setUTF16LE(const UCHAR16* p)
{
    if (NULL == p) {
        return set((XCHAR*)NULL);
    }
    return setUTF16LE(p, utf16Strlen(p));
}

BOOL Ant_String::setUTF16LE(const UCHAR16* p, INT l)
{
    if (p == NULL || l < 0) {
        return set((XCHAR*)NULL, l);
    }
#ifdef ANT_STRING_UNICODE
    set(p, l);

#ifndef COMP_OPT_LITTLE_ENDIAN
    SwitchEndian(m_szStrBuff, l);
#endif

    return TRUE;

#else // UTF_16LE -> UTF8
    XCHAR* szTemp = strAlloc(MAX_TRANSLATE_SIZE);
    if (NULL == szTemp) {
        return FALSE;
    }

    Ant_Converter cConverter;
    INT len = cConverter.utf16LEToUTF8(p, l, szTemp, MAX_TRANSLATE_SIZE);

    if (MAX_TRANSLATE_SIZE <= (DWORD)len) {
        szTemp = strRealloc(szTemp, MAX_TRANSLATE_SIZE, len+1);
        if (NULL == szTemp) return FALSE;

        cConverter.utf16LEToUTF8(p, l, szTemp, len+1);
        szTemp[len] = XTEXT('\0');
        BOOL bRes = set(szTemp);
        strFree(szTemp, MAX_TRANSLATE_SIZE);
        return bRes;
    }
    else {
        szTemp[len] = XTEXT('\0');
        BOOL bRes = set(szTemp);
        strFree(szTemp, MAX_TRANSLATE_SIZE);
        return bRes;
    }
    return FALSE;
#endif // ANT_STRING_UNICODE
}

BOOL Ant_String::setUTF16BOM(const UCHAR16* p)
{
    if (NULL == p) {
        return set((XCHAR*)NULL);
    }
    return setUTF16BOM(p, utf16Strlen(p));
}

BOOL Ant_String::setUTF16BOM(const UCHAR16* p, INT l)
{
    if (p == NULL || l < 0) {
        return set((XCHAR*)NULL, l);
    }
    if (BOM_ANTI_ENDIAN== *p) {
#ifdef COMP_OPT_LITTLE_ENDIAN
        return setUTF16BE(p+1, l - 1);
#else
        return setUTF16LE(p+1, l - 1);
#endif
    }
    else if (BOM_SAME_ENDIAN == *p) {
#ifdef COMP_OPT_LITTLE_ENDIAN
        return setUTF16LE(p+1, l - 1);
#else
        return setUTF16BE(p+1, l - 1);
#endif
    }
    else{}

    return FALSE;
}

INT Ant_String::getString(XCHAR* p, INT l) const
{
    if (isNullBuffer() || m_nStrLen < 0 || l < 0) {
        return 0;
    }
    if (p == NULL) {
        return m_nStrLen+1;
    }
    if (p != NULL && l > 0) {
        memcpy(p, m_szStrBuff, MIN(m_nStrLen + 1, l) * sizeof(XCHAR));
        p[l - 1] = XTEXT('\0');
    }
    if (l < m_nStrLen+1) return 0;
    return m_nStrLen+1;
}

#ifndef ANT_STRING_UNICODE
INT Ant_String::getString(WCHAR* p, INT l) const
{
    if (isNullBuffer() || m_nStrLen < 0 || l < 0) {
        return 0;
    }
    Ant_Converter cConverter;
    INT len = cConverter.utf8ToWChar(m_szStrBuff, -1, p, l);
    if (p != NULL && l > 0) {
        p[l-1] = L'\0';
        if (l < len) return 0;
    }
    else if (p != NULL && l == 0) {
        return 0;//for windowsAPI return 0 in this situation.
    }
    return len;
}
#endif // ANT_STRING_UNICODE

INT Ant_String::getString(DWORD dwCodePage, CHAR* p, INT l) const
{
    if (isNullBuffer() || m_nStrLen < 0 || l < 0) {
        return 0;
    }

    if (dwCodePage == ANT_CP_UTF16) {
        return getUTF16((UCHAR16*)p, l/sizeof(UCHAR16));
    }

    Ant_Converter cConverter;
#ifdef ANT_STRING_UNICODE // UTF-16
    INT len = cConverter.utf16ToChar(dwCodePage, m_szStrBuff, m_nStrLen+1, p, l);
    if (p != NULL && l > 0) {
        p[l-1]= '\0';
    }
#else  // UTF-8 to appointed code page
    if (dwCodePage == ANT_CP_UTF8) {
        return getString(p, l);
    }
    INT len = cConverter.utf8ToChar(dwCodePage, m_szStrBuff, m_nStrLen+1, p, l);
    if (p != NULL && l > 0) {
        p[l-1] = '\0';
        if (l < len) return 0;
    }
    else if (p != NULL && l == 0) {
        return 0;//for windowsAPI return 0 in this situation.
    }
#endif // ANT_STRING_UNICODE
    return len;
}

INT Ant_String::getStringLength(DWORD dwCodePage) const
{
    if (isNullBuffer() || m_nStrLen < 0) {
        return 0;
    }
    Ant_Converter cConverter;
#ifdef ANT_STRING_UNICODE // UTF-16
    if (dwCodePage == ANT_CP_UTF16) {
        return getLength();
    }
    INT len = cConverter.utf16ToChar(dwCodePage, m_szStrBuff, m_nStrLen+1, NULL, 0);
#else  // UTF-8 to appointed code page
    if (dwCodePage == ANT_CP_UTF8) {
        return getLength();
    }
    else if (dwCodePage == ANT_CP_UTF16) {
        return cConverter.utf8ToUTF16BE(m_szStrBuff, m_nStrLen+1, NULL, 0);
    }
    INT len = cConverter.utf8ToChar(dwCodePage, m_szStrBuff, m_nStrLen+1, NULL, 0);
#endif // ANT_STRING_UNICODE
    return len;
}

INT Ant_String::getUTF16(UCHAR16* p, INT l) const
{
#ifdef COMP_OPT_LITTLE_ENDIAN
    return getUTF16LE(p, l);
#else
    return getUTF16BE(p, l);
#endif
}

INT Ant_String::getUTF16BE(UCHAR16 *p, INT l) const
{
    if (isNullBuffer() || m_nStrLen < 0 || l < 0) {
        return 0;
    }
#ifdef ANT_STRING_UNICODE
    if (0 == p || 0 == l) {
        return m_nStrLen+1;
    }
#ifdef COMP_OPT_LITTLE_ENDIAN
    INT nLength = getString(p, l);
    if (0 >= nLength) return 0;
    SwitchEndian(p, nLength);
    p[nLength-1] = XTEXT('\0');
    return nLength;
#else
    return getString(p, l);
#endif
#else // ANT_STRING_UNICODE
    Ant_Converter cConverter;
    INT len = cConverter.utf8ToUTF16BE(m_szStrBuff, m_nStrLen+1, p, l);
    return len;
#endif // ANT_STRING_UNICODE
}

INT Ant_String::getUTF16LE(UCHAR16 *p, INT l) const
{
    if (isNullBuffer() || m_nStrLen < 0 || l < 0) {
        return 0;
    }
#ifdef ANT_STRING_UNICODE
    if (0 == p || 0 == l) {
        return m_nStrLen+1;
    }
#ifndef COMP_OPT_LITTLE_ENDIAN
    INT nLength = getString(p, l);
    if (0 >= nLength) return 0;
    SwitchEndian(p, nLength);
    p[nLength-1] = XTEXT('\0');
    return nLength;
#else
    return getString(p, l);
#endif
#else
    Ant_Converter cConverter;
    INT len = cConverter.utf8ToUTF16LE(m_szStrBuff, m_nStrLen+1, p, l);
    return len;
#endif
}

INT Ant_String::getUTF16BOM(UCHAR16 *p, INT l) const
{
    if (isNullBuffer() || m_nStrLen < 0 || l < 0) {
        return 0;
    }
    static Ant_String BOM(&BOM_SAME_ENDIAN, 1);
    Ant_String strWithBom = BOM + *this;
    return strWithBom.getUTF16(p, l);
}

BOOL Ant_String::add(const XCHAR* p)
{
    if (p == NULL) {
        return FALSE;
    }
    return add(p, xcslen(p));
}

BOOL Ant_String::add(const XCHAR* p, INT l)
{
    if (p == NULL || l <= 0) {
        return FALSE;
    }
    if (isNullBuffer() || m_nStrLen <= 0) {
        return set(p, l);
    }
    XCHAR* szTemp = m_szStrBuff;
    INT nOriLen = xcslen(m_szStrBuff);
    INT nAddLen = MIN(xcslen(p), (size_t)l);
    m_szStrBuff = strAlloc(nOriLen+nAddLen+1);
    if (isNullBuffer()) {
        m_szStrBuff = szTemp;
        return FALSE;
    }
    memcpy(m_szStrBuff, szTemp, nOriLen*sizeof(XCHAR));
    memcpy(m_szStrBuff+nOriLen, p, nAddLen*sizeof(XCHAR));
    m_szStrBuff[nOriLen+nAddLen] = XTEXT('\0');
    m_nStrLen = nOriLen+nAddLen;
    delete[] szTemp;
    return TRUE;
}

#ifndef ANT_STRING_UNICODE
BOOL Ant_String::add(const WCHAR* p)
{
    if (p == NULL) {
        return FALSE;
    }
    return add(p, wcslen(p));
}

BOOL Ant_String::add(const WCHAR* p, INT l)
{
    if (p == NULL || l <= 0) {
        return FALSE;
    }
    if (isNullBuffer() || m_nStrLen <= 0) {
        return set(p, l);
    }
    Ant_String strTemp(p, l);
    return add(strTemp.getString(), strTemp.getLength());
}
#endif

BOOL Ant_String::add(DWORD dwCodePage, const CHAR* p)
{
    if (p == NULL) {
        return FALSE;
    }
    return add(dwCodePage, p, strlen(p));
}

BOOL Ant_String::add(DWORD dwCodePage, const CHAR* p, INT l)
{
    Ant_String strTemp(dwCodePage, p, l);
    return add(strTemp.getString(), strTemp.getLength());
}

DWORD Ant_String::getSerializeSize() const
{
    DWORD dwSize = 0;

    dwSize += static_cast<DWORD>(sizeof(INT));

    dwSize += static_cast<DWORD>((getLength() + 1) * sizeof(XCHAR));

    return dwSize;
}

static VOID readDWORDString(const BYTE* bpSrc, DWORD& rdwDst)
{
    memcpy(&rdwDst, bpSrc, sizeof(DWORD));    // BuffChk_0003
}

static VOID writeDWORDString(BYTE* bpSrc, DWORD dwDst)
{
    memcpy(bpSrc, &dwDst, sizeof(DWORD));    // BuffChk_0004
}

BOOL Ant_String::serialize(BYTE* const buffer, const DWORD size) const
{
    if (NULL == buffer) {
        return FALSE;
    }
    if (size < Ant_String::getSerializeSize()) {
        return FALSE;
    }

    INT nLength = Ant_String::getLength();
    writeDWORDString(buffer, static_cast<DWORD>(nLength));
    INT nIndex = sizeof(INT);

    getString(reinterpret_cast<XCHAR*>(buffer + nIndex), (nLength + 1));

    return TRUE;
}

BOOL Ant_String::deserialize(const BYTE* const buffer)
{
    if (NULL == buffer) {
        return FALSE;
    }

    DWORD dwLength_Temp = 0;
    readDWORDString( buffer, dwLength_Temp);
    INT nLength = static_cast<INT>(dwLength_Temp);
    INT nIndex = sizeof(INT);

    const XCHAR* pchString = reinterpret_cast<const XCHAR*>(buffer + nIndex);

    set(pchString, nLength);

    return TRUE;
}

const XCHAR* Ant_String::getString() const
{
    return m_szStrBuff;
}

INT Ant_String::getLength() const
{
    return m_nStrLen;
}

Ant_String::operator const XCHAR*() const
{
    return getString();
}

BOOL Ant_String::operator == (const Ant_String& lhr) const
{
    if (this->m_nStrLen != lhr.m_nStrLen) return FALSE;
    if (lhr.m_nStrLen == 0) return TRUE;

    return 0 == xcsncmp(this->m_szStrBuff, lhr.m_szStrBuff, lhr.m_nStrLen);
}

BOOL Ant_String::operator != (const Ant_String& lhr) const
{
    return !Ant_String::operator ==(lhr);
}

BOOL Ant_String::format(INT maxlen, const XCHAR* format, ...)
{
    if (format == NULL) {
        return FALSE;
    }
    if (xcslen(format) == 0 || maxlen <= 0) {
        return set(XTEXT(""), 0);
    }
    XCHAR* szTemp = strAlloc(maxlen);
    if (szTemp == NULL) {
        return FALSE;
    }
    memset(szTemp, 0, maxlen*sizeof(XCHAR));
    va_list list;
    va_start(list, format);
#ifdef ANT_STRING_UNICODE
    INT nRes = _vsnwprintf(szTemp, maxlen-1, format, list);
#else
    INT nRes = _vsnprintf(szTemp, maxlen-1, format, list);
#endif
    va_end(list);
    set(szTemp, xcslen(szTemp));
    strFree(szTemp, maxlen);
    return TRUE;
}

BOOL Ant_String::format(const XCHAR* format, ...)
{
    if (format == NULL) {
        return FALSE;
    }
    if (xcslen(format) == 0) {
        return set(XTEXT(""), 0);
    }
    INT nMaxLen = xcslen(format) * 2 + 1;
    while (1) {
        va_list list;
        va_start(list, format);
        XCHAR* szTemp = strAlloc(nMaxLen);
        if (szTemp == NULL) return FALSE;
        /* Try to print in the allocated space. */
#ifdef ANT_STRING_UNICODE
        INT nRes = _vsnwprintf(szTemp, nMaxLen, format, list);
#else
        INT nRes = _vsnprintf(szTemp, nMaxLen, format, list);
#endif
        va_end(list);
        /* If that worked, return the string. */
        if (nRes > -1 && nRes < nMaxLen) {
            set(szTemp, nRes);
            strFree(szTemp, nMaxLen);
            return TRUE;
        }
        strFree(szTemp, nMaxLen);
        /* Else try again with more space. */
        if (nRes > -1) {        /* glibc 2.1 */
            nMaxLen = nRes + 1;    /* precisely what is needed */
        }
        else {                /* glibc 2.0 */
            nMaxLen *= 2;        /* twice the old size */
        }
    }
    return FALSE;
}

BOOL Ant_String::upperCase()
{
    if (isNullBuffer() || m_nStrLen <= 0) {
        return TRUE;
    }
#if defined(_WIN32) || defined(_WIN32_WCE)
    XCHAR* szRes = xcsupr(m_szStrBuff);
    return (NULL!=szRes)?TRUE:FALSE;
#else //_LINUX
    for (INT i = 0; i < m_nStrLen; i++) {
        if (m_szStrBuff[i] >= 'a' && m_szStrBuff[i] <= 'z') {
            m_szStrBuff[i] = m_szStrBuff[i] - 'a' + 'A';
        }
    }
    return TRUE;
#endif //_LINUX
}

BOOL Ant_String::lowerCase()
{
    if (isNullBuffer() || m_nStrLen <= 0) {
        return TRUE;
    }
#if defined(_WIN32) || defined(_WIN32_WCE)
    XCHAR* szRes = xcslwr(m_szStrBuff);
    return (NULL!=szRes)?TRUE:FALSE;
#else //_LINUX
    for (INT i = 0; i < m_nStrLen; i++) {
        if (m_szStrBuff[i] >= 'A' && m_szStrBuff[i] <= 'Z') {
            m_szStrBuff[i] = m_szStrBuff[i] - 'A' + 'a';
        }
    }
    return TRUE;
#endif //_LINUX
}

BOOL Ant_String::replace(INT pos, XCHAR ch)
{
    if (isNullBuffer() || m_nStrLen <= 0) {
        return TRUE;
    }
    if (pos < 0 || pos >= m_nStrLen) {
        return FALSE;
    }
    m_szStrBuff[pos] = ch;
    if (XTEXT('\0') == ch) {
        m_nStrLen = pos;
    }
    return TRUE;
}

BOOL Ant_String::trimLeft()
{
    return trimLeftEx(judgeSpace_Forward);
}

BOOL Ant_String::trimRight()
{
    return trimRightEx(judgeSpace_Backward);
}

BOOL Ant_String::trim()
{
    return trimEx(judgeSpace_Forward, judgeSpace_Backward);
}

BOOL Ant_String::trimLeftEx(Judger pfunc_forward)
{
    if (pfunc_forward == NULL) {
        return FALSE;
    }
    if (isNullBuffer() ||  m_nStrLen <= 0) {
        return TRUE;
    }
    const XCHAR* pStart = m_szStrBuff;
    USHORT chLen = 1;
    while (pfunc_forward(pStart, chLen) && (pStart <= m_szStrBuff+m_nStrLen)) pStart+=chLen;
    if (pStart == m_szStrBuff) {
        return TRUE;
    }
    else if (pStart > m_szStrBuff + m_nStrLen) {
        return FALSE;
    }
    return set(pStart);
}

BOOL Ant_String::trimRightEx(Judger pfunc_backward)
{
    if (pfunc_backward == NULL) {
        return FALSE;
    }
    if (isNullBuffer() ||  m_nStrLen <= 0) {
        return TRUE;
    }
    const XCHAR* pEnd = m_szStrBuff + m_nStrLen - 1;
    USHORT chLen = 1;
    while (pfunc_backward(pEnd, chLen) && (pEnd >= m_szStrBuff)) pEnd-=chLen;

    if (pEnd == m_szStrBuff + m_nStrLen - 1) {
        return TRUE;
    }
    else if (pEnd < m_szStrBuff - 1) {
        return FALSE;
    }

    m_szStrBuff[pEnd - m_szStrBuff + 1] = XTEXT('\0');
    return TRUE;
}

BOOL Ant_String::trimEx(Judger pfunc_forward, Judger pfunc_backward)
{
    if (pfunc_forward == NULL || pfunc_backward == NULL) {
        return FALSE;
    }
    if (isNullBuffer() ||  m_nStrLen <= 0) {
        return TRUE;
    }
    const XCHAR* pStart = m_szStrBuff;
    const XCHAR* pEnd = m_szStrBuff + m_nStrLen - 1;
    USHORT chLen = 1;
    while (pfunc_forward(pStart, chLen) && (pStart <= m_szStrBuff+m_nStrLen)) pStart+=chLen;
    while (pfunc_backward(pEnd, chLen) && (pEnd >= m_szStrBuff)) pEnd-=chLen;

    if ((pStart == m_szStrBuff) && (pEnd == m_szStrBuff + m_nStrLen - 1)) {
        return TRUE;
    }
    else if (pStart > pEnd) {
        return set(XTEXT(""));
    }

    return set(pStart, pEnd - pStart + 1);
}

BOOL Ant_String::endWith(XCHAR ch) const
{
    if (isNullBuffer() ||  m_nStrLen <= 0) {
        return FALSE;
    }

    return (m_szStrBuff[m_nStrLen - 1] == ch) ? TRUE : FALSE;
}

XCHAR Ant_String::operator[](INT offset) const
{
    if (isNullBuffer() || m_nStrLen < 0) {
        return 0;
    }
    if (offset < 0 || offset > m_nStrLen) {
        return 0;
    }
    return m_szStrBuff[offset];
}

Ant_String Ant_String::left(INT len) const
{
    Ant_String strDes;
    if (isNullBuffer() || m_nStrLen < 0) {
        return strDes;
    }
    if (len < 0) {
        return strDes;
    }
    if (len > m_nStrLen) {
        len = m_nStrLen;
    }
    strDes.set(m_szStrBuff, len);
    return strDes;
}

Ant_String Ant_String::right(INT len) const
{
    Ant_String strDes;
    if (isNullBuffer() || m_nStrLen < 0) {
        return strDes;
    }
    if (len < 0) {
        return strDes;
    }
    INT first = m_nStrLen - len;
    if (first < 0) {
        first = 0;
    }
    strDes.set(m_szStrBuff + first);
    return strDes;
}

BOOL Ant_String::takeLeft(INT len)
{
    if (isNullBuffer() || m_nStrLen <= 0) {
        return TRUE;
    }
    if (len <= 0) {
        releaseStrBuff();
        return TRUE;
    }
    if (len >= m_nStrLen) {
        return TRUE;
    }
    m_szStrBuff[len] = XTEXT('\0');
    m_nStrLen = len;
    return TRUE;
}

BOOL Ant_String::takeRight(INT len)
{
    if (isNullBuffer() || m_nStrLen <= 0) {
        return TRUE;
    }
    if (len <= 0) {
        releaseStrBuff();
        return TRUE;
    }
    if (len >= m_nStrLen) {
        return TRUE;
    }
    return set(m_szStrBuff + m_nStrLen - len);
}

Ant_String Ant_String::mid(INT first) const
{
    Ant_String strDes;
    if (isNullBuffer() || m_nStrLen < 0) {
        return strDes;
    }
    if (first < 0 || first > m_nStrLen) {
        return strDes;
    }
    strDes.set(m_szStrBuff + first);
    return strDes;
}

Ant_String Ant_String::mid(INT first, INT len) const
{
    Ant_String strDes;
    if (isNullBuffer() || m_nStrLen < 0) {
        return strDes;
    }
    if (first < 0 || first > m_nStrLen || len < 0) {
        return strDes;
    }
    if (len > m_nStrLen - first) {
        len = m_nStrLen - first;
    }
    strDes.set(m_szStrBuff + first, len);
    return strDes;
}

BOOL Ant_String::takeMid(INT first)
{
    if (isNullBuffer() ||  m_nStrLen <= 0) {
        return TRUE;
    }
    if (first < 0 || first > m_nStrLen) {
        releaseStrBuff();
        return TRUE;
    }
    return set(m_szStrBuff + first);
}

BOOL Ant_String::takeMid(INT first, INT len)
{
    if (isNullBuffer() ||  m_nStrLen <= 0) {
        return TRUE;
    }
    if (first < 0 || first > m_nStrLen || len < 0) {
        releaseStrBuff();
        return TRUE;
    }
    if (len > m_nStrLen - first) {
        len = m_nStrLen - first;
    }
    return set(m_szStrBuff + first, len);
}

INT Ant_String::replace(const XCHAR* src, const XCHAR* des)
{
    if (isNullBuffer() ||  m_nStrLen <= 0) {
        return 0;
    }
    if (NULL == src || NULL == des) {
        return 0;
    }
    INT nSrcLen = xcslen(src);
    INT nDesLen = xcslen(des);
    if (nSrcLen <= 0 || nSrcLen > m_nStrLen) {
        return 0;
    }
    INT nReplaceCount = 0;
    XCHAR* find_pos = m_szStrBuff;
    while (*find_pos != XTEXT('\0') && find_pos <= m_szStrBuff+m_nStrLen) {
        if (0 == xcsncmp(find_pos, src, nSrcLen)) {
            if (nSrcLen == nDesLen) {
                memcpy(find_pos, des, nSrcLen*sizeof(XCHAR));
                find_pos += nSrcLen;
            }
            else {
                INT l = m_nStrLen + nDesLen - nSrcLen;
                XCHAR* strBuff = strAlloc(l + 1);
                if (NULL == strBuff)
                {
                    return 0;
                }
                INT nPreLen = find_pos-m_szStrBuff;
                INT nLeftLen = m_nStrLen-nPreLen-nSrcLen;
                memcpy(strBuff, m_szStrBuff, nPreLen*sizeof(XCHAR));
                memcpy(strBuff+nPreLen, des, nDesLen*sizeof(XCHAR));
                memcpy(strBuff+nPreLen+nDesLen, find_pos+nSrcLen, nLeftLen*sizeof(XCHAR));
                strBuff[l] = XTEXT('\0');
                releaseStrBuff();
                m_szStrBuff = strBuff;
                m_nStrLen = l;
                find_pos = m_szStrBuff+nPreLen+nDesLen;
            }
            nReplaceCount++;
        }
        else {
            find_pos++;
        }
    }
    return nReplaceCount;
}

INT Ant_String::find(const XCHAR* substr) const
{
    return find(substr, 0);
}

INT Ant_String::find(const XCHAR* substr, INT start) const
{
    if (isNullBuffer() ||  m_nStrLen <= 0) {
        return -1;
    }
    if (start >= m_nStrLen || NULL == substr) {
        return -1;
    }
    INT find_len = xcslen(substr);
    if (find_len <= 0 || find_len > m_nStrLen) {
        return -1;
    }
    const XCHAR* find_pos = m_szStrBuff + start;
    while (*find_pos != XTEXT('\0')) {
        if (0 == xcsncmp(find_pos, substr, find_len)) {
            return find_pos-m_szStrBuff;
        }
        find_pos++;
    }
    return -1;
}

Ant_String* Ant_String::clone()
{
    Ant_String* strDes = new Ant_String(*this);
    return strDes;
}

VOID Ant_String::copy(const Ant_String* src)
{
    if (src == NULL) {
        releaseStrBuff();
        return;
    }
    *this = *src;
}

const Ant_String operator + (const Ant_String& lhs, const Ant_String& rhs)
{
    Ant_String res(lhs);
    res += rhs;
    return res;
}

BOOL operator == (const Ant_String& lhs, const XCHAR* rhs)
{
    return lhs.operator ==(Ant_String(rhs));
}

BOOL operator != (const Ant_String& lhs, const XCHAR* rhs)
{
    return !(lhs == rhs);
}

BOOL operator == (const XCHAR* lhs, const Ant_String& rhs)
{
    return rhs.operator ==(Ant_String(lhs));
}

BOOL operator != (const XCHAR* lhs, const Ant_String& rhs)
{
    return !(lhs == rhs);
}

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
/* EOF */
