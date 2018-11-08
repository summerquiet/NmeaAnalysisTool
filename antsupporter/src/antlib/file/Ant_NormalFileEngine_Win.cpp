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
#ifndef ANT_FILEDEF_H
#   include "Ant_FileDef.h"
#endif
#ifndef ANT_PATH_H
#   include "Ant_Path.h"
#endif

#if defined(_WIN32_WCE)
#   include <Winbase.h>
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Functions
static Ant_FileError Ant_ErrFromWinErr(const DWORD& dwErrorCode)
{
    Ant_FileError eLastError = ANT_FER_NoError;
    switch(dwErrorCode) {

    case ERROR_SUCCESS:
        eLastError = ANT_FER_NoError;
        break;
    case ERROR_INVALID_PARAMETER:
        eLastError = ANT_FER_InvalidParam;
        break;
    case ERROR_INVALID_NAME:
        eLastError = ANT_FER_InvalidPathName;
        break;
    case ERROR_FILE_NOT_FOUND:
    case ERROR_PATH_NOT_FOUND:
        eLastError = ANT_FER_NotExists;
        break;
    case ERROR_FILE_EXISTS:
    case ERROR_ALREADY_EXISTS:
        eLastError = ANT_FER_AlreadyExists;
        break;
    case ERROR_ACCESS_DENIED:
    case ERROR_INVALID_ACCESS:
        eLastError = ANT_FER_NoAccess;
        break;
    case ERROR_HANDLE_DISK_FULL:
    case ERROR_DISK_FULL:
        eLastError = ANT_FER_NoSpace;
        break;
    case ERROR_NO_MORE_FILES:
        eLastError = ANT_FER_NoMoreFile;
        break;
    case ERROR_DIR_NOT_EMPTY:
        eLastError = ANT_FER_NotEmpty;
        break;
    case ERROR_SHARING_VIOLATION:
    case ERROR_LOCK_VIOLATION:
        eLastError = ANT_FER_IsUsed;
        break;
    default:
        eLastError = ANT_FER_Unknown;
    }
    return eLastError;
}

static DWORD Ant_WinAttrsToFileFlag(const DWORD& dwFileAttrs)
{
    DWORD dwFileFlags = 0;
    dwFileFlags |= (ANT_FFG_PermOwnerRead | ANT_FFG_PermGroupRead);
    if (FILE_ATTRIBUTE_NORMAL == dwFileAttrs) {
        dwFileFlags |= (ANT_FFG_PermOwnerWrite | ANT_FFG_PermGroupWrite);
        dwFileFlags |= ANT_FFG_TypeFile;
        dwFileFlags |= ANT_FFG_AttrNormal;
    }
    else {
        if (!(dwFileAttrs & FILE_ATTRIBUTE_READONLY)
#if defined(_WIN32_WCE)
            && !(dwFileAttrs & FILE_ATTRIBUTE_INROM)
#endif
            ) {
            dwFileFlags |= (ANT_FFG_PermOwnerWrite | ANT_FFG_PermGroupWrite);
        }

        if (dwFileAttrs & FILE_ATTRIBUTE_DIRECTORY) {
            dwFileFlags |= ANT_FFG_TypeDirectory;
        }
        else {
            dwFileFlags |= ANT_FFG_TypeFile;
        }

        dwFileFlags |= ANT_FAB_Normal;
        if (dwFileAttrs & FILE_ATTRIBUTE_HIDDEN) {
            dwFileFlags |= ANT_FAB_Hidden;
            dwFileFlags &= ~ANT_FAB_Normal;
        }
        if (dwFileAttrs & FILE_ATTRIBUTE_SYSTEM) {
            dwFileFlags |= ANT_FAB_System;
            dwFileFlags &= ~ANT_FAB_Normal;
        }
    }

    return dwFileFlags;
}

VOID Ant_NormalFileEngine::nativeInitFileFlag()
{
    cleanLastError();

    m_dwFileAttrs = ::GetFileAttributes(m_strPath.getString());
    if( 0xFFFFFFFF == m_dwFileAttrs )
    {
        setLastError();
        if( ANT_FER_NotExists == m_eLastError )
        {
            m_dwFileFlag |= ANT_FFG_AttrNotExist;
            m_bFileFlagInited = TRUE;
        }
        return;
    }

    m_dwFileFlag = Ant_WinAttrsToFileFlag(m_dwFileAttrs);

    m_bFileFlagInited = TRUE;
}

LONG Ant_NormalFileEngine::nativeCurPos()
{
    cleanLastError();

    if( INVALID_HANDLE_VALUE == m_hFile )
    {
        setLastError(ANT_FER_FileNotOpen);
        return -1L;
    }

    DWORD dwRet = ::SetFilePointer(m_hFile, 0, NULL, FILE_CURRENT);
    if( INVALID_SET_FILE_POINTER == dwRet )
    {
        setLastError();
        return -1L;
    }
    return (LONG)dwRet;
}

BOOL Ant_NormalFileEngine::nativeIsSequential() const
{
    return FALSE;
}

BOOL Ant_NormalFileEngine::nativeCaseSensitive() const
{
    return FALSE;
}

BOOL Ant_NormalFileEngine::nativeOpen()
{
    cleanLastError();

    // All files are opened in share mode (both read and write).
    DWORD dwShareMode = FILE_SHARE_READ | FILE_SHARE_WRITE;

    DWORD dwDesiredAccess = 0;
    if( m_iOpenMode & ANT_FOM_ReadOnly )
    {
        dwDesiredAccess |= GENERIC_READ;
    }
    if( m_iOpenMode & ANT_FOM_WriteOnly )
    {
        dwDesiredAccess |= GENERIC_WRITE;
    }

    DWORD dwCreationDisp = (m_iOpenMode & ANT_FOM_WriteOnly) ? OPEN_ALWAYS : OPEN_EXISTING;
    m_hFile = ::CreateFile(m_strPath.getString(), dwDesiredAccess, dwShareMode, NULL, dwCreationDisp, FILE_ATTRIBUTE_NORMAL, NULL);
    if (INVALID_HANDLE_VALUE == m_hFile) {
        setLastError();
        m_iOpenMode = ANT_FOM_NotOpen;
        return FALSE;
    }

    BOOL bRet = TRUE;

    while (TRUE) {
        if(m_iOpenMode & ANT_FOM_Truncate) {
            // Set the file's size to zero when in Truncate mode
            if (INVALID_SET_FILE_POINTER == ::SetFilePointer(m_hFile, 0, NULL, FILE_BEGIN)) {
                setLastError();
                bRet = FALSE;
                break;
            }
            if (!::SetEndOfFile(m_hFile)) {
                setLastError();
                bRet = FALSE;
                break;
            }
        }

        break;
    }

    if (FALSE == bRet) {
        ::CloseHandle(m_hFile);
        m_hFile = INVALID_HANDLE_VALUE;
        m_iOpenMode = ANT_FOM_NotOpen;
    }

    return bRet;
}

BOOL Ant_NormalFileEngine::nativeClose()
{
    cleanLastError();

    if (INVALID_HANDLE_VALUE != m_hFile) {
        if (!::CloseHandle(m_hFile)) {
            setLastError();
            return FALSE;
        }
        m_hFile = INVALID_HANDLE_VALUE;
    }

    m_iOpenMode = ANT_FOM_NotOpen;

    return TRUE;
}

DWORD Ant_NormalFileEngine::nativeRead(VOID* pBuf, const DWORD& dwMaxSize)
{
    cleanLastError();

    // check parameters
    if( NULL == pBuf )
    {
        setLastError(ANT_FER_InvalidParam);
        return 0;
    }

    if( INVALID_HANDLE_VALUE == m_hFile )
    {
        setLastError(ANT_FER_FileNotOpen);
        return 0;
    }

    DWORD dwReadSize = 0;
    if( !::ReadFile(m_hFile, pBuf, dwMaxSize, &dwReadSize, NULL) )
    {
        setLastError();
    }

    return dwReadSize;
}

DWORD Ant_NormalFileEngine::nativeWrite(const VOID* pBuf, const DWORD& iSize)
{
    cleanLastError();

    // check parameters
    if( NULL == pBuf || iSize < 0 )
    {
        setLastError(ANT_FER_InvalidParam);
        return 0;
    }

    if( INVALID_HANDLE_VALUE == m_hFile )
    {
        setLastError(ANT_FER_FileNotOpen);
        return 0;
    }

    DWORD dwWriteSize = 0;
    if( !::WriteFile(m_hFile, pBuf, (DWORD)iSize, &dwWriteSize, NULL) )
    {
        //setLastError(ANT_FER_ReadFile);
        setLastError();
    }

    return dwWriteSize;
}

LONG Ant_NormalFileEngine::nativeSeek(const INT& iOffset, const Ant_FileSeekMode& eMode)
{
    cleanLastError();

    if( INVALID_HANDLE_VALUE == m_hFile )
    {
        setLastError(ANT_FER_FileNotOpen);
        return -1L;
    }

    DWORD dwMoveMethod = 0;
    switch (eMode) {

    case ANT_FSM_Begin:
        dwMoveMethod = FILE_BEGIN;
        break;
    case ANT_FSM_Current:
        dwMoveMethod = FILE_CURRENT;
        break;
    case ANT_FSM_End:
        dwMoveMethod = FILE_END;
        break;
    default:
        setLastError(ANT_FER_InvalidParam);
        return -1L;
    }

    DWORD dwRet = ::SetFilePointer(m_hFile, (LONG)iOffset, NULL, dwMoveMethod);
    if (INVALID_SET_FILE_POINTER == dwRet) {
        setLastError();
        return -1L;
    }

    return (LONG)dwRet;
}

BOOL Ant_NormalFileEngine::nativeFlush()
{
    cleanLastError();

    if( INVALID_HANDLE_VALUE == m_hFile )
    {
        setLastError(ANT_FER_FileNotOpen);
        return FALSE;
    }

    if( !::FlushFileBuffers(m_hFile) )
    {
        //setLastError(ANT_FER_FlushBuffer);
        setLastError();
        return FALSE;
    }

    return TRUE;
}

BOOL Ant_NormalFileEngine::nativeAtEnd()
{
    cleanLastError();

    if( INVALID_HANDLE_VALUE == m_hFile )
    {
        setLastError(ANT_FER_FileNotOpen);
        return FALSE;
    }

    BYTE b;
    DWORD dwReadSize = 0;
    BOOL bRet = ::ReadFile(m_hFile, &b, sizeof(b), &dwReadSize, NULL);
    if( bRet && (0 == dwReadSize) )
    {
        return TRUE;
    }

    if( !bRet ) {
        setLastError();
        if( 0 != dwReadSize )
        {
            ::SetFilePointer(m_hFile, 0 - (LONG)dwReadSize, NULL, FILE_CURRENT);
        }
    }
    else
    {
        DWORD dwRet = ::SetFilePointer(m_hFile, 0 - (LONG)dwReadSize, NULL, FILE_CURRENT);
        if( INVALID_SET_FILE_POINTER == dwRet )
        {
            setLastError();
        }
    }

    return FALSE;
}

LONGLONG Ant_NormalFileEngine::nativeSize()
{
    cleanLastError();

    HANDLE hFile = m_hFile;
    BOOL bNeedCloseHandle = FALSE;

    if(INVALID_HANDLE_VALUE == hFile) {
        hFile = ::CreateFile(m_strPath.getString(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        bNeedCloseHandle = TRUE;
    }
    if (INVALID_HANDLE_VALUE == hFile) {
        setLastError();
        return -1L;
    }

    // TODO: replace it with GetFileSizeEx for large file support(Rocky)
    DWORD dwRet = ::GetFileSize(hFile, NULL);
    if (0xFFFFFFFF == dwRet) {
        setLastError();
        if( bNeedCloseHandle ) ::CloseHandle(hFile);
        return -1L;
    }

    if(bNeedCloseHandle) {
        ::CloseHandle(hFile);
    }

    return (LONGLONG)dwRet;
}

BOOL Ant_NormalFileEngine::nativeSetSize(const LONG& lSize)
{
    // TODO: set size when mapping file
    cleanLastError();

    HANDLE hFile = m_hFile;
    BOOL bNeedCloseHandle = FALSE;

    if( INVALID_HANDLE_VALUE == hFile )
    {
        hFile = ::CreateFile(m_strPath.getString(),
                             GENERIC_WRITE,
                             FILE_SHARE_READ | FILE_SHARE_WRITE,
                             NULL,
                             OPEN_EXISTING,
                             FILE_ATTRIBUTE_NORMAL,
                             NULL);
        bNeedCloseHandle = TRUE;
    }

    if( INVALID_HANDLE_VALUE == hFile )
    {
        //setLastError(ANT_FER_OpenFile);
        setLastError();
        return FALSE;
    }

    DWORD dwCurPos = ::SetFilePointer(hFile, 0, NULL, FILE_CURRENT);
    if( INVALID_SET_FILE_POINTER == dwCurPos )
    {
        setLastError();
        if( bNeedCloseHandle ) ::CloseHandle(hFile);
        return FALSE;
    }

    DWORD dwRet = ::SetFilePointer(hFile, (DWORD)lSize, NULL, FILE_BEGIN);
    if( INVALID_SET_FILE_POINTER == dwRet )
    {
        setLastError();
        if( bNeedCloseHandle ) ::CloseHandle(hFile);
        return FALSE;
    }

    if( !::SetEndOfFile(hFile) )
    {
        setLastError();
        if( bNeedCloseHandle ) ::CloseHandle(hFile);
        return FALSE;
    }

    dwRet = ::SetFilePointer(hFile, MIN(dwCurPos, (DWORD)lSize), NULL, FILE_BEGIN);
    if( INVALID_SET_FILE_POINTER == dwRet )
    {
        setLastError();
        if( bNeedCloseHandle ) ::CloseHandle(hFile);
        return FALSE;
    }

    if( bNeedCloseHandle ) ::CloseHandle(hFile);

    return TRUE;
}

BOOL Ant_NormalFileEngine::nativeExists()
{
    cleanLastError();

    HANDLE hFile = ::CreateFile(m_strPath.getString(),
                                GENERIC_READ,
                                FILE_SHARE_READ | FILE_SHARE_WRITE,
                                NULL,
                                OPEN_EXISTING,
                                FILE_ATTRIBUTE_NORMAL,
                                NULL);
    if( INVALID_HANDLE_VALUE == hFile )
    {
        setLastError();
        if( ANT_FER_NotExists == m_eLastError )
        {
                m_dwFileFlag |= ANT_FFG_AttrNotExist;
                m_bFileFlagInited = TRUE;
            setLastError(ANT_FER_NoError);
        }
        return FALSE;
    }
    ::CloseHandle(hFile);

    return TRUE;
}

BOOL Ant_NormalFileEngine::nativeRemove(BOOL bForse)
{
    cleanLastError();

    if( m_bFileFlagInited )
    {
        if( m_dwFileFlag & ANT_FFG_TypeDirectory )
        {
            if( !::RemoveDirectory(m_strPath.getString()) )
            {
                setLastError();
                return FALSE;
            }
        }
        else
        {
            if( !::DeleteFile(m_strPath.getString()) )
            {
                setLastError();
                return FALSE;
            }
        }
    }
    else
    {
        m_dwFileAttrs = ::GetFileAttributes(m_strPath.getString());
        if( 0xFFFFFFFF == m_dwFileAttrs )
        {
            setLastError();
            if( ANT_FER_NotExists == m_eLastError )
            {
                m_dwFileFlag |= ANT_FFG_AttrNotExist;
                m_bFileFlagInited = TRUE;
            }
            return FALSE;
        }

        if( m_dwFileAttrs & FILE_ATTRIBUTE_DIRECTORY )
        {
            if( !::RemoveDirectory(m_strPath.getString()) )
            {
                setLastError();
                return FALSE;
            }
        }
        else
        {
            if( !::DeleteFile(m_strPath.getString()) )
            {
                setLastError();
                return FALSE;
            }
        }
    }

    m_dwFileFlag = ANT_FFG_AttrNotExist;
    m_bFileFlagInited = TRUE;

    return TRUE;
}

BOOL Ant_NormalFileEngine::nativeCopyTo(const Ant_String& strDstName)
{
    cleanLastError();

    if (!::CopyFile(m_strPath.getString(), strDstName.getString(), TRUE)) {
        setLastError();
        if (ANT_FER_NotExists == m_eLastError) {
            m_dwFileFlag |= ANT_FFG_AttrNotExist;
            m_bFileFlagInited = TRUE;
        }
        return FALSE;
    }

    return TRUE;
}

BOOL Ant_NormalFileEngine::nativeMoveTo(const Ant_String& strNewName)
{
    cleanLastError();

    if (!::MoveFile(m_strPath.getString(), strNewName.getString())) {
        setLastError();
        if (ANT_FER_NotExists == m_eLastError) {
            m_dwFileFlag |= ANT_FFG_AttrNotExist;
            m_bFileFlagInited = TRUE;
        }
        return FALSE;
    }

    m_strPath = strNewName;

    return TRUE;
}

BOOL Ant_NormalFileEngine::nativeRename(const Ant_String& strNewName)
{
    return FALSE;
}

BOOL Ant_NormalFileEngine::nativeMakeLink(const Ant_String& strLinkName)
{
    return FALSE;
}

Ant_String Ant_NormalFileEngine::nativeLinkTarget()
{
    return XTEXT("");
}

BOOL Ant_NormalFileEngine::nativeMakeDir(const Ant_String& strDirName)
{
    cleanLastError();

    if (!::CreateDirectory(strDirName.getString(), NULL)) {
        setLastError();
        return FALSE;
    }
    return TRUE;
}

DWORD Ant_NormalFileEngine::nativeFileFlags(const DWORD& dwMask)
{
    cleanLastError();

    m_dwFileAttrs = ::GetFileAttributes(m_strPath.getString());
    if (0xFFFFFFFF == m_dwFileAttrs) {
        setLastError();
        if (ANT_FER_NotExists == m_eLastError) {
            m_dwFileFlag |= ANT_FAB_NotExist;
            m_bFileFlagInited = TRUE;
        }
        return 0;
    }

    m_dwFileFlag = Ant_WinAttrsToFileFlag(m_dwFileAttrs);
    m_bFileFlagInited = TRUE;
    return m_dwFileFlag & dwMask;
}

BOOL Ant_NormalFileEngine::nativeSetPermissions(DWORD dwPerms)
{
    cleanLastError();

    if (dwPerms & ANT_FPM_OwnerWrite) {
        m_dwFileAttrs &= ~FILE_ATTRIBUTE_READONLY;
        if (!::SetFileAttributes(m_strPath.getString(),m_dwFileAttrs)) {
            setLastError();
            return FALSE;
        }
    }
    else if (dwPerms & ANT_FPM_OwnerRead) {
        m_dwFileAttrs |= FILE_ATTRIBUTE_READONLY;
        if (!::SetFileAttributes(m_strPath.getString(), m_dwFileAttrs)) {
            setLastError();
            return FALSE;
        }
    }

    m_dwFileFlag &= ~ANT_FFG_PermsMask;
    m_dwFileFlag |= dwPerms;

    return TRUE;
}

BOOL Ant_NormalFileEngine::nativeSetEndOfFile()
{
	return FALSE;
}

Ant_String Ant_NormalFileEngine::nativeOwnerName(const Ant_FileUser& eKind) const
{
    return XTEXT("");
}

DWORD Ant_NormalFileEngine::nativeOwnerID(const Ant_FileUser& eKind) const
{
    return -1;
}

Ant_Time Ant_NormalFileEngine::nativeFileTime(const Ant_FileTime& eKind)
{
    cleanLastError();

    WIN32_FILE_ATTRIBUTE_DATA sFileInfo = {0};

    if (!::GetFileAttributesEx(m_strPath.getString(), GetFileExInfoStandard, &sFileInfo)) {
        setLastError();
        return Ant_Time();
    }

    FILETIME sTime;
    switch (eKind) {

    case ANT_FTM_Creation:
        sTime = sFileInfo.ftCreationTime;
        break;
    case ANT_FTM_LastWrite:
        sTime = sFileInfo.ftLastWriteTime;
        break;
    case ANT_FTM_LastAccess:
        sTime = sFileInfo.ftLastAccessTime;
        break;
    default:
        setLastError(ANT_FER_InvalidParam);
        return Ant_Time();
    }

    FILETIME sLocalTime;
    if (!::FileTimeToLocalFileTime(&sTime, &sLocalTime)) {
        setLastError();
        return Ant_Time();
    }

    SYSTEMTIME sSysTime;
    if (!::FileTimeToSystemTime(&sLocalTime, &sSysTime)) {
        setLastError();
        return Ant_Time();
    }

    Ant_Time cTime;
    cTime.setDate(sSysTime.wYear, sSysTime.wMonth, sSysTime.wDay);
    cTime.setTime(sSysTime.wHour, sSysTime.wMinute, sSysTime.wSecond, sSysTime.wMilliseconds);

    return cTime;
}

VOID Ant_NormalFileEngine::nativeSetLastError()
{
    DWORD dwErrorCode = ::GetLastError();
    m_eLastError = Ant_ErrFromWinErr(dwErrorCode);
}

Ant_NormalFileEngineIterator::Ant_NormalFileEngineIterator()
: m_strWorkDir()
, m_strCurEntry()
#if defined(_WIN32)||defined(_WIN32_WCE)
, m_hFindHandle(INVALID_HANDLE_VALUE)
#else
, m_pDir(NULL)
#endif
 ,m_eLastError(ANT_FER_NoError)
, m_bEnd(FALSE)
{

}

Ant_NormalFileEngineIterator::Ant_NormalFileEngineIterator(const Ant_String& strDirPath)
: m_strWorkDir(strDirPath)
, m_strCurEntry()
, m_hFindHandle(INVALID_HANDLE_VALUE)
, m_eLastError(ANT_FER_NoError)
, m_bEnd(FALSE)
, m_dwCurFileFlag(0)
{
    init();
}

Ant_NormalFileEngineIterator::~Ant_NormalFileEngineIterator()
{
    if( INVALID_HANDLE_VALUE != m_hFindHandle )
    {
        ::FindClose(m_hFindHandle);
        m_hFindHandle = INVALID_HANDLE_VALUE;
    }
}

Ant_AbstractFileEngineIterator& Ant_NormalFileEngineIterator::operator ++ ()
{
    cleanLastError();

    if (INVALID_HANDLE_VALUE == m_hFindHandle) {
        m_eLastError = ANT_FER_NoMoreFile;
        m_strCurEntry = XTEXT("");
        m_dwCurFileFlag = ANT_FFG_AttrNotExist;
        m_bEnd = TRUE;
        return *this;
    }

    WIN32_FIND_DATA sFindData;
    if (!::FindNextFile(m_hFindHandle, &sFindData)) {
        setLastError();
        m_strCurEntry = XTEXT("");
        m_dwCurFileFlag = ANT_FFG_AttrNotExist;
        m_bEnd = TRUE;
        return *this;
    }

    m_strCurEntry = sFindData.cFileName;
    return *this;
}

BOOL Ant_NormalFileEngineIterator::init()
{
    cleanLastError();

    WIN32_FIND_DATA sFindData;
    Ant_Path cFilePath(m_strWorkDir);
    if (!cFilePath.append(XTEXT("*"))) {
        return FALSE;
    }

    m_hFindHandle = ::FindFirstFile(cFilePath.pathName().getString(), &sFindData);
    if (INVALID_HANDLE_VALUE == m_hFindHandle) {
        setLastError();
        m_strCurEntry = XTEXT("");
        m_dwCurFileFlag = ANT_FFG_AttrNotExist;
        m_bEnd = TRUE;

        return FALSE;
    }

    m_strCurEntry = sFindData.cFileName;
    m_dwCurFileFlag = Ant_WinAttrsToFileFlag(sFindData.dwFileAttributes);

    return TRUE;
}

VOID Ant_NormalFileEngineIterator::setWorkDir(const Ant_String& strDirPath)
{
    cleanLastError();

    if (INVALID_HANDLE_VALUE != m_hFindHandle) {
        ::FindClose(m_hFindHandle);
        m_hFindHandle = INVALID_HANDLE_VALUE;
    }
    m_strWorkDir = strDirPath;

    init();
}

VOID Ant_NormalFileEngineIterator::setLastError()
{
    DWORD dwErrorCode = ::GetLastError();
    m_eLastError = Ant_ErrFromWinErr(dwErrorCode);
}

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
/* EOF */
