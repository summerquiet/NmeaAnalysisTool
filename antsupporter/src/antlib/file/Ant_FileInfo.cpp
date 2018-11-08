/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#include "stdafx.h"

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_FILEINFO_H
#   include "Ant_FileInfo.h"
#endif
#ifndef ANT_FILEINFOIMPL_H
#   include "Ant_FileInfoImpl.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Functions
Ant_FileInfo::Ant_FileInfo(const BOOL& bRecLog)
: m_ptr(new Ant_FileInfoImpl(bRecLog))
, m_bRecLog(bRecLog)
{
}

Ant_FileInfo::Ant_FileInfo(const Ant_String& strFileName, const BOOL& bRecLog)
: m_ptr(new Ant_FileInfoImpl(strFileName, bRecLog))
, m_bRecLog(bRecLog)
{
}

Ant_FileInfo::Ant_FileInfo(const Ant_FileInfo& cInfo)
: m_ptr(new Ant_FileInfoImpl(cInfo.fileName(), cInfo.m_bRecLog))
{
}

Ant_FileInfo&  Ant_FileInfo::operator=(const Ant_FileInfo& cInfo)
{
    if (&cInfo != this) {
        if (NULL != m_ptr) {
            delete m_ptr;
            m_ptr = NULL;
        }
        m_ptr = new Ant_FileInfoImpl(cInfo.fileName(), cInfo.m_bRecLog);
    }
    return *this;
}

Ant_FileInfo::~Ant_FileInfo()
{
    if (NULL != m_ptr) {
        delete m_ptr;
        m_ptr = NULL;
    }
}

VOID Ant_FileInfo::setFileName(const Ant_String& strFileName)
{
    if (NULL != m_ptr) {
        m_ptr->setFileName(strFileName);
    }
}

Ant_String Ant_FileInfo::fileName() const
{
    return m_ptr ? m_ptr->fileName() : XTEXT("");
}

BOOL Ant_FileInfo::exists() const
{
    return m_ptr ? m_ptr->exists() : FALSE;
}

BOOL Ant_FileInfo::isReadable() const
{
    return m_ptr ? m_ptr->isReadable() : FALSE;
}

BOOL Ant_FileInfo::isWritable() const
{
    return m_ptr ? m_ptr->isWritable() : FALSE;
}

BOOL Ant_FileInfo::isExecutable() const
{
    return m_ptr ? m_ptr->isExecutable() : FALSE;
}

BOOL Ant_FileInfo::isHidden() const
{
    return m_ptr ? m_ptr->isHidden() : FALSE;
}

DWORD Ant_FileInfo::getFileType() const
{
    return m_ptr ? m_ptr->getFileType() : FALSE;
}

BOOL Ant_FileInfo::isFile() const
{
    return m_ptr ? m_ptr->isFile() : FALSE;
}

BOOL Ant_FileInfo::isDir() const
{
    return m_ptr ? m_ptr->isDir() : FALSE;
}

BOOL Ant_FileInfo::isLink() const
{
    return m_ptr ? m_ptr->isLink() : FALSE;
}

Ant_Time Ant_FileInfo::creationTime() const
{
    return m_ptr ? m_ptr->creationTime() : Ant_Time();
}

Ant_Time Ant_FileInfo::lastWriteTime() const
{
    return m_ptr ? m_ptr->lastWriteTime() : Ant_Time();
}

Ant_Time Ant_FileInfo::lastAccessTime() const
{
    return m_ptr ? m_ptr->lastAccessTime() : Ant_Time();
}

Ant_String Ant_FileInfo::user() const
{
    return m_ptr ? m_ptr->user() : Ant_String();
}

DWORD Ant_FileInfo::userID() const
{
    return m_ptr ? m_ptr->userID() : 0;
}

Ant_String Ant_FileInfo::group() const
{
    return m_ptr ? m_ptr->group() : Ant_String();
}

DWORD Ant_FileInfo::groupID() const
{
    return m_ptr ? m_ptr->groupID() : 0;
}

LONG Ant_FileInfo::size() const
{
    return m_ptr ? static_cast<LONG>(m_ptr->size()) : -1L;
}

LONGLONG Ant_FileInfo::size64() const
{
    return m_ptr ? m_ptr->size() : -1LL;
}

DWORD Ant_FileInfo::entryNum(const DWORD& dwFilters) const
{
    return m_ptr ? m_ptr->entryNum(dwFilters) : 0;
}

Ant_FileError Ant_FileInfo::getLastError() const
{
    return m_ptr ? m_ptr->getLastError() : ANT_FER_NoError;
}

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
/* EOF */
