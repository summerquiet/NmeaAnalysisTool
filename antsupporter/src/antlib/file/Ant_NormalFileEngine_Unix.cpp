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
#ifndef ANT_DEBUG_H
#   include "Ant_Debug.h"
#endif

#include <time.h>
#include <string.h>

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Value define

#define PATH_SEPARATOR              XTEXT("/")
#define TEMP_BUFF_SIZE              (8 * 1024)

const mode_t ANT_DEFAULT_MODE_FILE  = 0644;
const mode_t ANT_DEFAULT_MODE_DIR   = 0755;

/*---------------------------------------------------------------------------*/
// Functions
static Ant_String Ant_OpenModeToFopenMode(const INT& iOpenMode, const BOOL& bFileExist)
{
    if ((iOpenMode & ANT_FOM_ReadOnly) &&
        !(iOpenMode & ANT_FOM_WriteOnly)) {
        return XTEXT("r");
    }

    Ant_String strFopenMode = XTEXT("");
    if (bFileExist) {
        strFopenMode = XTEXT("r+");
    }
    else {
        strFopenMode = XTEXT("w");
        if (iOpenMode & ANT_FOM_ReadOnly) {
            strFopenMode += XTEXT("+");
        }
    }

    return strFopenMode;
}

static INT Ant_OpenModeToOpenFlags(const INT& iOpenMode)
{
    INT iOpenFlag = 0;
    if (ANT_FOM_ReadWrite == (iOpenMode & ANT_FOM_ReadWrite)) {
        iOpenFlag = O_RDWR | O_CREAT;
    }
    else if (iOpenMode & ANT_FOM_WriteOnly) {
        iOpenFlag = O_WRONLY | O_CREAT;
    }
    else {
        iOpenFlag = O_RDONLY;
    }
    return iOpenFlag;
}

static DWORD Ant_UnixModeToFileFlag(const mode_t& fm)
{
    DWORD dwFileFlags = 0;

    if (fm & S_IRUSR) {
        dwFileFlags |= ANT_FFG_PermOwnerRead;
    }
    if (fm & S_IWUSR){
        dwFileFlags |= ANT_FFG_PermOwnerWrite;
    }
    if (fm & S_IXUSR) {
        dwFileFlags |= ANT_FFG_PermOwnerExe;
    }
    if (fm & S_IRGRP) {
        dwFileFlags |= ANT_FFG_PermGroupRead;
    }
    if (fm & S_IWGRP) {
        dwFileFlags |= ANT_FFG_PermGroupWrite;
    }
    if (fm & S_IXGRP) {
        dwFileFlags |= ANT_FFG_PermGroupExe;
    }
    if (fm & S_IROTH) {
        dwFileFlags |= ANT_FFG_PermOtherRead;
    }
    if (fm & S_IWOTH) {
        dwFileFlags |= ANT_FFG_PermOtherWrite;
    }
    if (fm & S_IXOTH) {
        dwFileFlags |= ANT_FFG_PermOtherExe;
    }

    if (S_ISREG(fm)) {
        dwFileFlags |= ANT_FFG_TypeFile;
    }
    else if (S_ISDIR(fm)) {
        dwFileFlags |= ANT_FFG_TypeDirectory;
    }
    else if (S_ISLNK(fm)) {
        dwFileFlags |= ANT_FFG_TypeLink;
    }
    else if (S_ISCHR(fm) || S_ISBLK(fm)) {
        dwFileFlags |= ANT_FFG_TypeDevice;
    }
    else if (S_ISFIFO(fm)) {
        dwFileFlags |= ANT_FFG_TypePipe;
    }
    else if (S_ISSOCK(fm)) {
        dwFileFlags |= ANT_FFG_TypeSocket;
    }
    else {
        dwFileFlags |= ANT_FFG_AttrNormal;
    }

    return dwFileFlags;
}

static Ant_FileError Ant_ErrorFromErrno(const int& en)
{
    Ant_FileError eLastError = ANT_FER_NoError;
    switch( en )
    {
    case 0:
        eLastError = ANT_FER_NoError;
        break;
    case ENOENT:
    case ENODEV:
    case ENXIO:
        eLastError = ANT_FER_NotExists;
        break;
    case EPERM:
    case EACCES:
    case EROFS:
        eLastError = ANT_FER_NoAccess;
        break;
    case EEXIST:
        eLastError = ANT_FER_AlreadyExists;
        break;
    case ENOTDIR:
        eLastError = ANT_FER_IsNotDirectory;
        break;
    case EISDIR:
        eLastError = ANT_FER_IsDirectory;
        break;
    case EINVAL:
    case ENAMETOOLONG:
        eLastError = ANT_FER_InvalidParam;
        break;
    case ENOSPC:
        eLastError = ANT_FER_NoSpace;
        break;
    case ENOTEMPTY:
        eLastError = ANT_FER_NotEmpty;
        break;
    case EBUSY:
        eLastError = ANT_FER_IsUsed;
        break;
    case ENOMEM:
        eLastError = ANT_FER_NoMemory;
        break;
    default:
        eLastError = ANT_FER_Unknown;
    }

    return eLastError;
}

static BOOL Ant_FileIsUsedByOthers(const Ant_String& strFilePath)
{
    INT fd = ANT_OPEN(strFilePath.getString(), O_RDONLY, ANT_DEFAULT_MODE_FILE);
    if (-1 == fd) {
        return FALSE;
    }

    if (-1 == ANT_FLOCK(fd, LOCK_EX | LOCK_NB)) {
        return TRUE;
    }
    ANT_FLOCK(fd, LOCK_UN);
    ANT_CLOSE(fd);
    return FALSE;
}

BOOL Ant_NormalFileEngine::nativeIsSequential() const
{
    return FALSE;
}

BOOL Ant_NormalFileEngine::nativeCaseSensitive() const
{
    return TRUE;
}

VOID Ant_NormalFileEngine::nativeInitFileFlag()
{
    cleanLastError();

    INT fd = -1;
    if (NULL != m_fp) {
        fd = ANT_FILENO(m_fp);
    }
    else if (-1 != m_fd) {
        fd = m_fd;
    }

    struct stat sbuf;
    if (-1 != fd) {
        if(-1 == ANT_FSTAT(fd, &sbuf) )
        {
            setLastError();
            return;
        }
    }
    else if (-1 == ANT_LSTAT(m_strPath.getString(), &sbuf)) {
        setLastError();
        if (ANT_FER_NotExists == m_eLastError)     {
            m_dwFileFlag |= ANT_FFG_AttrNotExist;
            m_bFileFlagInited = TRUE;
        }
        return;
    }

    m_dwFileFlag = Ant_UnixModeToFileFlag(sbuf.st_mode);
    m_bFileFlagInited = TRUE;
}

BOOL Ant_NormalFileEngine::nativeOpen()
{
    cleanLastError();

    if (!(m_iOpenMode & ANT_FOM_NoBuffer)) {
        // use C style io to implement buffer io
        Ant_String strOpenMode = Ant_OpenModeToFopenMode(m_iOpenMode, exists());
        m_fp = ANT_FOPEN(m_strPath.getString(), strOpenMode.getString());
        if (NULL == m_fp)     {
            setLastError();
            m_iOpenMode = ANT_FOM_NotOpen;
            return FALSE;
        }

        initFileFlag();

        if (m_iOpenMode & ANT_FOM_Truncate)     {    // truncate the file size to zero
            if (-1 == ANT_FTRUNCATE(ANT_FILENO(m_fp), 0))         {
                setLastError();
                ANT_FCLOSE(m_fp);
                m_fp = NULL;
                m_iOpenMode = ANT_FOM_NotOpen;
                return FALSE;
            }
        }
        else if (m_iOpenMode & ANT_FOM_Append)     {    // set the file pointer to the end of the file
            if (-1 == ANT_FSEEK(m_fp, 0, SEEK_END))         {
                setLastError();
                ANT_FCLOSE(m_fp);
                m_fp = NULL;
                m_iOpenMode = ANT_FOM_NotOpen;
                return FALSE;
            }
        }
    }
    else
    {
        INT iOpenFlags = Ant_OpenModeToOpenFlags(m_iOpenMode);
        m_fd = ANT_OPEN(m_strPath.getString(), iOpenFlags, ANT_DEFAULT_MODE_FILE);
        if (-1 == m_fd)     {
            setLastError();
            m_iOpenMode = ANT_FOM_NotOpen;
            return FALSE;
        }

        initFileFlag();

        if (m_iOpenMode & ANT_FOM_Truncate)     {    // truncate the file size to zero
            if (-1 == ANT_FTRUNCATE(m_fd, 0))         {
                setLastError();
                ANT_CLOSE(m_fd);
                m_fd = -1;
                m_iOpenMode = ANT_FOM_NotOpen;
                return FALSE;
            }
        }
        else if (m_iOpenMode & ANT_FOM_Append)     {    // set the file pointer to the end of the file
            if (-1 == ANT_LSEEK(m_fd, 0, SEEK_END))         {
                setLastError();
                ANT_CLOSE(m_fd);
                m_fd = -1;
                m_iOpenMode = ANT_FOM_NotOpen;
                return FALSE;
            }
        }
    }

    return TRUE;
}

BOOL Ant_NormalFileEngine::nativeClose()
{
    cleanLastError();

    if (m_fp) {
        if (EOF == ANT_FCLOSE(m_fp))     {
            m_fp = NULL;
            setLastError();
            m_fp = NULL;
            return FALSE;
        }
        m_fp = NULL;
    }
    else if (-1 != m_fd) {
        if (-1 == ANT_CLOSE(m_fd))     {
            m_fd = -1;
            setLastError();
            return FALSE;
        }
        m_fd = -1;
    }

    m_iOpenMode = ANT_FOM_NotOpen;

    return TRUE;
}

DWORD Ant_NormalFileEngine::nativeRead(VOID* pBuf, const DWORD& dwMaxSize)
{
    cleanLastError();

    // check parameters
    if (NULL == pBuf) {
        setLastError(ANT_FER_InvalidParam);
        return -1;
    }

    DWORD dwRet = 0;
    if (m_fp) {
        size_t res = ANT_FREAD(pBuf, dwMaxSize, m_fp);
        if (dwMaxSize != res && !ANT_FEOF(m_fp))     {
            setLastError();
        }
        dwRet = res;
    }
    else if (-1 != m_fd) {
        ssize_t res = ANT_READ(m_fd, pBuf, dwMaxSize);
        if (-1 == res)     {
            setLastError();
            dwRet = 0;
        }
        else
        {
            dwRet = res;
        }
    }
    else
    {
        setLastError(ANT_FER_FileNotOpen);
    }

    return dwRet;
}

DWORD Ant_NormalFileEngine::nativeWrite(const VOID* pBuf, const DWORD& dwSize)
{
    cleanLastError();

    // check parameters
    if (NULL == pBuf) {
        setLastError(ANT_FER_InvalidParam);
        return -1;
    }

    DWORD dwRet = 0;
    if (m_fp) {
        size_t res = ANT_FWRITE(pBuf, dwSize, m_fp);
        if (dwSize != res)     {
            setLastError();
        }
        dwRet = res;
    }
    else if (-1 != m_fd) {
        ssize_t res = ANT_WRITE(m_fd, const_cast<VOID*>(pBuf), dwSize);
        if (-1 == res)     {
            setLastError();
            dwRet = 0;
        }
        else
        {
            dwRet = res;
        }
    }
    else
    {
        setLastError(ANT_FER_FileNotOpen);
    }

    return dwRet;
}

LONG Ant_NormalFileEngine::nativeSeek(const INT& iOffset, const Ant_FileSeekMode& eMode)
{
    cleanLastError();

    INT whence = 0;
    switch(eMode) {
    
    case ANT_FSM_Begin:
        whence = SEEK_SET;
        break;
    case ANT_FSM_Current:
        whence = SEEK_CUR;
        break;
    case ANT_FSM_End:
        whence = SEEK_END;
        break;
    default:
        setLastError(ANT_FER_InvalidParam);
        return -1L;
    }

    LONG lRet = 0;
    if (m_fp) {
        if (-1 == ANT_FSEEK(m_fp, iOffset, whence)) {
            setLastError();
            return -1L;
        }
        lRet = ANT_FTELL(m_fp);
        if (-1 == lRet) {
            setLastError();
        }
    }
    else if (-1 != m_fd) {
        off_t res = ANT_LSEEK(m_fd, iOffset, whence);
        if ((off_t)-1 == res) {
            setLastError();
        }
        lRet = (LONG)res;
    }
    else {
        setLastError(ANT_FER_FileNotOpen);
        return -1L;
    }

    return lRet;
}

BOOL Ant_NormalFileEngine::nativeFlush()
{
    cleanLastError();

    if (m_fp && EOF == ANT_FFLUSH(m_fp)) {
        setLastError();
        return FALSE;
    }
    else if (-1 != m_fd && -1 == ANT_FSYNC(m_fd)) {
        setLastError();
        return FALSE;
    }
    else {
        setLastError(ANT_FER_FileNotOpen);
        return FALSE;
    }

    return TRUE;
}

LONGLONG Ant_NormalFileEngine::nativeSize()
{
    cleanLastError();

    struct stat sbuf;
    if (m_fp) {
        // When the file is opened in read-only mode,
        // call fflush failed, so I comments this code.
        //if (EOF == ANT_FFLUSH(m_fp))     //{
        //    setLastError();
        //    return -1L;
        //}
        int fd = ANT_FILENO(m_fp);
        if (-1 == ANT_FSTAT(fd, &sbuf))     {
            setLastError();
            return -1L;
        }
    }
    else if (-1 != m_fd && -1 == ANT_FSTAT(m_fd, &sbuf)) {
        setLastError();
        return -1L;
    }
    else if (-1 == ANT_LSTAT(m_strPath.getString(), &sbuf)) {
        setLastError();
        if (ANT_FER_NotExists == m_eLastError)     {
            m_dwFileFlag |= ANT_FFG_AttrNotExist;
            m_bFileFlagInited = TRUE;
        }
        return -1L;
    }

    return sbuf.st_size;
}

BOOL Ant_NormalFileEngine::nativeSetSize(const LONG& lSize)
{
    cleanLastError();

    if (ANT_FOM_NotOpen != m_iOpenMode) {
        if (!(ANT_FOM_WriteOnly & m_iOpenMode)) {
            setLastError(ANT_FER_NoAccess);
            return FALSE;
        }

        if (m_fp)     {
            LONG lOrgPos = ANT_FTELL(m_fp);
            if (-1 == lOrgPos)         {
                setLastError();
                return FALSE;
            }
            if (-1 == lOrgPos |             -1 == ANT_FTRUNCATE(ANT_FILENO(m_fp), lSize) ||
                -1 == ANT_FSEEK(m_fp, MIN(lOrgPos, lSize), SEEK_SET) )
            {
                setLastError();
                return FALSE;
            }
        }
        else if (-1 != m_fd)     {
            off_t org_pos = ANT_LSEEK(m_fd, 0, SEEK_CUR);
            if ((off_t)-1 == org_pos |             -1 == ANT_FTRUNCATE(m_fd, lSize) ||
                (off_t)-1 == ANT_LSEEK(m_fd, MIN(org_pos, lSize), SEEK_SET) )
            {
                setLastError();
                return FALSE;
            }
        }
        else
        {
            setLastError(ANT_FER_FileNotOpen);
            return FALSE;
        }
    }
    else
    {
        // there's no truncate function on Android
        BOOL bRet = TRUE;
        do {
            INT fd = ANT_OPEN(m_strPath.getString(), O_WRONLY, ANT_DEFAULT_MODE_FILE);
            if (-1 == fd)         {
                setLastError();
                bRet = FALSE;
                break;
            }

            if (-1 == ANT_FTRUNCATE(fd, lSize)) {
                setLastError();
                bRet = FALSE;
            }
            ANT_CLOSE(fd);
        } while( FALSE );

        if (!bRet && ANT_FER_NotExists == m_eLastError) {
            m_dwFileFlag |= ANT_FFG_AttrNotExist;
            m_bFileFlagInited = TRUE;
        }
        return bRet;
    }

    return TRUE;
}

BOOL Ant_NormalFileEngine::nativeAtEnd()
{
    cleanLastError();

    if (m_fp) {
        BYTE b = 0;
        size_t res = ANT_FREAD(&b, sizeof(b), m_fp);
        if ((sizeof(b) != res) && ANT_FEOF(m_fp))     {
            return TRUE;
        }

        if (sizeof(b) != res)     {
            setLastError();
            return FALSE;
        }
        else
        {
            if (-1 == ANT_FSEEK(m_fp, 0 - (long)res, SEEK_CUR))         {
                setLastError();
                return FALSE;
            }
        }
    }
    else if (-1 != m_fd) {
        BYTE b = 0;
        ssize_t res = ANT_READ(m_fd, &b, sizeof(b));
        if (0 == res)     {
            return TRUE;
        }

        if (-1 == res)     {
            setLastError();
            return FALSE;
        }
        else
        {
            if (-1 == ANT_LSEEK(m_fd, 0 - (long)res, SEEK_CUR))         {
                setLastError();
                return FALSE;
            }
        }
    }
    else
    {
        setLastError(ANT_FER_FileNotOpen);
    }

    return FALSE;
}

BOOL Ant_NormalFileEngine::nativeExists()
{
    return ( 0 == ANT_ACCESS(m_strPath.getString(), F_OK) ) ? TRUE : FALSE;
}

LONG Ant_NormalFileEngine::nativeCurPos()
{
    cleanLastError();

    LONG ret = 0;
    if (m_fp) {
        ret = ANT_FTELL(m_fp);
    }
    else if (-1 != m_fd) {
        ret = ANT_LSEEK(m_fd, 0, SEEK_CUR);
    }
    else
    {
        setLastError(ANT_FER_FileNotOpen);
        return -1L;
    }

    if (-1 == ret) {
        setLastError();
    }

    return ret;
}

BOOL Ant_NormalFileEngine::nativeRemove(BOOL bForse)
{
    cleanLastError();

    if (Ant_FileIsUsedByOthers(m_strPath)) {
        setLastError(ANT_FER_IsUsed);
        return FALSE;
    }

    Ant_Path cPath(m_strPath);
    struct stat sbuf;

    // get the infomation of the file, if fail, we may not have the permission
    // of searching for the file.
    if (-1 == ANT_LSTAT(m_strPath.getString(), &sbuf)) {
        setLastError();
        return FALSE;
    }

    //as link the result is for the dereferent file, do not check
    if (!S_ISLNK(sbuf.st_mode)) {
        // unable to write ,the file is readonly, if not force flag was set
        // we do not delete the readonly file
        if (ANT_ACCESS(m_strPath.getString(), W_OK) < 0) {
            if (!bForse) {
                setLastError();
                return FALSE;
            }
        }
    }

    if (m_bFileFlagInited) {
        if (m_dwFileFlag & ANT_FFG_TypeDirectory)     {
            if (-1 == ANT_RMDIR(m_strPath.getString()))         {
                setLastError();
                return FALSE;
            }
        }
        else if (-1 == ANT_UNLINK(m_strPath.getString()))     {
            setLastError();
            if (bForse)         return FALSE;
        }
    }
    else
    {
        struct stat sbuf;
        if (-1 == ANT_LSTAT(m_strPath.getString(), &sbuf))     {
            setLastError();
            if (ANT_FER_NotExists == m_eLastError)         {
                m_dwFileFlag |= ANT_FFG_AttrNotExist;
                m_bFileFlagInited = TRUE;
            }
            return FALSE;
        }

        if (S_ISDIR(sbuf.st_mode))     {
            if (-1 == ANT_RMDIR(m_strPath.getString()))         {
                setLastError();
                return FALSE;
            }
        }
        else if (-1 == ANT_UNLINK(m_strPath.getString()))     {
            setLastError();
            return FALSE;
        }
    }

    m_dwFileFlag = ANT_FFG_AttrNotExist;
    m_bFileFlagInited = TRUE;

    return TRUE;
}

BOOL Ant_NormalFileEngine::nativeCopyTo(const Ant_String& strDstName)
{
    cleanLastError();

    BYTE* pTempBuff = new BYTE[TEMP_BUFF_SIZE];
    if (NULL == pTempBuff) {
        setLastError(ANT_FER_NoMemory);
        return FALSE;
    }

    INT iSrcFileDes = ANT_OPEN(m_strPath.getString(),
            O_RDONLY, ANT_DEFAULT_MODE_FILE);
    if (-1 == iSrcFileDes) {
        setLastError();
        delete[] pTempBuff;
        return FALSE;
    }

    struct stat sbuf;
    if (-1 == ANT_FSTAT(iSrcFileDes, &sbuf)) {
        setLastError();
        delete[] pTempBuff;
        ANT_CLOSE(iSrcFileDes);
        return FALSE;
    }

    INT iDstFileDes = ANT_OPEN(strDstName.getString(),
            O_CREAT | O_WRONLY | O_EXCL, ANT_DEFAULT_MODE_FILE);
    if (-1 == iDstFileDes) {
        setLastError();
        delete[] pTempBuff;
        ANT_CLOSE(iSrcFileDes);
        return FALSE;
    }

    ssize_t iReadSize = 0;
    BOOL bRet = TRUE;
    while( (iReadSize = ANT_READ(iSrcFileDes, pTempBuff, TEMP_BUFF_SIZE)) > 0 )
    {
        if (iReadSize != ANT_WRITE(iDstFileDes, pTempBuff, iReadSize))     {
            setLastError();
            bRet = FALSE;
            break;
        }
    }

    if (-1 == iReadSize) {
        setLastError();
        bRet = FALSE;
    }

    delete[] pTempBuff;
    pTempBuff = NULL;

    if (-1 == ANT_CLOSE(iSrcFileDes)) {
        setLastError();
        bRet = FALSE;
    }
    iSrcFileDes = -1;
    if (-1 == ANT_CLOSE(iDstFileDes)) {
        setLastError();
        bRet = FALSE;
    }
    iDstFileDes = -1;

    if (bRet) {
        if (-1 == ANT_CHMOD(strDstName.getString(), sbuf.st_mode)
            || -1 == ANT_CHOWN(strDstName.getString(), sbuf.st_uid, sbuf.st_gid)) {
            setLastError();
            bRet = FALSE;
        }
    }

    return bRet;
}

BOOL Ant_NormalFileEngine::nativeMoveTo(const Ant_String& strNewName)
{
    cleanLastError();

    if (Ant_FileIsUsedByOthers(m_strPath)) {
        setLastError(ANT_FER_IsUsed);
        return FALSE;
    }

    if (0 == ANT_ACCESS(strNewName.getString(), F_OK)) {
        setLastError(ANT_FER_AlreadyExists);
        return FALSE;
    }

    Ant_Path cPath(m_strPath);
    struct stat sbuf;
    if (-1 == ANT_LSTAT(cPath.upperDir(), &sbuf)
        || -1 == ANT_CHMOD(cPath.upperDir(), ANT_DEFAULT_MODE_DIR)) {
        setLastError();
        return FALSE;
    }

    Ant_Path cNewPath(strNewName);
    struct stat snewbuf;
    if (-1 == ANT_LSTAT(cNewPath.upperDir(), &snewbuf) 
        || -1 == ANT_CHMOD(cNewPath.upperDir(), ANT_DEFAULT_MODE_DIR)) {
        setLastError();
        return FALSE;
    }

    if (-1 == ::ANT_RENAME(m_strPath.getString(), strNewName.getString())) {
        setLastError();
        if (ANT_FER_NotExists == m_eLastError)     {
            m_dwFileFlag |= ANT_FFG_AttrNotExist;
            m_bFileFlagInited = TRUE;
        }
        ANT_CHMOD(cPath.upperDir(), sbuf.st_mode);
        ANT_CHMOD(cNewPath.upperDir(), snewbuf.st_mode);
        return FALSE;
    }

    m_strPath = strNewName;
    initFileFlag();
    ANT_CHMOD(cPath.upperDir(), sbuf.st_mode);
    ANT_CHMOD(cNewPath.upperDir(), snewbuf.st_mode);

    return TRUE;
}

BOOL Ant_NormalFileEngine::nativeRename(const Ant_String& strNewName)
{
    cleanLastError();

    if (Ant_FileIsUsedByOthers(m_strPath)) {
        setLastError(ANT_FER_IsUsed);
        return FALSE;
    }
    if (-1 == ::ANT_RENAME(m_strPath.getString(), strNewName.getString())) {
        setLastError();
        if (ANT_FER_NotExists == m_eLastError)     {
            m_dwFileFlag |= ANT_FFG_AttrNotExist;
            m_bFileFlagInited = TRUE;
        }
        return FALSE;
    }

    m_strPath = strNewName;
    initFileFlag();

    return TRUE;
}

BOOL Ant_NormalFileEngine::nativeMakeLink(const Ant_String& strLinkName)
{
    cleanLastError();

    return FALSE;
}

Ant_String Ant_NormalFileEngine::nativeLinkTarget()
{
    cleanLastError();

    return XTEXT("");
}

BOOL Ant_NormalFileEngine::nativeMakeDir(const Ant_String& strDirName)
{
    cleanLastError();

    if (-1 == ANT_MKDIR(strDirName.getString(), ANT_DEFAULT_MODE_DIR)) {
        setLastError();
        return FALSE;
    }
    return TRUE;
}

DWORD Ant_NormalFileEngine::nativeFileFlags(const DWORD& dwMask)
{
    cleanLastError();

    INT fd = -1;
    if (NULL != m_fp) {
        fd = ANT_FILENO(m_fp);
    }
    else if (-1 != m_fd) {
        fd = m_fd;
    }

    struct stat sbuf;
    if (-1 != fd &     -1 == ANT_FSTAT(fd, &sbuf) )
    {
        setLastError();
        return 0;
    }
    else if (-1 == ANT_LSTAT(m_strPath.getString(), &sbuf)) {
        setLastError();
        if (ANT_FER_NotExists == m_eLastError)     {
            m_dwFileFlag |= ANT_FFG_AttrNotExist;
            m_bFileFlagInited = TRUE;
        }
        return 0;
    }

    m_dwFileFlag = Ant_UnixModeToFileFlag(sbuf.st_mode);
    m_bFileFlagInited = TRUE;

    return m_dwFileFlag & dwMask;
}

BOOL Ant_NormalFileEngine::nativeSetPermissions(DWORD dwPerms)
{
    cleanLastError();

    mode_t mode = 0;
    if (dwPerms & ANT_FPM_OwnerRead) {
        mode |= S_IRUSR;
    }
    if (dwPerms & ANT_FPM_OwnerWrite) {
        mode |= S_IWUSR;
    }
    if (dwPerms & ANT_FPM_OwnerExe) {
        mode |= S_IXUSR;
    }
    if (dwPerms & ANT_FPM_GroupRead) {
        mode |= S_IRGRP;
    }
    if (dwPerms & ANT_FPM_GroupWrite) {
        mode |= S_IWGRP;
    }
    if (dwPerms & ANT_FPM_GroupExe) {
        mode |= S_IXGRP;
    }
    if (dwPerms & ANT_FPM_OtherRead) {
        mode |= S_IROTH;
    }
    if (dwPerms & ANT_FPM_OtherWrite) {
        mode |= S_IWOTH;
    }
    if (dwPerms & ANT_FPM_OtherExe) {
        mode |= S_IXOTH;
    }
    
    INT fd = -1;
    if (NULL != m_fp) {
        fd = ANT_FILENO(m_fp);
    }
    else if (-1 != m_fd) {
        fd = m_fd;
    }

    if (-1 != fd
        && -1 == ANT_FCHMOD(fd, mode)) {
        setLastError();
        return 0;
    }
    else if (-1 == ANT_CHMOD(m_strPath.getString(), mode)) {
        setLastError();
        if (ANT_FER_NotExists == m_eLastError) {
            m_dwFileFlag |= ANT_FFG_AttrNotExist;
            m_bFileFlagInited = TRUE;
        }
        return FALSE;
    }

    m_dwFileFlag &= ~ANT_FFG_PermsMask;
    m_dwFileFlag |= dwPerms;

    return TRUE;
}

BOOL Ant_NormalFileEngine::nativeSetEndOfFile()
{
    cleanLastError();

    LONG cur_pos = 0;
    if (m_fp) {
        cur_pos = ANT_FTELL(m_fp);
    }
    else if (-1 != m_fd) {
        cur_pos = ANT_LSEEK(m_fd, 0, SEEK_CUR);
    }
    if (-1 == cur_pos) {
        setLastError();
        return FALSE;
    }
    INT ret = ANT_TRUNCATE(m_strPath.getString(), cur_pos);
    if (-1 == ret) {
        setLastError();
        return FALSE;
    }

    return TRUE;
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

    INT fd = -1;
    if (NULL != m_fp) {
        fd = ANT_FILENO(m_fp);
    }
    else if (-1 != m_fd) {
        fd = m_fd;
    }

    struct stat sbuf;
    if (-1 != fd 
        && -1 == ANT_FSTAT(fd, &sbuf)) {
        setLastError();
        return Ant_Time();
    }
    else if (-1 == ANT_LSTAT(m_strPath.getString(), &sbuf)) {
        setLastError();
        if (ANT_FER_NotExists == m_eLastError) {
            m_dwFileFlag |= ANT_FFG_AttrNotExist;
            m_bFileFlagInited = TRUE;
        }
        return Ant_Time();
    }

    time_t time = 0;
    switch (eKind) {

    case ANT_FTM_Creation:
        time = sbuf.st_ctime;
        break;
    case ANT_FTM_LastAccess:
        time = sbuf.st_atime;
        break;
    case ANT_FTM_LastWrite:
        time = sbuf.st_mtime;
        break;
    default:
        setLastError(ANT_FER_InvalidParam);
        return Ant_Time();
    }

    struct tm* pTime = ::localtime(&time);
    if (NULL == pTime) {
        setLastError();
        return Ant_Time();
    }

    Ant_Time cTime;
    cTime.setDate(pTime->tm_year, pTime->tm_mon, pTime->tm_mday);
    cTime.setTime(pTime->tm_hour, pTime->tm_min, pTime->tm_sec, 0);

    return cTime;
}

VOID Ant_NormalFileEngine::nativeSetLastError()
{
    int en = errno;
    m_eLastError = Ant_ErrorFromErrno(en);
    ANT_PERROR("[Ant_File]err: %d - %s", errno, strerror(errno));
}

Ant_NormalFileEngineIterator::Ant_NormalFileEngineIterator()
: m_strWorkDir()
, m_strCurEntry()
, m_pDir(NULL)
, m_eLastError(ANT_FER_NoError)
, m_bEnd(FALSE)
, m_dwCurFileFlag(0)
{

}

Ant_NormalFileEngineIterator::Ant_NormalFileEngineIterator(const Ant_String& strDirPath)
: m_strWorkDir(strDirPath)
, m_strCurEntry()
, m_pDir(NULL)
, m_eLastError(ANT_FER_NoError)
, m_bEnd(FALSE)
, m_dwCurFileFlag(0)
{
    init();
}

Ant_NormalFileEngineIterator::~Ant_NormalFileEngineIterator()
{
    if (NULL != m_pDir) {
        ANT_CLOSEDIR(m_pDir);
        m_pDir = NULL;
    }
}

Ant_AbstractFileEngineIterator& Ant_NormalFileEngineIterator::operator ++ ()
{
    cleanLastError();

    if (NULL == m_pDir) {
        m_eLastError = ANT_FER_NoMoreFile;
        m_strCurEntry = XTEXT("");
        m_bEnd = TRUE;
        return *this;
    }

    INT iErrNo = errno;
    struct dirent* pRes = ANT_READDIR(m_pDir);
    if (NULL == pRes) {
        if (iErrNo != errno) {
            setLastError();
        }
        m_bEnd = TRUE;
        m_strCurEntry = XTEXT("");
        return *this;
    }
    m_strCurEntry = pRes->d_name;

    Ant_String fullpath = m_strWorkDir +PATH_SEPARATOR+ m_strCurEntry;
    if (!initFileFlags(fullpath)) {
        //error happend, but we can do nothing, file maybe delete
        //TODO: change for security
    }

    return *this;
}

BOOL Ant_NormalFileEngineIterator::initFileFlags(const Ant_String &path)
{
    cleanLastError();
    struct stat sbuf;
    if (-1 == ANT_LSTAT(path.getString(), &sbuf)) {
        setLastError();
        if (ANT_FER_NotExists == m_eLastError) {
            m_dwCurFileFlag |= ANT_FFG_AttrNotExist;
        }
        return FALSE;
    }

    m_dwCurFileFlag = Ant_UnixModeToFileFlag(sbuf.st_mode);
    return TRUE;
}

BOOL Ant_NormalFileEngineIterator::init()
{
    cleanLastError();

    m_pDir = ANT_OPENDIR(m_strWorkDir.getString());
    if (NULL == m_pDir) {
        setLastError();
        m_bEnd = TRUE;
        m_strCurEntry = XTEXT("");
        return FALSE;
    }

    INT iErrNo = errno;
    struct dirent* pRes = ANT_READDIR(m_pDir);
    if (NULL == pRes) {
        BOOL bRet = TRUE;
        if (iErrNo != errno)     {
            setLastError();
            bRet = FALSE;
        }
        m_bEnd = TRUE;
        m_strCurEntry = XTEXT("");
        return bRet;
    }
    m_strCurEntry = pRes->d_name;
    Ant_String fullpath = m_strWorkDir +PATH_SEPARATOR+ m_strCurEntry;
    if (!initFileFlags(fullpath)) {
        //here we close to dir to avoid fileno leak
        ANT_CLOSEDIR(m_pDir);
        m_pDir = NULL;
        return FALSE;
    }

    return TRUE;
}

VOID Ant_NormalFileEngineIterator::setWorkDir(const Ant_String& strDirPath)
{
    cleanLastError();

    if (NULL != m_pDir) {
        ANT_CLOSEDIR(m_pDir);
        m_pDir = NULL;
    }
    m_strWorkDir = strDirPath;

    init();
}

VOID Ant_NormalFileEngineIterator::setLastError()
{
    int en = errno;
    m_eLastError = Ant_ErrorFromErrno(en);
}

#undef TEMP_BUFF_SIZE

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
/* EOF */
