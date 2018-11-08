/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_ABSTRACTFILEENGINE_H
#define ANT_ABSTRACTFILEENGINE_H
#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_OBJECT_H
#   include "Ant_Object.h"
#endif
#ifndef ANT_FILEPUBDEF_H
#   include "Ant_FilePubDef.h"
#endif
#ifndef ANT_STRING_H
#   include "Ant_String.h"
#endif
#ifndef ANT_TIME_H
#   include "Ant_Time.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// class declaration

class Ant_AbstractFileEngineIterator;

class Ant_AbstractFileEngine : public virtual Ant_Object
{
public:

    /**
     * @brief Create a file engine for file
     *
     * The \a strFileName should be an absolute path
     */
    static Ant_AbstractFileEngine* create(const Ant_String& strFileName, const BOOL& bRecLog);

    /**
     * @brief Destructor
     */
    virtual ~Ant_AbstractFileEngine() {}

    /**
     * @brief Whether the file is a sequential access device
     */
    virtual BOOL isSequential() const;

    /**
     * @brief Whether the underlying file system is case-sensitive
     */
    virtual BOOL caseSensitive() const;

    /**
     * @brief Open the file in the specified mode
     */
    virtual BOOL open(INT iOpenMode);
    /**
     * @brief Close the file
     */
    virtual BOOL close();
    /**
     * @brief Whether the file is opened
     */
    virtual BOOL isOpen();
    /**
     * @brief Read a number of bytes from the file into \a pBuf
     */
    virtual DWORD read(VOID* pBuf, const DWORD& dwMaxSize);
    /**
     * @brief Write \a dwSize bytes from \a pBuf to the file
     */
    virtual DWORD write(const VOID* pBuf, const DWORD& dwSize);
    /**
     * @brief Set the file pointer to the given \a iOffset
     */
    virtual LONG seek(const INT& iOffset, const Ant_FileSeekMode& eMode);
    /**
     * @brief Flush the open file
     */
    virtual BOOL flush();
    /**
     * @brief Whether reach the end of the file
     */
    virtual BOOL atEnd();
    /**
     * @brief Return the size of the file
     */
    virtual LONGLONG size();
    /**
     * @brief Change the file's size to \a iSize
     */
    virtual BOOL setSize(const LONG& lSize);

    /**
     * @brief Whether the file or directory exists
     */
    virtual BOOL exists();

    /**
     * @brief Remove the file or directory from the file system
     *
     * The directory should be empty.
     */
    virtual BOOL remove(BOOL bForse = TRUE);

    /**
     * @brief Copy the file or directory to a new position
     */
    virtual BOOL copyTo(const Ant_String& strDstName);

    /**
     * @brief Move the file or directory to new position
     */
    virtual BOOL moveTo(const Ant_String& strNewName);

    /**
     * @brief change the file or directory to name
     */
    virtual BOOL rename(const Ant_String& strNewName);

    /**
     * @brief Create a link for the file or directory
     */
    virtual BOOL makeLink(const Ant_String& strLinkName);

    /**
     * @brief Link to a target
     */
    virtual Ant_String linkTarget();

    /**
     * @brief Create a subdirectory under the directory
     *
     * The upper directory should exist.
     */
    virtual BOOL makeDir(const Ant_String& strDirName);

    typedef Ant_AbstractFileEngineIterator Iterator;

    /**
     * @brief Get the iterator of all entries in the directory
     *
     * You should delete it.
     */
    virtual Iterator* entryIterator();

    /**
     * Whether the file or directory this file engine refers to has a relative path
     */
    //virtual BOOL IsRelativePath() const;
    /**
     * @brief Return the flags of the file or directory
     */
    virtual DWORD fileFlags(const DWORD& dwMask = ANT_FFG_AllInfo);

    /**
     * @brief Set the permissions of the file or directory
     */
    virtual BOOL setPermissions(const DWORD& dwPerms);

    /**
     * @brief Set End of a File, truncate file into current position
     */
    virtual BOOL setEndOfFile();

    /**
     * @brief Return the name of the user of the file or directory
     */
    virtual Ant_String ownerName(const Ant_FileUser& eKind) const;

    /**
     * @brief Return the ID of the user of the file or directory
     */
    virtual DWORD ownerID(const Ant_FileUser& eKind) const;

    /**
     * @brief Return the time of the file or directory
     */
    virtual Ant_Time fileTime(const Ant_FileTime& eKind);

    /**
     * @brief Get last error code
     */
    virtual Ant_FileError getLastError() const;

protected:

    /**
     * @brief Constructor
     */
    Ant_AbstractFileEngine() {}

private:

    // Disable the copy constructor and operator =
    Ant_AbstractFileEngine(const Ant_AbstractFileEngine&);
    Ant_AbstractFileEngine& operator = (const Ant_AbstractFileEngine&);

};


class Ant_AbstractFileEngineIterator : public virtual Ant_Object
{
public:

    /**
     * @brief Destructor
     */
    virtual ~Ant_AbstractFileEngineIterator() {}

    /**
     * @brief Get the name of current file
     */
    virtual Ant_String currentName() const = 0;

    /**
     * @brief Whether there's no more file
     */
    virtual BOOL end() const = 0;

    /**
     * @brief Move to next file
     */
    virtual Ant_AbstractFileEngineIterator& operator ++ () = 0;

    /**
     * @brief Get last error code
     */
    virtual Ant_FileError getLastError() const = 0;

    /**
     * @brief Get current file flag
     */
    virtual DWORD currentFileFlag() const = 0;

protected:

    /**
     * @brief Constructor
     */
    Ant_AbstractFileEngineIterator() {}

private:

    // Disable the copy constructor and operator =
    Ant_AbstractFileEngineIterator(const Ant_AbstractFileEngineIterator&);
    Ant_AbstractFileEngineIterator& operator = (const Ant_AbstractFileEngineIterator&);

};

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif  // ANT_ABSTRACTFILEENGINE_H

/*---------------------------------------------------------------------------*/
/* EOF */

