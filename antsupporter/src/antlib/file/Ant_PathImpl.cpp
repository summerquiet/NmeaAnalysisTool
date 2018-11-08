/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#include "stdafx.h"

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_PATHIMPL_H
#   include "Ant_PathImpl.h"
#endif
#ifndef ANT_PATH_H
#   include "Ant_Path.h"
#endif
#ifndef ANT_DIR_H
#   include "Ant_Dir.h"
#endif
#ifndef CXX_Ant_PATHPARSER_H
#   include "Ant_PathParser.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Functions
Ant_PathImpl::Ant_PathImpl(const Ant_String& strPathName)
: m_strPathName(strPathName)
, m_bIsNative(FALSE)
{
    m_strPathName.trim();
    toNative();
}

BOOL Ant_PathImpl::toNative()
{
    if (m_bIsNative) {
        return TRUE;
    }

    if (!toNative(m_strPathName)) {
        return FALSE;
    }

    m_bIsNative = TRUE;

    return TRUE;
}

Ant_String Ant_PathImpl::driveName() const
{
    if (isRelative(m_strPathName)) {
        return XTEXT("");
    }

    if (isRoot(m_strPathName)) {
        return m_strPathName;
    }

    const XCHAR* p = m_strPathName.getString();
    const XCHAR* q = xcschr(p + 1, Ant_Path::separator());
    Ant_String strRet;
    if (NULL == q) {
        if (!strRet.set(p)) {
            return XTEXT("");
        }
    }
    else
    {
        if (!strRet.set(p, q - p + 1)) {
            return XTEXT("");
        }
    }

    return strRet;
}

Ant_String Ant_PathImpl::fileName() const
{
    if (0 == m_strPathName.getLength()) {
        return XTEXT("");
    }
    const XCHAR* p = m_strPathName.getString();
    const XCHAR* q = xcsrchr(p, Ant_Path::separator());
    Ant_String strRet;
    if (!strRet.set((NULL == q) ? p : q + 1)) {
        return XTEXT("");
    }
    return strRet;
}

Ant_String Ant_PathImpl::baseName() const
{
    // TODO:
    return XTEXT("");
}

Ant_String Ant_PathImpl::fullBaseName() const
{
    // TODO:
    return XTEXT("");
}

Ant_String Ant_PathImpl::suffix() const
{
    // TODO:
    return XTEXT("");
}

Ant_String Ant_PathImpl::fullSuffix() const
{
    // TODO:
    return XTEXT("");
}

BOOL Ant_PathImpl::append(const Ant_String& strFileName)
{
    // check parameters
    if (0 == strFileName.getLength()) {
        return TRUE;
    }

    Ant_String strTemp = strFileName;
    if (!strTemp.trim() || !toNative(strTemp) || !isValid(strTemp)) {
        return FALSE;
    }

    const XCHAR* p = strTemp.getString();
    if (m_strPathName.endWith(Ant_Path::separator())) {
        if (Ant_Path::separator() == *p) {
            p++;
        }
        if (!m_strPathName.add(p)) {
            return FALSE;
        }
    }
    else
    {
        if (Ant_Path::separator() != *p) {
            XCHAR ch = Ant_Path::separator();
            if (!m_strPathName.add(&ch, 1)) {
                return FALSE;
            }
        }
        if (!m_strPathName.add(p)) {
            return FALSE;
        }
    }

    return TRUE;
}

BOOL Ant_PathImpl::toNative(Ant_String& strPathName)
{
    const XCHAR* p = strPathName.getString();
    // Ant_String strTemp = XTEXT("");
    for(INT i = 0; i < strPathName.getLength() ; i++ )
    {
#if defined(_WIN32) || defined(_WIN32_WCE)
        if (p[i] == XTEXT('/')) {
#else
        if (p[i] == XTEXT('\\')) {
#endif
            if (!strPathName.replace(i, Ant_Path::separator()))         {
                return FALSE;
            }
        }
    }
    return TRUE;
}

BOOL Ant_PathImpl::toAbsolute(const Ant_String& strCurrentDir)
{
    // if the path is absolute path, just like "[a-z | A-Z]:\XXX" on Windows NT,
    // "\XXX" on WinCE or "/XXX" on Linux, don't need to transform it.
    if (!isRelative(m_strPathName)) {
        return TRUE;
    }

    Ant_String strAbsPath = strCurrentDir;
    const XCHAR* p = m_strPathName.getString();

    while (*p) {
        if (XTEXT('.') == *p) {
            // the path starts with "."
            if (*(p+1)) {
                if (XTEXT('.') == *(p+1)) {
                    // the path starts with ".."
                    if (*(p+2)) {
                        if (Ant_Path::separator() == *(p+2)) {
                            // the path is like "..\XXX"(on Windows) or "../XXX"(on Linux),
                            // that means upper directory
                            if (Ant_Path::isRoot(strAbsPath)) {
                                // the path is root, so there's no upper directory
                                return FALSE;
                            }
                            strAbsPath = upperDir(strAbsPath);
                            p += 3;
                        }
                        else {
                            // the path is like "..XXX\YYY" (on Windows) or "..XXX/YYY"(on Linux)
                            const XCHAR* q = xcschr(p, Ant_Path::separator());
                            if (!strAbsPath.endWith(Ant_Path::separator())) {
                                XCHAR sep = Ant_Path::separator();
                                if (!strAbsPath.add(&sep, 1)) {
                                    return FALSE;
                                }
                            }
                            if (NULL == q) {
                                if (!strAbsPath.add(p)) {
                                    return FALSE;
                                }
                                break;
                            }
                            else {
                                if (!strAbsPath.add(p, q - p)) {
                                    return FALSE;
                                }
                                p = q + 1;
                            }
                        }
                    }
                    else {
                        // the path is "..", that means upper level directory.
                        if (Ant_Path::isRoot(strAbsPath)) {
                            // the path is root, so there's no upper directory
                            return FALSE;
                        }

                        m_strPathName = upperDir(strAbsPath);
                        return TRUE;
                    }
                }
                else if (Ant_Path::separator() == *(p+1)) {
                    // the path is like ".\XXX"(on Windows) or "./XXX"(on Linux),
                    // that means upper directory
                    p += 2;
                }
                else {
                    // the path is like ".XXX\YYY" (on Windows) or ".XXX/YYY"(on Linux)
                    const XCHAR* q = xcschr(p, Ant_Path::separator());
                    if (!strAbsPath.endWith(Ant_Path::separator())) {
                        XCHAR sep = Ant_Path::separator();
                        if (!strAbsPath.add(&sep, 1)) {
                            return FALSE;
                        }
                    }
                    if (NULL == q) {
                        if (!strAbsPath.add(p)) {
                            return FALSE;
                        }
                        break;
                    }
                    else {
                        if (!strAbsPath.add(p, q - p)) {
                            return FALSE;
                        }
                        p = q + 1;
                    }
                }
            }
            else {
                // the path is ".", that means current directory.
                m_strPathName = strAbsPath;
                return TRUE;
            }
        }
        else {
            // the path is like "XXX\YYY" on Windows or "XXX/YYY" on Linux.
            const XCHAR* q = xcschr(p, Ant_Path::separator());
            if (!strAbsPath.endWith(Ant_Path::separator())) {
                XCHAR sep = Ant_Path::separator();
                if (!strAbsPath.add(&sep, 1)) {
                    return FALSE;
                }
            }
            if (NULL == q) {
                if (!strAbsPath.add(p)) {
                    return FALSE;
                }
                break;
            }
            else
            {
                if (!strAbsPath.add(p, q - p)) {
                    return FALSE;
                }
                p = q + 1;
            }
        }
    }

    m_strPathName = strAbsPath;

    return TRUE;
}

BOOL Ant_PathImpl::parseUserDefPath()
{
    Ant_PathFactoryIF* pParserFactory = NULL;
    Ant_GetPathFactory(&pParserFactory);
    if (pParserFactory == NULL) return FALSE;

    Ant_PathParserIF* pParser = pParserFactory->createPathParser();
    if (pParser == NULL) return FALSE;

    Ant_String strAbsPath;
    pParser->parseUserDefPath(m_strPathName, strAbsPath);
    m_strPathName = strAbsPath;

    delete pParser;
    return TRUE;
}

BOOL Ant_PathImpl::isValid(const Ant_String& strPathName) const
{
    if (0 == strPathName.getLength()) {
        return FALSE;
    }

    const XCHAR* p = xcschr(strPathName.getString(), Ant_Path::separator());
    while( NULL != p )
    {
        if (Ant_Path::separator() == *(p + 1)) {
            return FALSE;
        }
        p = xcschr(p + 1, Ant_Path::separator());
    }
    return TRUE;
}

BOOL Ant_PathImpl::isRelative(const Ant_String& strPathName) const
{
    // If the path is root path, it must be not a relative path.
    if (isRoot(strPathName)) {
        return FALSE;
    }

    // On WindowsNT platforms, the absolute path is like "[a-z | A-Z]:\XXX".
    // On Windows CE, the absolute path is like "\XXX", and on Linux, the absolute path is like "/XXX".
    const XCHAR* p = strPathName.getString();
#if defined(_WIN32)
    if (xisalpha(*p) && *(p+1) == XTEXT(':')) {
        return FALSE;
    }
#else
    if (*p == Ant_Path::separator()) {
        return FALSE;
    }
#endif

    return TRUE;
}

BOOL Ant_PathImpl::isRoot(const Ant_String& strPathName) const
{
    // If the path is empty, it must be not a relative path.
    if (0 == strPathName.getLength()) {
        return FALSE;
    }

    const XCHAR* p = strPathName.getString();

    // On WindowsNT platforms, the root path is like "[a-z | A-Z]:" or "[a-z | A-Z]:\".
    // On Windows CE, the root path is "\", and on Linux, the root path is "/".
#if defined(_WIN32)
    if (xisalpha(*p)) {
        if (*(p+1) == XTEXT(':')) {
            if ((*(p+2) == XTEXT('\0'))
                || ((*(p+2) == Ant_Path::separator()) && (*(p+3) == XTEXT('\0')))) {
                return TRUE;
            }
        }
    }
#else
    if (*p == Ant_Path::separator() && *(p+1) == XTEXT('\0')) {
        return TRUE;
    }
#endif

    return FALSE;
}

Ant_String Ant_PathImpl::upperDir(const Ant_String& strPathName) const
{
    if (0 == strPathName.getLength()) {
        return XTEXT("");
    }

    Ant_String strRet;
    const XCHAR* pLastSep = xcsrchr(strPathName.getString(), Ant_Path::separator());
    // ignore the last path separator
    if (pLastSep == (strPathName.getString() + strPathName.getLength() - 1)) {
        pLastSep = xcsrchr(--pLastSep, Ant_Path::separator());
    }

    if (NULL == pLastSep) {
        return XTEXT("");
    }

    if (pLastSep == strPathName.getString()) {
        // like "/XXX"(on Linux ) or "\XXX"(on WinC)
        if (!strRet.set(strPathName.getString(), 1)) {
            return XTEXT("");
        }
    }
    else {
        if (!strRet.set(strPathName.getString(), pLastSep - strPathName.getString())) {
            return XTEXT("");
        }
    }
    return strRet;
}

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
/* EOF */
