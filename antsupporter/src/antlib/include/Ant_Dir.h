/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_DIR_H
#define ANT_DIR_H
#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_NEWTYPESDEFINE_H
#   include "Ant_NewTypesDefine.h"
#endif
#ifndef ANT_OBJECT_H
#   include "Ant_Object.h"
#endif
#ifndef ANT_FILEPUBDEF_H
#   include "Ant_FilePubDef.h"
#endif
#ifndef ANT_STRING_H
#   include "Ant_String.h"
#endif
#ifndef ANT_FILEINFO_H
#   include "Ant_FileInfo.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Class declare

// external class declaration
class Ant_DirImpl;
class Ant_DirIteratorImpl;
class Ant_DirIterator;

/**
 * @class Ant_Dir
 * @brief The class to manipulate directories
 *
 */
class ANTLIB_API Ant_Dir : public virtual Ant_Object
{
public:

    //Ant_Dir();
    /**
     * @brief Constructor
     *
     * @param[in]    bRecLog    Whether record log or not. For TRUE, record log.
     */
    explicit Ant_Dir(const BOOL& bRecLog = TRUE);

    /**
     * @brief \overload
     *
     * @param[in]    strDirName    The absolute path of the work directory.
     * @param[in]    bRecLog        Whether record log or not. For TRUE, record log.
     */
    explicit Ant_Dir(const Ant_String& strDirName, const BOOL& bRecLog = TRUE);

    /**
     * @brief \overload
     *
     * @param[in]    szDirName    The absolute path of the work directory.
     * @param[in]    bRecLog        Whether record log or not. For TRUE, record log.
     */
    explicit Ant_Dir(const XCHAR* szDirName, const BOOL& bRecLog = TRUE);

    /**
     * @brief Destructor
     */
    ~Ant_Dir();

    /**
     * @brief Set the work directory
     *
     * @param[in]    strDirName    The absolute path of the work directory.
     */
    VOID setWorkDir(const Ant_String& strDirName);

    /**
     * @brief Check whether a file or directory exists
     *
     * @param[in]    strFileName    The name of the file or directory to check.
     * @return TRUE indicates that the file or directory exists. Otherwise indicates not exist.
     * @remarks If the class instance has work directory, you can use relative or absolute path.
            Otherwise, you should use absolute path.
     */
    BOOL exists(const Ant_String& strFileName);

    /**
     * @brief Move a file or directory to a new place
     *
     * For directory, all items in the old directory will be moved to the new directory.
     *
     * @param[in]    strOldName    The name of the file or directory to be moved.
     * @param[in]    strNewName    The name of the file or directory to move to.
     * @return Whether the function call succeed. TRUE indicates success.
            Otherwise indicates failure, and you can call \ref GetLastError to get detail information.
     * @remarks If the class instance has work directory, you can use relative or absolute path.
            Otherwise, you should use absolute path.\n
            On Linux, even if the file or directory is used by others, it can be moved too.
            So you should ensure that the file or directory is not used when you move it. If not, errors will occur.
     */
    BOOL move(const Ant_String& strOldName, const Ant_String& strNewName);

    /**
     * @brief Copy a file or directory
     *
     * For directory, all items in the source directory will be copied to the destination directory.
     *
     * @param[in]    strSrcName    The name of the file or directory to be copied.
     * @param[in]    strDstName    The name of the file or directory to copy to.
     * @return Whether the function call succeed. TRUE indicates success.
            Otherwise indicates failure, and you can call \ref GetLastError to get detail information.
     * @remarks If the class instance has work directory, you can use relative or absolute path.
            Otherwise, you should use absolute path.\n
     */
    BOOL copy(const Ant_String& strSrcName, const Ant_String& strDstName);

    /**
     * @brief Remove a file or directory
     *
     * For directory, all files and subdirectories in the directory will be removed.
     *
     * @param[in]    strFileName    The name of the file or directory to be removed.
     * @param[in]    bForce        Whether remove the file or directory forcedly
     * @return Whether the function call succeed. TRUE indicates success.
            Otherwise indicates failure, and you can call \ref GetLastError to get detail information.
     * @remarks If the class instance has work directory, you can use relative or absolute path.
            Otherwise, you should use absolute path.\n
            On Linux, even if the file or directory is used by others, it can be removed too.
            So you should ensure that the file or directory is not used when you remove it. If not, errors will occur.
     */
    BOOL remove(const Ant_String& strFileName, const BOOL& bForce = TRUE);

    /**
     * @brief Create a directory
     *
     * @param[in]    strDirName    The name of the directory to be created.
     * @return Whether the function call succeed. TRUE indicates success.
            Otherwise indicates failure, and you can call \ref GetLastError to get detail information.
     * @remarks If the class instance has work directory, you can use relative or absolute path.
            Otherwise, you should use absolute path.\n
            This function can make one level directory only,
            so if the upper level directory does not exist, this function call will fail.
     */
    BOOL makeDir(const Ant_String& strDirName);

    /**
     * @brief Create a path
     *
     * @param[in]    strPathName    The name of the path to be created.
     * @return Whether the function call succeed. TRUE indicates success.
            Otherwise indicates failure, and you can call \ref GetLastError to get detail information.
     * @remarks If the class instance has work directory, you can use relative or absolute path.
            Otherwise, you should use absolute path.\n
            This function can make the full path named in the parameter <i>strPathName</i>.
     */
    BOOL makePath(const Ant_String& strPathName);

    /**
     * @brief Get the number of entries
     *
     * @param[in]    dwFilters    Entry filters. See \ref Ant_FileFilter.
     * @return The number of entries in the directory. If there's no entry or error occur, zero is return.
     */
    DWORD entryNum(const DWORD& dwFilters);

    /**
     * @brief Get the entry iterator
     *
     * @param[in]    dwFilters    Entry filters. See \ref Ant_FileFilter.
     * @return The pointer to the entry iterator. See more about \ref Ant_DirIterator.
     * @remarks To use this function, the class instance should have work directory.
            Without the work directory, you can use \ref SetWorkDir to set the work directory first.\n
            After using the entry iterator, you should delete it.
            There are some sample code:
            \code
                Ant_Dir cDir(XTEXT("your/dir/path"));
                Ant_DirIterator* pcIter = cDir.EntryIterator(Ant_FFT_AllEntries);
                // do something using pcIter
                delete pcIter;    // you must do it to avoid memory leak
                // do something else
            \endcode
     */
    Ant_DirIterator* entryIterator(const DWORD& dwFilters);

    /**
     * @brief Get the path of current directory
     *
     * @return The path of current directory.
     * @remarks This function get the path of the directory where the application starts
            but not the work directory of the class instance.
     */
    static Ant_String currentDir();

    /**
     * @brief Get the last error code
     *
     * @return The last error code. See \ref Ant_FileError.
     */
    Ant_FileError getLastError() const;

protected:

    Ant_DirImpl* m_ptr;

};

/**
 * @class Ant_DirIterator
 * @brief Entry iterator of a directory
 *
 * @par Usage sample:
        \code
            Ant_Dir cDir(XTEXT("your/dir/path"));
            Ant_DirIterator* pcIter = cDir.EntryIterator(Ant_FFT_AllEntries);
            while( !pcIter->End() )
            {
                Ant_String strFileName = pcIter->CurrentFileName();
                Ant_FileInfo cFileInfo = pcIter->CurrentFileInfo();

                // do something else

                ++(*pcIter);
            }
            delete pcIter;    // you must do it to avoid memory leak
        \endcode
 *
 */
class ANTLIB_API Ant_DirIterator
{
public:

    friend class Ant_DirImpl;

    /**
     * @brief Destructor
     */
    ~Ant_DirIterator();

    /**
     * @brief Get the name of current entry
     *
     * @return The name of current entry.
     */
    Ant_String currentFileName() const;

    /**
     * @brief Get the file info of current entry
     *
     * @return The file info(see \ref Ant_FileInfo) of current entry.
     */
    Ant_FileInfo currentFileInfo() const;

    //*
    // * @brief Get the info of current entry
    // */
    //Ant_FileInfo CurrentFileInfo();

    /**
     * @brief Check whether there are more entries
     *
     * @return TRUE indicates that there are more entries. Otherwise indicates no more entry.
     */
    BOOL end() const;

    /**
     * @brief Move to next entry
     *
     * This function overwrites the left-increment operator
     */
    Ant_DirIterator& operator ++ ();

    /**
     * @brief Reset the iterator
     *
     * After this function call, the iterator points to the first entry
     */
    VOID reset();

protected:

    /**
     * @brief Constructor
     *
     * @param[in]    strDirPath     The directory whose entries to iterate.
     * @param[in]    dwFilters      Entry filters. See \ref Ant_FileFilter.
     * @param[in]    bRecLog        Whether record log or not. For TRUE, record log.
     */
    Ant_DirIterator(const Ant_String& strDirPath, const DWORD& dwFilters, const BOOL& bRecLog);

    Ant_DirIteratorImpl* m_ptr;

};

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif  // ANT_DIR_H

/*---------------------------------------------------------------------------*/
/* EOF */
