/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#include "stdafx.h"

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_DIR_H
#   include "Ant_Dir.h"
#endif
#ifndef ANT_DIRIMPL_H
#   include "Ant_DirImpl.h"
#endif
#ifndef ANT_PATH_H
#   include "Ant_Path.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Functions
Ant_Dir::Ant_Dir(const BOOL& bRecLog)
: m_ptr(new Ant_DirImpl(bRecLog))
{

}

Ant_Dir::Ant_Dir(const Ant_String& strDirName, const BOOL& bRecLog)
: m_ptr(new Ant_DirImpl(strDirName, bRecLog))
{

}

Ant_Dir::Ant_Dir(const XCHAR* szDirName, const BOOL& bRecLog)
: m_ptr(new Ant_DirImpl(szDirName, bRecLog))
{

}

Ant_Dir::~Ant_Dir()
{
    if(NULL != m_ptr) {
        delete m_ptr;
        m_ptr = NULL;
    }
}

VOID Ant_Dir::setWorkDir(const Ant_String& strDirName)
{
    if(NULL == m_ptr) {
        // m_ptr = new Ant_DirImpl(strDirName);
        return;
    }
    else {
        m_ptr->setWorkDir(strDirName);
    }
}

BOOL Ant_Dir::exists(const Ant_String& strFileName)
{
    return m_ptr ? m_ptr->exists(strFileName) : FALSE;
}

BOOL Ant_Dir::move(const Ant_String& strOldNme, const Ant_String& strNewName)
{
    return m_ptr ? m_ptr->move(strOldNme, strNewName) : FALSE;
}

BOOL Ant_Dir::copy(const Ant_String& strSrcName, const Ant_String& strDstName)
{
    return m_ptr ? m_ptr->copy(strSrcName, strDstName) : FALSE;
}

BOOL Ant_Dir::remove(const Ant_String& strFileName, const BOOL& bForce)
{
    return m_ptr ? m_ptr->remove(strFileName, bForce) : FALSE;
}

BOOL Ant_Dir::makeDir(const Ant_String& strDirName)
{
    return m_ptr ? m_ptr->makeDir(strDirName) : FALSE;
}

BOOL Ant_Dir::makePath(const Ant_String& strPathName)
{
    return m_ptr ? m_ptr->makePath(strPathName) : FALSE;
}

DWORD Ant_Dir::entryNum(const DWORD& dwFilters)
{
    return m_ptr ? m_ptr->entryNum(dwFilters) : 0;
}

Ant_DirIterator* Ant_Dir::entryIterator(const DWORD& dwFilters)
{
    return m_ptr ? m_ptr->entryIterator(dwFilters) : NULL;
}

Ant_String Ant_Dir::currentDir()
{
#if defined(_WIN32) || defined(_WIN32_WCE)
    XCHAR szPath[MAX_PATH + 1] = { 0 };
    DWORD dwLength = ::GetModuleFileName(NULL, szPath, MAX_PATH);
    if(0 == dwLength) {
        return XTEXT("");
    }
    XCHAR* p = xcsrchr(szPath, Ant_Path::separator());
    if(NULL == p) {
        return Ant_String(szPath);
    }
    return Ant_String(szPath, p - szPath);
#else // For Linux
    return XTEXT("");
#endif
}

Ant_FileError Ant_Dir::getLastError() const
{
    return m_ptr ? m_ptr->getLastError() : ANT_FER_NoError;
}

Ant_DirIterator::Ant_DirIterator(const Ant_String& strDirPath, const DWORD& dwFilters, const BOOL& bRecLog)
: m_ptr(new Ant_DirIteratorImpl(strDirPath, dwFilters, bRecLog))
{

}

Ant_DirIterator::~Ant_DirIterator()
{
    if(NULL != m_ptr) {
        delete m_ptr;
        m_ptr = NULL;
    }
}

Ant_String Ant_DirIterator::currentFileName() const
{
    return m_ptr ? m_ptr->currentFileName() : XTEXT("");
}

Ant_FileInfo Ant_DirIterator::currentFileInfo() const
{
    return m_ptr ? m_ptr->currentFileInfo() : Ant_FileInfo();
}

BOOL Ant_DirIterator::end() const
{
    return m_ptr ? m_ptr->end() : TRUE;
}

Ant_DirIterator& Ant_DirIterator::operator ++ ()
{
    if (NULL != m_ptr) {
        ++(*m_ptr);
    }
    return *this;
}

VOID Ant_DirIterator::reset()
{
    if (NULL != m_ptr) {
        m_ptr->reset();
    }
}

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
/* EOF */
