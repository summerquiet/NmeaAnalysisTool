/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_NORMALFILEENGINE_H
#define ANT_NORMALFILEENGINE_H
#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_ABSTRACTFILEENGINE_H
#   include "Ant_AbstractFileEngine.h"
#endif
#if !defined(_WIN32) && !defined(_WIN32_WCE)
#   include <stdio.h>
#   include <sys/types.h>
#   include <dirent.h>
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// class declare

/**
 * @class Ant_NormalFileEngine
 * @brief The class to manipulate normal files
 */
class Ant_NormalFileEngine : public Ant_AbstractFileEngine
{
public:

    //explicit Ant_NormalFileEngine(const BOOL& bRecLog);

    /**
     * The \a strFilePath should be an absolute path
     */
    Ant_NormalFileEngine(const Ant_String& strFilePath, const BOOL& bRecLog);
    virtual ~Ant_NormalFileEngine();

    virtual BOOL isSequential() const;
    virtual BOOL caseSensitive() const;
    virtual BOOL open(INT iOpenMode);
    virtual BOOL close();
    virtual BOOL isOpen() {return ANT_FOM_NotOpen == m_iOpenMode ? FALSE : TRUE;}
    virtual DWORD read(VOID* pBuf, const DWORD& dwMaxSize);
    virtual DWORD write(const VOID* pBuf, const DWORD& dwSize);
    virtual LONG seek(const INT& iOffset, const Ant_FileSeekMode& eMode);
    virtual BOOL flush();
    virtual BOOL atEnd();
    virtual LONGLONG size();
    virtual BOOL setSize(const LONG& lSize);
    virtual BOOL exists();
    virtual BOOL remove(BOOL bForse);
    virtual BOOL copyTo(const Ant_String& strDstName);
    virtual BOOL rename(const Ant_String& strNewName);
    virtual BOOL moveTo(const Ant_String& strNewName);
    virtual BOOL makeLink(const Ant_String& strLinkName);
    virtual Ant_String linkTarget();
    virtual BOOL makeDir(const Ant_String& strDirName);
    virtual Iterator* entryIterator();
    virtual DWORD fileFlags(const DWORD& dwMask);
    virtual BOOL setPermissions(const DWORD& dwPerms);
    virtual BOOL setEndOfFile();
    virtual Ant_String ownerName(const Ant_FileUser& eKind) const;
    virtual DWORD ownerID(const Ant_FileUser& eKind) const;
    virtual Ant_Time fileTime(const Ant_FileTime& eKind);
    virtual Ant_FileError getLastError() const {return m_eLastError;}

protected:

    VOID initFileFlag();
    VOID nativeInitFileFlag();

    LONG curPos();
    LONG nativeCurPos();

    BOOL nativeIsSequential() const;
    BOOL nativeCaseSensitive() const;
    BOOL nativeOpen();
    BOOL nativeClose();
    DWORD nativeRead(VOID* pBuf, const DWORD& dwMaxSize);
    DWORD nativeWrite(const VOID* pBuf, const DWORD& iSize);
    LONG nativeSeek(const INT& iOffset, const Ant_FileSeekMode& eMode);
    BOOL nativeFlush();
    BOOL nativeAtEnd();
    LONGLONG nativeSize();
    BOOL nativeSetSize(const LONG& lSize);
    BOOL nativeExists();
    BOOL nativeRemove(BOOL bForse);
    BOOL nativeCopyTo(const Ant_String& strDstName);
    BOOL nativeMoveTo(const Ant_String& strNewName);
    BOOL nativeRename(const Ant_String& strNewName);
    BOOL nativeMakeLink(const Ant_String& strLinkName);
    Ant_String nativeLinkTarget();
    BOOL nativeMakeDir(const Ant_String& strDirName);
    DWORD nativeFileFlags(const DWORD& dwMask);
    BOOL nativeSetPermissions(DWORD dwPerms);
    BOOL nativeSetEndOfFile();
    Ant_String nativeOwnerName(const Ant_FileUser& eKind) const;
    DWORD nativeOwnerID(const Ant_FileUser& eKind) const;
    Ant_Time nativeFileTime(const Ant_FileTime& eKind);

    VOID setLastError();
    VOID setLastError(const Ant_FileError& eError) {m_eLastError = eError;}
    VOID nativeSetLastError();
    VOID cleanLastError() {m_eLastError = ANT_FER_NoError;}

private:

    Ant_String      m_strPath;      // Absolute path
    INT             m_iOpenMode;    // Open mode
#if defined(_WIN32) || defined(_WIN32_WCE)
    HANDLE          m_hFile;        // File handle
    HANDLE          m_hFileMap;     // File mapping handle
    DWORD           m_dwFileAttrs;
#else // For Linux
    FILE*           m_fp;           // File pointer
    INT             m_fd;           // File description
#endif
    DWORD           m_dwFileFlag;   // File flag
    Ant_FileError   m_eLastError;
    BOOL         m_bFileFlagInited;
    BOOL         m_bRecLog;

private:

    // Disable the copy constructor and operator =
    Ant_NormalFileEngine(const Ant_NormalFileEngine&);
    Ant_NormalFileEngine& operator=(const Ant_NormalFileEngine&);

};

class Ant_NormalFileEngineIterator : public Ant_AbstractFileEngineIterator
{
    friend class Ant_NormalFileEngine;

public:

    virtual ~Ant_NormalFileEngineIterator();

    virtual Ant_String currentName() const {return m_strCurEntry;}
    virtual BOOL end() const {return m_bEnd;}
    virtual Ant_AbstractFileEngineIterator& operator ++ ();
    virtual Ant_FileError getLastError() const {return m_eLastError;}
    virtual DWORD currentFileFlag() const {return m_dwCurFileFlag;}

protected:

    Ant_NormalFileEngineIterator();
    explicit Ant_NormalFileEngineIterator(const Ant_String& strDirPath);

    VOID setWorkDir(const Ant_String& strDirPath);
    BOOL init();
    VOID setLastError();
    VOID cleanLastError() {m_eLastError = ANT_FER_NoError;}

private:

    BOOL initFileFlags(const Ant_String& path);

    Ant_String      m_strWorkDir;
    Ant_String      m_strCurEntry;
#if defined(_WIN32) || defined(_WIN32_WCE)
    HANDLE          m_hFindHandle;
#else    // For Linux
    DIR*            m_pDir;
#endif
    Ant_FileError   m_eLastError;
    BOOL         m_bEnd;
    DWORD           m_dwCurFileFlag;

private:

    // Disable the copy constructor and operator =
    Ant_NormalFileEngineIterator(const Ant_NormalFileEngineIterator&);
    Ant_NormalFileEngineIterator& operator=(const Ant_NormalFileEngineIterator&);

};

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif  //ANT_NORMALFILEENGINE_H

/*---------------------------------------------------------------------------*/
/* EOF */
