/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#include "stdafx.h"

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_FILEINFOIMPL_H
#   include "Ant_FileInfoImpl.h"
#endif
#ifndef ANT_ABSTRACTFILEENGINE_H
#   include "Ant_AbstractFileEngine.h"
#endif
#ifndef ANT_PATH_H
#   include "Ant_Path.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Functions
Ant_FileInfoImpl::Ant_FileInfoImpl(const BOOL& bRecLog)
: m_pFileEngine(NULL)
, m_strFilePath()
, m_eLastError(ANT_FER_NoError)
, m_bRecLog(bRecLog)
{
}

Ant_FileInfoImpl::Ant_FileInfoImpl(const Ant_String& strFileName, const BOOL& bRecLog)
: m_pFileEngine(NULL)
, m_strFilePath()
, m_eLastError(ANT_FER_NoError)
, m_bRecLog(bRecLog)
{
    Ant_Path cPath(strFileName);
    m_strFilePath = cPath.pathName();
    m_pFileEngine = Ant_AbstractFileEngine::create(m_strFilePath, m_bRecLog);
}

Ant_FileInfoImpl::~Ant_FileInfoImpl()
{
    if (NULL != m_pFileEngine) {
        delete m_pFileEngine;
        m_pFileEngine = NULL;
    }
}

VOID Ant_FileInfoImpl::setFileName(const Ant_String& strFileName)
{
    if (NULL != m_pFileEngine) {
        delete m_pFileEngine;
        m_pFileEngine = NULL;
    }
    Ant_Path cPath(strFileName);
    m_strFilePath = cPath.pathName();
    m_pFileEngine = Ant_AbstractFileEngine::create(m_strFilePath, m_bRecLog);
}

BOOL Ant_FileInfoImpl::exists()
{
    cleanLastError();

    if (NULL == m_pFileEngine) {
        return FALSE;
    }

    BOOL ret = m_pFileEngine->exists();

    return ret;
}

BOOL Ant_FileInfoImpl::isReadable()
{
    cleanLastError();

    if (NULL == m_pFileEngine) {
        return FALSE;
    }

    DWORD dwFilePerms = m_pFileEngine->fileFlags(ANT_FFG_PermsMask);
    if (0 == dwFilePerms) {
        setLastError(m_pFileEngine->getLastError());
        return FALSE;
    }

    return (dwFilePerms & ANT_FPM_OwnerRead) ? TRUE : FALSE;
}

BOOL Ant_FileInfoImpl::isWritable()
{
    cleanLastError();

    if (NULL == m_pFileEngine) {
        return FALSE;
    }

    DWORD dwFilePerms = m_pFileEngine->fileFlags(ANT_FFG_PermsMask);
    if (0 == dwFilePerms) {
        setLastError(m_pFileEngine->getLastError());
        return FALSE;
    }

    return (dwFilePerms & ANT_FPM_OwnerWrite) ? TRUE : FALSE;
}

BOOL Ant_FileInfoImpl::isExecutable()
{
    cleanLastError();

    if (NULL == m_pFileEngine) {
        return FALSE;
    }

    DWORD dwFilePerms = m_pFileEngine->fileFlags(ANT_FFG_PermsMask);
    if (0 == dwFilePerms) {
        setLastError(m_pFileEngine->getLastError());
        return FALSE;
    }

    return (dwFilePerms & ANT_FPM_OwnerExe) ? TRUE : FALSE;
}

BOOL Ant_FileInfoImpl::isHidden()
{
    cleanLastError();

    if (NULL == m_pFileEngine) {
        return FALSE;
    }

    DWORD dwFileAttrs = m_pFileEngine->fileFlags(ANT_FFG_AttrsMask);
    if (0 == dwFileAttrs) {
        setLastError(m_pFileEngine->getLastError());
        return FALSE;
    }

    return (dwFileAttrs & ANT_FFG_AttrHidden) ? TRUE : FALSE;
}

DWORD Ant_FileInfoImpl::getFileType()
{
    cleanLastError();

    if (NULL == m_pFileEngine) {
        return 0;
    }

    DWORD dwFileAttrs = m_pFileEngine->fileFlags(ANT_FFG_TypesMask);
    if (0 == dwFileAttrs) {
        setLastError(m_pFileEngine->getLastError());
        return 0;
    }

    return dwFileAttrs;
}

BOOL Ant_FileInfoImpl::isFile()
{
    cleanLastError();

    if (NULL == m_pFileEngine) {
        return FALSE;
    }

    DWORD dwFileTypes = m_pFileEngine->fileFlags(ANT_FFG_TypesMask);
    if (0 == dwFileTypes) {
        setLastError(m_pFileEngine->getLastError());
        return FALSE;
    }

    return (dwFileTypes & ANT_FFG_TypeFile) ? TRUE : FALSE;
}

BOOL Ant_FileInfoImpl::isDir()
{
    cleanLastError();

    if (NULL == m_pFileEngine) {
        return FALSE;
    }

    DWORD dwFileTypes = m_pFileEngine->fileFlags(ANT_FFG_TypesMask);
    if (0 == dwFileTypes) {
        setLastError(m_pFileEngine->getLastError());
        return FALSE;
    }

    return (dwFileTypes & ANT_FFG_TypeDirectory) ? TRUE : FALSE;
}

BOOL Ant_FileInfoImpl::isLink()
{
    cleanLastError();

    if (NULL == m_pFileEngine) {
        return FALSE;
    }

    DWORD dwFileTypes = m_pFileEngine->fileFlags(ANT_FFG_TypesMask);
    if (0 == dwFileTypes) {
        setLastError(m_pFileEngine->getLastError());
        return FALSE;
    }

    return (dwFileTypes & ANT_FFG_TypeLink) ? TRUE : FALSE;
}

Ant_Time Ant_FileInfoImpl::creationTime()
{
    cleanLastError();

    if (NULL == m_pFileEngine) {
        return Ant_Time();
    }

    Ant_Time cTime = m_pFileEngine->fileTime(ANT_FTM_Creation);
    if (Ant_Time() == cTime) {
        setLastError(m_pFileEngine->getLastError());
    }
    return cTime;
}

Ant_Time Ant_FileInfoImpl::lastWriteTime()
{
    cleanLastError();

    if (NULL == m_pFileEngine) {
        return Ant_Time();
    }

    Ant_Time cTime = m_pFileEngine->fileTime(ANT_FTM_LastWrite);
    if (Ant_Time() == cTime) {
        setLastError(m_pFileEngine->getLastError());
    }
    return cTime;
}

Ant_Time Ant_FileInfoImpl::lastAccessTime()
{
    cleanLastError();

    if (NULL == m_pFileEngine) {
        return Ant_Time();
    }

    Ant_Time cTime = m_pFileEngine->fileTime(ANT_FTM_LastAccess);
    if (Ant_Time() == cTime) {
        setLastError(m_pFileEngine->getLastError());
    }
    return cTime;
}

Ant_String Ant_FileInfoImpl::user()
{
    // TODO:
    return Ant_String();
}

DWORD Ant_FileInfoImpl::userID()
{
    // TODO:
    return 0;
}

Ant_String Ant_FileInfoImpl::group()
{
    // TODO:
    return Ant_String();
}

DWORD Ant_FileInfoImpl::groupID()
{
    // TODO:
    return 0;
}

LONGLONG Ant_FileInfoImpl::size()
{
    cleanLastError();

    if (NULL == m_pFileEngine) {
        return -1LL;
    }

    LONGLONG llSize = m_pFileEngine->size();
    if (-1LL == llSize) {
        setLastError(m_pFileEngine->getLastError());
    }

    return llSize;
}

DWORD Ant_FileInfoImpl::entryNum(const DWORD& dwFilters)
{
    cleanLastError();

    if (NULL == m_pFileEngine) {
        return 0;
    }

    Ant_AbstractFileEngine::Iterator* pIter = m_pFileEngine->entryIterator();
    DWORD dwEntryNum = 0;
    while( !(pIter->end()) )
    {
        // TODO: check the file matches the filters
        dwEntryNum++;
        ++(*pIter);
    }
    delete pIter;

    return dwEntryNum;
}

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
/* EOF */
