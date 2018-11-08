/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#include "stdafx.h"

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_FILEIMPL_H
#   include "Ant_FileImpl.h"
#endif
#ifndef ANT_PATH_H
#   include "Ant_Path.h"
#endif
#ifndef ANT_ABSTRACTFILEENGINE_H
#   include "Ant_AbstractFileEngine.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Functions
Ant_FileImpl::Ant_FileImpl(const BOOL& bRecLog)
: m_pFileEngine(NULL)
, m_strFilePath()
, m_eLastError(ANT_FER_NoError)
, m_bRecLog(bRecLog)
{

}

Ant_FileImpl::Ant_FileImpl(const Ant_String& strFileName, const BOOL& bRecLog)
: m_pFileEngine(NULL)
, m_strFilePath()
, m_eLastError(ANT_FER_NoError)
, m_bRecLog(bRecLog)
{
    Ant_Path cPath(strFileName);
    cPath.parseUserDefPath();
    m_strFilePath = cPath.pathName();
    m_pFileEngine = Ant_AbstractFileEngine::create(m_strFilePath, m_bRecLog);
}

Ant_FileImpl::~Ant_FileImpl()
{
    if(NULL != m_pFileEngine) {
        delete m_pFileEngine;
        m_pFileEngine = NULL;
    }
}

VOID Ant_FileImpl::setFileName(const Ant_String& strFileName)
{
    if(NULL != m_pFileEngine) {
        delete m_pFileEngine;
        m_pFileEngine = NULL;
    }
    Ant_Path cPath(strFileName);
    cPath.parseUserDefPath();
    m_strFilePath = cPath.pathName();
    m_pFileEngine = Ant_AbstractFileEngine::create(m_strFilePath, m_bRecLog);
}

BOOL Ant_FileImpl::open(const INT& iOpenMode)
{
    cleanLastError();

    if(NULL == m_pFileEngine) {
        return FALSE;
    }
    
    if(m_pFileEngine->fileFlags() & ANT_FFG_TypeDirectory) {
        setLastError(ANT_FER_IsDirectory);
        return FALSE;
    }

    if(!m_pFileEngine->open(const_cast<INT&>(iOpenMode))) {
        setLastError(m_pFileEngine->getLastError());
        return FALSE;
    }

    return TRUE;
}

BOOL Ant_FileImpl::close()
{
    cleanLastError();

    if(NULL == m_pFileEngine) {
        return FALSE;
    }
    if(!m_pFileEngine->close()) {
        setLastError(m_pFileEngine->getLastError());
        return FALSE;
    }
    return TRUE;
}

BOOL Ant_FileImpl::isOpen() const
{
    if (NULL == m_pFileEngine) {
        return FALSE;
    }

    return m_pFileEngine->isOpen();
}

DWORD Ant_FileImpl::read(VOID* pBuff, const DWORD& dwMaxSize)
{
    cleanLastError();

    if (NULL == m_pFileEngine) {
        return 0;
    }

    DWORD dwRet = m_pFileEngine->read(pBuff, dwMaxSize);
    if (dwMaxSize != dwRet) {
        setLastError(m_pFileEngine->getLastError());
    }

    return dwRet;
}

DWORD Ant_FileImpl::write(const VOID* pBuff, const DWORD& dwSize)
{
    cleanLastError();

    if (NULL == m_pFileEngine) {
        return 0;
    }

    DWORD dwRet = m_pFileEngine->write(pBuff, dwSize);
    if (dwSize != dwRet )
    {
        setLastError(m_pFileEngine->getLastError());
    }

    return dwRet;
}

LONG Ant_FileImpl::seek(const INT& iOffset, const Ant_FileSeekMode& eMode)
{
    cleanLastError();

    if (NULL == m_pFileEngine) {
        return -1L;
    }

    LONG lRet = m_pFileEngine->seek(iOffset, eMode);
    if (-1L == lRet) {
        setLastError(m_pFileEngine->getLastError());
    }

    return lRet;
}

BOOL
Ant_FileImpl::atEnd()
{
    cleanLastError();

    if (NULL == m_pFileEngine) {
        return FALSE;
    }

    if (!m_pFileEngine->atEnd()) {
        setLastError(m_pFileEngine->getLastError());
        return FALSE;
    }

    return TRUE;
}

BOOL Ant_FileImpl::flush()
{
    cleanLastError();

    if (NULL == m_pFileEngine) {
        return FALSE;
    }
    if (!m_pFileEngine->flush()) {
        setLastError(m_pFileEngine->getLastError());
        return FALSE;
    }

    return TRUE;
}

BOOL Ant_FileImpl::exists()
{
    cleanLastError();

    if (NULL == m_pFileEngine) {
        return FALSE;
    }
    if (!m_pFileEngine->exists()) {
        setLastError(m_pFileEngine->getLastError());
        return FALSE;
    }

    return TRUE;
}

BOOL Ant_FileImpl::remove()
{
    cleanLastError();

    if (NULL == m_pFileEngine) {
        return FALSE;
    }

    if (m_pFileEngine->fileFlags() & ANT_FFG_TypeDirectory) {
        setLastError(ANT_FER_IsDirectory);
        return FALSE;
    }

    if (!m_pFileEngine->remove()) {
        setLastError(m_pFileEngine->getLastError());
        return FALSE;
    }

    return TRUE;
}

BOOL Ant_FileImpl::moveTo(const Ant_String& strNewName)
{
    cleanLastError();

    if (NULL == m_pFileEngine) {
        return FALSE;
    }

    if (m_pFileEngine->fileFlags() & ANT_FFG_TypeDirectory) {
        setLastError(ANT_FER_IsDirectory);
        return FALSE;
    }

    Ant_Path cPath(strNewName);
    if (cPath.pathName() == m_strFilePath) {
        return TRUE;
    }
    
    if (!m_pFileEngine->copyTo(cPath.pathName())) {
        setLastError(m_pFileEngine->getLastError());
        return FALSE;
    }

    if (!m_pFileEngine->remove(TRUE)) {
        setLastError(m_pFileEngine->getLastError());
        return FALSE;
    }

    return TRUE;
}

BOOL Ant_FileImpl::copyTo(const Ant_String& strDstFileName)
{
    cleanLastError();

    if (NULL == m_pFileEngine) {
        return FALSE;
    }
    
    if (m_pFileEngine->fileFlags() & ANT_FFG_TypeDirectory) {
        setLastError(ANT_FER_IsDirectory);
        return FALSE;
    }
    
    Ant_Path cPath(strDstFileName);
    if (!m_pFileEngine->copyTo(cPath.pathName())) {
        setLastError(m_pFileEngine->getLastError());
        return FALSE;
    }

    return TRUE;
}

BOOL Ant_FileImpl::makeLink(const Ant_String& strLinkName)
{
    // TODO:
    return FALSE;
}

Ant_String Ant_FileImpl::linkTarget()
{
    // TODO:
    return XTEXT("");
}

BOOL Ant_FileImpl::setSize(const LONG& lSize)
{
    cleanLastError();

    if (NULL == m_pFileEngine) {
        return FALSE;
    }
    if (!m_pFileEngine->setSize(lSize)) {
        setLastError(m_pFileEngine->getLastError());
        return FALSE;
    }

    return TRUE;
}

BOOL Ant_FileImpl::setPermissions(const DWORD& dwPerms)
{
    cleanLastError();

    if (NULL == m_pFileEngine) {
        return FALSE;
    }
    if (!m_pFileEngine->setPermissions(dwPerms)) {
        setLastError(m_pFileEngine->getLastError());
        return FALSE;
    }

    return TRUE;
}

BOOL Ant_FileImpl::setEndOfFile()
{
    cleanLastError();

    if (NULL == m_pFileEngine) {
        return FALSE;
    }
    if (!m_pFileEngine->setEndOfFile()) {
        setLastError(m_pFileEngine->getLastError());
        return FALSE;
    }

    return TRUE;
}

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
/* EOF */
