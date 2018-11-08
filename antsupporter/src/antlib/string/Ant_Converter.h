/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_CONVERTER_H
#define ANT_CONVERTER_H
#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_NEWTYPESDEFINE_H
#   include "Ant_NewTypesDefine.h"
#endif
#ifndef ANT_OBJECT_H
#   include "Ant_Object.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Value define

// Default Code page.
const DWORD ANT_CP_ACP = 0; // ANSII

// Mark for UTF16, UTF32
const DWORD ANT_CP_UTF16LE = DWORD(-2);
const DWORD ANT_CP_UTF16BE = DWORD(-3);
const DWORD ANT_CP_UTF32BE = DWORD(-4);
const DWORD ANT_CP_UTF32LE = DWORD(-5);

// Max CodePage Number (supported)
const DWORD ANT_CP_MAX = 23;

/*---------------------------------------------------------------------------*/
// Class declare

/**
 * @@brief The class that convert CHAR and WCHAR from or to UTF8
 *
 */
class Ant_Converter : public virtual Ant_Object
{
public:

    Ant_Converter() {}
    ~Ant_Converter() {}

    INT utf8ToChar(const DWORD& dwCodePage, const CHAR* pszSrc, const INT& nSrcLen, CHAR* pszDst, const INT& nDstLen);

    INT utf8ToWChar(const CHAR* pszSrc, const INT& nSrcLen, WCHAR* pszDst, const INT& nDstLen);

    INT charToUTF8(const DWORD& dwCodePage, const CHAR* pszSrc, const INT& nSrcLen, CHAR* pszDst, const INT& nDstLen);

    INT wcharToUTF8(const WCHAR* pszSrc, const INT& nSrcLen, CHAR* pszDst, const INT& nDstLen);

    INT utf16BEToUTF8(const UCHAR16* pszSrc, const INT& nSrcLen, CHAR* pszDst, const INT& nDstLen);

    INT utf16LEToUTF8(const UCHAR16* pszSrc, const INT& nSrcLen, CHAR* pszDst, const INT& nDstLen);

    INT utf8ToUTF16BE(const CHAR* pszSrc, const INT& nSrcLen, UCHAR16* pszDst, const INT& nDstLen);

    INT utf8ToUTF16LE(const CHAR* pszSrc, const INT& nSrcLen, UCHAR16* pszDst, const INT& nDstLen);

    INT utf16ToChar(const DWORD& dwCodePage, const UCHAR16* pszSrc, const INT& nSrcLen, CHAR* pszDst, const INT& nDstLen);

    INT charToUTF16(const DWORD& dwCodePage, const CHAR* pszSrc, const INT& nSrcLen, UCHAR16* pszDst, const INT& nDstLen);

private:

    struct CONV_CP_INFO {
        const DWORD dwToCP;
        const CHAR* pszToCP;
        const DWORD dwFromCP;
        const CHAR* pszFromCP;
    };

    INT convert(const CONV_CP_INFO& cp_info, CHAR* pszDst, const INT& nDstLen, const CHAR* pszSrc, const INT& nSrcLen);

    VOID getCPName(const DWORD dwCodePage, CHAR* szCPName, INT nStrLen);

private:

    // Disable the copy constructor and operator =
    Ant_Converter(const Ant_Converter&);
    Ant_Converter& operator = (const Ant_Converter&);

};

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif  // ANT_CONVERTER_H

/*---------------------------------------------------------------------------*/
/* EOF */
