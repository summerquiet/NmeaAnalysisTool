/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#include "stdafx.h"

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_NORMALFILEENGINE_H
#   include "Ant_NormalFileEngine.h"
#endif
#ifndef ANT_PATH_H
#   include "Ant_Path.h"
#endif
#ifndef ANT_FILELOG_H
#   include "Ant_FileLog.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Functions
Ant_NormalFileEngine::Ant_NormalFileEngine(const Ant_String& strFilePath, const BOOL& bRecLog)
: m_strPath(strFilePath)
, m_iOpenMode(ANT_FOM_NotOpen)
#if defined(_WIN32) || defined(_WIN32_WCE)
, m_hFile(INVALID_HANDLE_VALUE)
, m_hFileMap(INVALID_HANDLE_VALUE)
, m_dwFileAttrs(0)
#else // _LINUX
, m_fp(NULL)
, m_fd(-1)
#endif
, m_dwFileFlag(0)
, m_eLastError(ANT_FER_NoError)
, m_bFileFlagInited(FALSE)
, m_bRecLog(bRecLog)
{
    initFileFlag();
}

Ant_NormalFileEngine::~Ant_NormalFileEngine()
{
    close();
}

VOID Ant_NormalFileEngine::initFileFlag()
{
    if (!m_bFileFlagInited) {
        nativeInitFileFlag();
    }
}


BOOL Ant_NormalFileEngine::isSequential() const
{
    return nativeIsSequential();
}

BOOL Ant_NormalFileEngine::caseSensitive() const
{
    return nativeCaseSensitive();
}

BOOL Ant_NormalFileEngine::open(INT iOpenMode)
{
    Ant_FileLog Ant_FileLog(XTEXT("Open"), m_strPath, m_bRecLog);

    cleanLastError();

    // check parameter
    if (!(iOpenMode & ANT_FOM_ReadOnly) &
        !(iOpenMode & ANT_FOM_WriteOnly) &&
        !(iOpenMode & ANT_FOM_Append)) {
        setLastError(ANT_FER_InvalidParam);
        return FALSE;
    }
    if ((iOpenMode & ANT_FOM_Truncate) &
        (!(iOpenMode & ANT_FOM_WriteOnly) || iOpenMode & ANT_FOM_Append)) {
        setLastError(ANT_FER_InvalidParam);
        return FALSE;
    }
    if ((iOpenMode & ANT_FOM_NoBuffer) &
        (!(iOpenMode & ANT_FOM_ReadWrite) && !(iOpenMode & ANT_FOM_Append))) {
        setLastError(ANT_FER_InvalidParam);
        return FALSE;
    }

    if (m_bFileFlagInited) {
        // The file flag has been initialized
        if (m_dwFileFlag & ANT_FFG_TypeDirectory) { 
            // the path is a directory, can not open it
            setLastError(ANT_FER_IsDirectory);
            return FALSE;
        }
    }

    if (iOpenMode & ANT_FOM_Append) {
        // Append implies WriteOnly.
        iOpenMode |= ANT_FOM_WriteOnly;
    }

    // close the opened file first
    if (isOpen()) {
        if (iOpenMode == m_iOpenMode) {
            return TRUE;
        }
        else if (!close()) {
            return FALSE;
        }
    }

    m_iOpenMode = iOpenMode;

    return nativeOpen();
}

BOOL Ant_NormalFileEngine::close()
{
    Ant_FileLog Ant_FileLog(XTEXT("Close"), m_strPath, m_bRecLog);

    cleanLastError();

    if (!isOpen()) {
        return TRUE;
    }

    return nativeClose();
}

DWORD Ant_NormalFileEngine::read(VOID* pBuf, const DWORD& dwMaxSize)
{
    Ant_FileLog Ant_FileLog(XTEXT("Read"), m_strPath, m_bRecLog);

    cleanLastError();

    // check parameters
    if (NULL == pBuf) {
        setLastError(ANT_FER_InvalidParam);
        return 0;
    }

    if (!isOpen()) {
        setLastError(ANT_FER_FileNotOpen);
        return 0;
    }

    if (!(m_iOpenMode & ANT_FOM_ReadOnly)) {
        setLastError(ANT_FER_NoAccess);
        return 0;
    }

    return nativeRead(pBuf, dwMaxSize);
}

DWORD Ant_NormalFileEngine::write(const VOID* pBuf, const DWORD& dwSize)
{
    Ant_FileLog Ant_FileLog(XTEXT("Write"), m_strPath, m_bRecLog);

    cleanLastError();

    // check parameters
    if (NULL == pBuf) {
        setLastError(ANT_FER_InvalidParam);
        return 0;
    }

    if (!isOpen()) {
        setLastError(ANT_FER_FileNotOpen);
        return 0;
    }

    if (!(m_iOpenMode & ANT_FOM_WriteOnly)) {
        setLastError(ANT_FER_NoAccess);
        return 0;
    }

    if (m_iOpenMode & ANT_FOM_Append) {
        if (-1 == seek(0, ANT_FSM_End)) {
            return 0;
        }
    }

    return nativeWrite(pBuf, dwSize);
}

LONG Ant_NormalFileEngine::seek(const INT& iOffset, const Ant_FileSeekMode& eMode)
{
    Ant_FileLog Ant_FileLog(XTEXT("Seek"), m_strPath, m_bRecLog);

    cleanLastError();

    if (!isOpen()) {
        setLastError(ANT_FER_FileNotOpen);
        return -1L;
    }

    // check parameters
    LONG lCurPos = -1L;
    if (-1 == (lCurPos = curPos())) {
        return -1L;
    }
    LONG lFileSize = -1L;
    if (-1 == (lFileSize = size())) {
        return -1L;
    }
    if (((ANT_FSM_Begin == eMode) && (iOffset < 0)) |
        ((ANT_FSM_Current == eMode) && (iOffset < 0 - lCurPos)) ||
        ((ANT_FSM_End == eMode) && (iOffset < 0 - lFileSize))) {
        setLastError(ANT_FER_InvalidParam);
        return -1L;
    }

    return nativeSeek(iOffset, eMode);
}

BOOL Ant_NormalFileEngine::flush()
{
    Ant_FileLog Ant_FileLog(XTEXT("Flush"), m_strPath, m_bRecLog);

    cleanLastError();

    if (!isOpen()) {
        setLastError(ANT_FER_FileNotOpen);
        return FALSE;
    }

    if (!(m_iOpenMode & ANT_FOM_WriteOnly)) {
        setLastError(ANT_FER_NoAccess);
        return FALSE;
    }

    return nativeFlush();
}

LONGLONG Ant_NormalFileEngine::size()
{
    Ant_FileLog Ant_FileLog(XTEXT("GetSize"), m_strPath, m_bRecLog);

    cleanLastError();

    return nativeSize();
}

BOOL Ant_NormalFileEngine::atEnd()
{
    Ant_FileLog Ant_FileLog(XTEXT("AtEnd"), m_strPath, m_bRecLog);

    cleanLastError();

    if (ANT_FOM_NotOpen == m_iOpenMode) {
        setLastError(ANT_FER_FileNotOpen);
        return FALSE;
    }

    return nativeAtEnd();
}

BOOL Ant_NormalFileEngine::setSize(const LONG& lSize)
{
    Ant_FileLog Ant_FileLog(XTEXT("SetSize"), m_strPath, m_bRecLog);

    // check parameters
    if (lSize < 0) {
        setLastError(ANT_FER_InvalidParam);
        return FALSE;
    }

    if (m_bFileFlagInited) {
        // The file flag has been initialized
        if (m_dwFileFlag & ANT_FFG_AttrNotExist) {
            // the file does not exist
            setLastError(ANT_FER_NotExists);
            return FALSE;
        }
        else if (m_dwFileFlag & ANT_FFG_TypeDirectory) {
            // the path is a directory, can not set its size
            setLastError(ANT_FER_IsDirectory);
            return FALSE;
        }
    }

    return nativeSetSize(lSize);
}

BOOL Ant_NormalFileEngine::exists()
{
    Ant_FileLog Ant_FileLog(XTEXT("Exists"), m_strPath, m_bRecLog);

    if (m_bFileFlagInited) {
        // The file flag has been initialized
        if (m_dwFileFlag & ANT_FFG_AttrNotExist) {
            setLastError(ANT_FER_NoError);
            return FALSE;
        }
        return TRUE;
    }

    return nativeExists();
}

BOOL Ant_NormalFileEngine::remove(BOOL bForse)
{
    Ant_FileLog Ant_FileLog(XTEXT("Remove"), m_strPath, m_bRecLog);

    cleanLastError();

    if (m_bFileFlagInited) {
        // The file flag has been initialized
        if (m_dwFileFlag & ANT_FFG_AttrNotExist) {
            // the file does not exist
            setLastError(ANT_FER_NotExists);
            return FALSE;
        }
    }

    // if the file is open, close it
    if (isOpen() && !close()) {
        return FALSE;
    }

    return nativeRemove(bForse);
}

BOOL Ant_NormalFileEngine::copyTo(const Ant_String& strDstName)
{
    Ant_FileLog Ant_FileLog(XTEXT("CopyTo"), m_strPath + XTEXT("->") + strDstName, m_bRecLog);

    cleanLastError();

    Ant_Path cPath(strDstName);
    // check parameters
    if (!cPath.isValid() || cPath.isRoot()) {
        setLastError(ANT_FER_InvalidParam);
        return FALSE;
    }

    if (m_bFileFlagInited) {
        // The file flag has been initialized
        if (m_dwFileFlag & ANT_FFG_AttrNotExist) {
            // the file does not exist
            setLastError(ANT_FER_NotExists);
            return FALSE;
        }
        else if (m_dwFileFlag & ANT_FFG_TypeDirectory) {
            // the path is a directory, can not copy it
            setLastError(ANT_FER_IsDirectory);
            return FALSE;
        }
    }

    // if the file is open, close it
    if (isOpen() && !close()) {
        return FALSE;
    }

    return nativeCopyTo(cPath.pathName());
}

BOOL Ant_NormalFileEngine::moveTo(const Ant_String& strNewName)
{
    Ant_FileLog Ant_FileLog(XTEXT("Move"), m_strPath + XTEXT("->") + strNewName, m_bRecLog);

    cleanLastError();

    Ant_Path cPath(strNewName);
    // check parameters
    if (!cPath.isValid() || cPath.isRoot()) {
        setLastError(ANT_FER_InvalidParam);
        return FALSE;
    }

    if (m_bFileFlagInited) {
        // The file flag has been initialized
        if (m_dwFileFlag & ANT_FFG_AttrNotExist) {
            // the file does not exist
            setLastError(ANT_FER_NotExists);
            return FALSE;
        }
    }

    // if the file is open, close it
    if (isOpen() && !close()) {
        return FALSE;
    }

    return nativeMoveTo(cPath.pathName());
}

BOOL Ant_NormalFileEngine::rename(const Ant_String& strNewName)
{
    Ant_FileLog Ant_FileLog(XTEXT("Rename"), m_strPath + XTEXT("->") + strNewName, m_bRecLog);

    cleanLastError();

    Ant_Path cPath(strNewName);
    // check parameters
    if (!cPath.isValid() || cPath.isRoot()) {
        setLastError(ANT_FER_InvalidParam);
        return FALSE;
    }

    if (m_bFileFlagInited) {
        // The file flag has been initialized
        if (m_dwFileFlag & ANT_FFG_AttrNotExist)
        {
            // the file does not exist
            setLastError(ANT_FER_NotExists);
            return FALSE;
        }
    }

    // if the file is open, close it
    if (isOpen() && !close()) {
        return FALSE;
    }

    return nativeRename(cPath.pathName());
}

BOOL Ant_NormalFileEngine::makeLink(const Ant_String& strLinkName)
{
    Ant_FileLog Ant_FileLog(XTEXT("MakeLink"), strLinkName + XTEXT("->") + m_strPath, m_bRecLog);

    cleanLastError();

    // check parameters
    Ant_Path cPath(strLinkName);
    // check parameters
    if (!cPath.isValid() || cPath.isRoot()) {
        setLastError(ANT_FER_InvalidParam);
        return FALSE;
    }

    return nativeMakeLink(cPath.pathName());
}

Ant_String Ant_NormalFileEngine::linkTarget()
{
    Ant_FileLog Ant_FileLog(XTEXT("LinkTarget"), m_strPath, m_bRecLog);

    cleanLastError();

    return nativeLinkTarget();
}

BOOL Ant_NormalFileEngine::makeDir(const Ant_String& strDirName)
{
    Ant_FileLog Ant_FileLog(XTEXT("MakeDir"), m_strPath, m_bRecLog);

    cleanLastError();

    // check parameters
    //Ant_Path cPath(strDirName);
    if (0 == strDirName.getLength()) {
        setLastError(ANT_FER_InvalidParam);
        return FALSE;
    }

    if (m_bFileFlagInited) {
        // The file flag has been initialized
        if (m_dwFileFlag & ANT_FFG_AttrNotExist) {
            // the file does not exist
            setLastError(ANT_FER_NotExists);
            return FALSE;
        }
    }

    Ant_Path cPath(m_strPath);
    if (!cPath.append(strDirName)) {
        setLastError(ANT_FER_InvalidParam);
        return FALSE;
    }

    return nativeMakeDir(cPath.pathName());
}

Ant_AbstractFileEngine::Iterator* Ant_NormalFileEngine::entryIterator()
{
    cleanLastError();

    if (m_bFileFlagInited) {
        // The file flag has been initialized
        if (m_dwFileFlag & ANT_FFG_AttrNotExist) {
            // the file does not exist
            setLastError(ANT_FER_NotExists);
            return NULL;
        }
        else if (!(m_dwFileFlag & ANT_FFG_TypeDirectory)) {
            // the path is not a directory
            setLastError(ANT_FER_IsNotDirectory);
            return NULL;
        }
    }

    return (new Ant_NormalFileEngineIterator(m_strPath));
}

DWORD Ant_NormalFileEngine::fileFlags(const DWORD& dwMask)
{
    cleanLastError();

    if (m_bFileFlagInited) {
        // The file flag has been initialized
        if (m_dwFileFlag & ANT_FFG_AttrNotExist) {
            // the file does not exist
            setLastError(ANT_FER_NotExists);
            return 0;
        }
        return m_dwFileFlag & dwMask;
    }

    return nativeFileFlags(dwMask);
}

BOOL Ant_NormalFileEngine::setPermissions(const DWORD& dwPerms)
{
    Ant_FileLog Ant_FileLog(XTEXT("SetPermissions"), m_strPath, m_bRecLog);

    cleanLastError();

    // check parameters
    if (dwPerms > ANT_FPM_Mask) {
        setLastError(ANT_FER_InvalidParam);
        return FALSE;
    }

    if (m_bFileFlagInited) {
        // The file flag has been initialized
        if (m_dwFileFlag & ANT_FFG_AttrNotExist) {
            // the file does not exist
            setLastError(ANT_FER_NotExists);
            return FALSE;
        }
    }

    return nativeSetPermissions(dwPerms);
}

BOOL Ant_NormalFileEngine::setEndOfFile()
{
    Ant_FileLog Ant_FileLog(XTEXT("SetEndOfFile"), m_strPath, m_bRecLog);

    cleanLastError();

    if (m_bFileFlagInited) {
        // The file flag has been initialized
        if (m_dwFileFlag & ANT_FFG_AttrNotExist) {
            // the file does not exist
            setLastError(ANT_FER_NotExists);
            return FALSE;
        }
    }

    return nativeSetEndOfFile();
}

Ant_String Ant_NormalFileEngine::ownerName(const Ant_FileUser& eKind) const
{
    return nativeOwnerName(eKind);
}

DWORD Ant_NormalFileEngine::ownerID(const Ant_FileUser& eKind) const
{
    return nativeOwnerID(eKind);
}

Ant_Time Ant_NormalFileEngine::fileTime(const Ant_FileTime& eKind)
{
    return nativeFileTime(eKind);
}

LONG Ant_NormalFileEngine::curPos()
{
    cleanLastError();

    if (!isOpen())
    {
        setLastError(ANT_FER_FileNotOpen);
        return -1;
    }

    return nativeCurPos();
}

VOID Ant_NormalFileEngine::setLastError()
{
    nativeSetLastError();
}

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
/* EOF */
