/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_FILEINFO_H
#define ANT_FILEINFO_H
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
#ifndef ANT_TIME_H
#   include "Ant_Time.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Class declare

// external class declaration
class Ant_FileInfoImpl;

/**
 * @class Ant_FileInfo
 * @brief The class to get detail information of a file
 *
 */
class ANTLIB_API Ant_FileInfo : public virtual Ant_Object
{
public:

    /**
     * @brief Constructor
     *
     * @param[in]    bRecLog    Whether record log or not. For TRUE, record log.
     * @note If you use this constructor, you should call \ref SetFileName before you get info of a file.
     */
    explicit Ant_FileInfo(const BOOL& bRecLog = TRUE);

    /**
     * @brief \overload
     *
     * @param[in]    strFileName    The absolute path of the file.
     * @param[in]    bRecLog        Whether record log or not. For TRUE, record log.
     * @remarks Before you call \ref SetFileName to change the file to manipulate,
            all operations will act on the file specified by the parameter <i>strFileName</i>.
     */
    explicit Ant_FileInfo(const Ant_String& strFileName, const BOOL& bRecLog = TRUE);

    /**
     * @brief Copy constructor
     *
     * @param[in]    cInfo    Another instance of \ref Ant_FileInfo.
     * @remarks Before you call \ref SetFileName to change the file to manipulate,
            all operations will act on the file specified by the file name of <i>cInfo</i>.
     */
    Ant_FileInfo(const Ant_FileInfo& cInfo);

    /**
     * @brief Copy function
     *
     * @param[in]    cInfo    Another instance of \ref Ant_FileInfo.
     * @return The reference to this instance.
     * @remarks After this function, the operation of this instance will be changed to the file name of <i>cInfo</i>.
     */
    Ant_FileInfo& operator = (const Ant_FileInfo& cInfo);

    /**
     * @brief Destructor
     */
    virtual ~Ant_FileInfo();

    /**
     * @brief Set the name of the file to manipulate
     *
     * @param[in]    strFileName    The absolute path of the file.
     * @remarks After this function call, all operations will be changed to the file
            specified by the parameter <i>strFileName</i>.
     */
    VOID setFileName(const Ant_String& strFileName);

    /**
     * @brief Get the file name
     *
     * @return The file path that this instance manipulate.
     */
    Ant_String fileName() const;

    /**
     * @brief check file exist
     *
     * @retval TRUE file exist
     * @retval FALSE file not exist
     */
    BOOL exists() const;

    /**
     * @brief Check whether the file is readable
     *
     * @return TRUE indicates that the file is readable, otherwise the file is not readable.
     */
    BOOL isReadable() const;

    /**
     * @brief Check whether the file is writable
     *
     * @return TRUE indicates that the file is writable, otherwise the file is not writable.
     */
    BOOL isWritable() const;

    /**
     * @brief Check whether the file is executable
     *
     * @return TRUE indicates that the file is executable, otherwise the file is not executable.
     */
    BOOL isExecutable() const;

    /**
     * @brief Check whether the file is hidden
     *
     * @return TRUE indicates that the file is hidden, otherwise the file is not hidden.
     */
    BOOL isHidden() const;

    /**
     * @brief get the file type
     *
     * @retval ANT_FFG_TypeFile      Normal file
     * @retval ANT_FFG_TypeDirectory Directory
     * @retval ANT_FFG_TypeLink      Link file
     * @retval ANT_FFG_TypeDevice    Device
     * @retval ANT_FFG_TypePipe      Pipe file
     * @retval ANT_FFG_TypeSocket    Socket
     */
    DWORD getFileType() const;

    /**
     * @brief Check whether the file is a normal file
     *
     * @return TRUE indicates that the file is normal file, otherwise the file is not normal file.
     */
    BOOL isFile() const;

    /**
     * @brief Check whether the file is a directory
     *
     * @return TRUE indicates that the file is a directory, otherwise the file is not a directory.
     */
    BOOL isDir() const;

    /**
     * @brief Check whether the file is a link file
     *
     * @return TRUE indicates that the file is a link file, otherwise the file is not a link file.
     */
    BOOL isLink() const;

    //Ant_String LinkTarget() const;

    /**
     * @brief Get the creation time of the file
     *
     * @return The creation time of the file.
     */
    Ant_Time creationTime() const;

    /**
     * @brief Get the last-write time of the file
     *
     * @return The last-write time of the file.
     */
    Ant_Time lastWriteTime() const;

    /**
     * @brief Get the last-access time of the file
     *
     * @return The last-access time of the file.
     */
    Ant_Time lastAccessTime() const;

    /**
     * @brief Get the user name of the file
     *
     * @return The user name of the file.
     */
    Ant_String user() const;

    /**
     * @brief Get the user's ID of the file
     *
     * @return The user's ID of the file.
     */
    DWORD userID() const;

    /**
     * @brief Get the group name of the file
     *
     * @return The group name of the file.
     */
    Ant_String group() const;

    /**
     * @brief Get the group's ID of the file
     *
     * @return The group's ID of the file. If the function call fails, the return value is zero,
            and you can call \ref GetLastError to get detail information.
     */
    DWORD groupID() const;

    /**
     * @brief Get the size of the file
     *
     * @return The size of the file. If the function call fails, the return value is -1,
            and you can call \ref GetLastError to get detail information.
     */
    LONG size() const;

    /**
     * @brief Get the size of the file(large file)
     *
     * @return The size of the file. If the function call fails, the return value is -1,
            and you can call \ref GetLastError to get detail information.
     */
    LONGLONG size64() const;

    /**
     * @brief Get the number of entries in the directory
     *
     * @param[in]    dwFilters    Entry filters. See \ref Ant_FileFilter.
     * @return The number of entries in the directory. If there's no entry or error occur, zero is return.
     */
    DWORD entryNum(const DWORD& dwFilters) const;

    /**
     * @brief Get the last error code
     *
     * @return The last error code. See \ref Ant_FileError.
     */
    Ant_FileError getLastError() const;

protected:

    Ant_FileInfoImpl*   m_ptr;
    BOOL                m_bRecLog;

};

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif  //ANT_FILEINFO_H

/*---------------------------------------------------------------------------*/
/* EOF */

