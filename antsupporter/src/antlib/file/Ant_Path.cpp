/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#include "stdafx.h"

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_PATH_H
#   include "Ant_Path.h"
#endif
#ifndef ANT_PATHIMPL_H
#   include "Ant_PathImpl.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Functions
Ant_Path::Ant_Path(const Ant_String& strPathName)
: m_ptr(new Ant_PathImpl(strPathName))
{

}

Ant_Path::~Ant_Path()
{
    if(NULL != m_ptr) {
        delete m_ptr;
        m_ptr = NULL;
    }
}

XCHAR Ant_Path::separator()
{
#if defined(_WIN32) || defined(_WIN32_WCE)
    return XTEXT('\\');
#else
    return XTEXT('/');
#endif
}

BOOL Ant_Path::toNative()
{
    return m_ptr ? m_ptr->toNative() : FALSE;
}

BOOL Ant_Path::toAbsolute(const Ant_String& strCurrentDir)
{
    return m_ptr ? m_ptr->toAbsolute(strCurrentDir) : FALSE;
}

BOOL Ant_Path::parseUserDefPath()
{
    return m_ptr ? m_ptr->parseUserDefPath() : FALSE;
}

BOOL Ant_Path::isValid() const
{
    return m_ptr ? m_ptr->isValid() : FALSE;
}

BOOL Ant_Path::isValid(const Ant_String& strPathName)
{
    Ant_Path cPath(strPathName);
    return cPath.isValid();
}

BOOL Ant_Path::isRelative() const
{
    return m_ptr ? m_ptr->isRelative() : FALSE;
}

BOOL Ant_Path::isRelative(const Ant_String& strPathName)
{
    Ant_Path cPath(strPathName);
    return cPath.isRelative();
}

BOOL Ant_Path::isRoot() const
{
    return m_ptr ? m_ptr->isRoot() : FALSE;
}

BOOL Ant_Path::isRoot(const Ant_String& strPathName)
{
    Ant_Path cPath(strPathName);
    return cPath.isRoot();
}

Ant_String Ant_Path::driveName() const
{
    return m_ptr ? m_ptr->driveName() : XTEXT("");
}

Ant_String Ant_Path::upperDir() const
{
    return m_ptr ? m_ptr->upperDir() : XTEXT("");
}

Ant_String Ant_Path::pathName() const
{
    return m_ptr ? m_ptr->pathName() : XTEXT("");
}

Ant_String Ant_Path::fileName() const
{
    return m_ptr ? m_ptr->fileName() : XTEXT("");
}

Ant_String Ant_Path::baseName() const
{
    return m_ptr ? m_ptr->baseName() : XTEXT("");
}

Ant_String Ant_Path::fullBaseName() const
{
    return m_ptr ? m_ptr->fullBaseName() : XTEXT("");
}

Ant_String Ant_Path::suffix() const
{
    return m_ptr ? m_ptr->suffix() : XTEXT("");
}

Ant_String Ant_Path::fullSuffix() const
{
    return m_ptr ? m_ptr->fullSuffix() : XTEXT("");
}

BOOL Ant_Path::append(const Ant_String& strFileName)
{
    return m_ptr ? m_ptr->append(strFileName) : FALSE;
}

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
/* EOF */
