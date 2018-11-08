/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef CXX_Ant_CODE_H
#define CXX_Ant_CODE_H
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
#ifndef ANT_STRINGDEF_H
#   include "Ant_StringDef.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Value define

#define ANT_CODE_MAX    MAX_PATH

/*---------------------------------------------------------------------------*/
// Class declare

/**
 * @breif class to handle code page translation.
 *
 * @detail convert string from OEM code pages to UTF16-LE.
 */
class ANTLIB_API Ant_Code : public virtual Ant_Object
{
public:

    Ant_Code();
    Ant_Code(const CHAR* szOrgCode ,const INT iMaxCodeSize);
    ~Ant_Code();

    VOID set(const CHAR* szOrgCode ,const INT iMaxCodeSize = ANT_CODE_MAX);

    VOID clear();

    BOOL convert(DWORD dwCodePage);

    INT getSize();

    TCHAR* get();

private:

    const CHAR*  m_kszOrgCode;
    TCHAR        m_szConvCode[ANT_CODE_MAX];
    INT          m_iCodeSize;
    INT          m_iMaxCodeSize;

private:

    // Disable the copy constructor and operator =
    Ant_Code(const Ant_Code&);
    Ant_Code& operator = (const Ant_Code&);

};

/**
 * @breif class to handle code page translation.
 *
 * @detail convert string from UTF16-LE to OEM code pages.
 */
class ANTLIB_API Ant_Decode : public virtual Ant_Object
{
public:

    Ant_Decode();
    Ant_Decode(const TCHAR* szOrgCode ,const INT iMaxCodeSize);
    ~Ant_Decode();

    VOID set(const TCHAR* szOrgCode ,const INT iMaxCodeSize = ANT_CODE_MAX);

    VOID clear();

    BOOL convert(DWORD dwCodePage);

    INT getSize();

    CHAR* get();

private:

    const TCHAR*  m_kszOrgCode;
    CHAR          m_szConvCode[ANT_CODE_MAX];
    INT           m_iCodeSize;
    INT           m_iMaxCodeSize;

private:

    // Disable the copy constructor and operator =
    Ant_Decode(const Ant_Decode&);
    Ant_Decode& operator = (const Ant_Decode&);

};

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif  // ANT_CODE_H

/*---------------------------------------------------------------------------*/
/* EOF */
