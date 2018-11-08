/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_FILE_H
#define ANT_FILE_H
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
// Class clare

// external class declaration
class Ant_FileImpl;

/**
 * @class Ant_File
 * @brief The class to manipulate normal files
 */
class ANTLIB_API Ant_File : public virtual Ant_Object
{
public:

    /**
     * @brief Constructor
     *
     * @param[in]    bRecLog    Whether record log or not. For TRUE, record log.
     * @note If you use this constructor, you should call \ref SetFileName before you manipulate a file.
     */
    explicit Ant_File(const BOOL& bRecLog = TRUE);

    /**
     * @brief \overload
     *
     * @param[in]    strFileName    The absolute path of the file.
     * @param[in]    bRecLog        Whether record log or not. For TRUE, record log.
     * @remarks Before you call \ref SetFileName to change the file to manipulate,
            all operations will act on the file specified by the parameter <i>strFileName</i>.
     */
    explicit Ant_File(const Ant_String& strFileName, const BOOL& bRecLog = TRUE);

    /**
     * @brief \overload
     *
     * @param[in]    szFileName    The absolute path of the file.
     * @param[in]    bRecLog        Whether record log or not. For TRUE, record log.
     * @remarks Before you call \ref SetFileName to change the file to manipulate,
            all operations will act on the file specified by the parameter <i>szFileName</i>.
     */
    explicit Ant_File(const XCHAR* szFileName, const BOOL& bRecLog = TRUE);

    /**
     * @brief Destructor
     */
    virtual ~Ant_File();

    /**
     * @brief Set the name of the file to manipulate
     *
     * @param[in]    strFileName    The absolute path of the file.
     * @remarks After this function call, all operations will be changed to the file
            specified by the parameter <i>strFileName</i>.
     */
    VOID setFileName(const Ant_String& strFileName);

    /**
     * @brief Open the file
     *
     * @param[in]    iOpenMode    The open mode. See \ref Ant_FileOpenMode.
     * @return Whether the function call succeed. TRUE indicates success.
            Otherwise indicates failure, and you can call \ref GetLastError to get detail information.
     * @remarks After open a file, you should close it using \ref Close.
     */
    virtual BOOL open(const INT& iOpenMode);

    /**
     * @brief Close the file
     *
     * @return Whether the function call succeed. TRUE indicates success.
            Otherwise indicates failure, and you can call \ref GetLastError to get detail information.
     * @remarks After open a file using \ref Open, you should close it.
     */
    virtual BOOL close();

    /**
     * @brief Check whether the file is open
     *
     * @return TRUE indicates that the file is open. Otherwise indicates the file is not open.
     */
    virtual BOOL isOpen() const;

    /**
     * @brief Read data from the file
     *
     * @param[in]    pBuff        The pointer to the buffer that accepts the data read from the file.
     * @param[in]    dwMaxSize    The maximum size to read from the file.
     * @return The return value indicates the size that actually read from the file.
            If reaching the end of file(EOF) or some error occurs, the return value is zero.
            So you should use \ref AtEnd to check whether reaches the end of file.
            If the return value is zero and \ref AtEnd returns TRUE, that means reaching the end of file.
            Otherwise means some error occurs, and you can call \ref GetLastError to get detail information.
     */
    virtual DWORD read(VOID* pBuff, const DWORD& dwMaxSize);

    /**
     * @brief Write data to the file
     *
     * @param[in]    pBuff    The pointer to the buffer that contains the data write to the file.
     * @param[in]    dwSize    The data size to write to the file.
     * @return The return value indicates the size that actually write to the file.
            If some error occurs, the return value is zero, and you can call \ref GetLastError to get detail information.
     */
    virtual DWORD write(const VOID* pBuff, const DWORD& dwSize);

    /**
     * @brief Move the file pointer.
     *
     * The function moves the position of file pointer for reading or writing.
     *
     * @param[in]    iOffset    The offset to move from the starting point.
                            If its value is negative, then move backward; if its value is positive, then move foreward.
     * @param[in]    eMode    Specify the starting point. See \ref Ant_FileSeekMode.
     * @return The return value indicates the new position of the file pointer(the offset from the beginning of the file).
            If some error occurs, the return value is -1, and you can call \ref GetLastError to get detail information.
     */
    virtual LONG seek(const INT& iOffset, const Ant_FileSeekMode& eMode);

    /**
     * @brief Check whether reach the end of file
     *
     * @return TRUE indicates reaching the end of file. Otherwise indicates not.
     */
    virtual BOOL atEnd();

    /**
     * @brief Flush data from IO cache to hardware
     *
     * @return Whether the function call succeed. TRUE indicates success.
            Otherwise indicates failure, and you can call \ref GetLastError to get detail information.
     */
    virtual BOOL flush();

    /**
     * @brief Check whether the file exists
     *
     * @return TRUE indicates that the file or directory exists. Otherwise indicates not exist.
     */
    BOOL exists() const;

    /**
     * @brief Check whether a file exists
     *
     * @param[in]    strFileName    The absolute path of the file to check.
     * @return TRUE indicates that the file or directory exists. Otherwise indicates not exist.
     */
    static BOOL exists(const Ant_String& strFileName);

    /**
     * @brief Remove the file
     *
     * @return Whether the function call succeed. TRUE indicates success.
            Otherwise indicates failure, and you can call \ref GetLastError to get detail information.
     * @remarks On Linux, even if the file or directory is used by others, it can be removed too.
            So you should ensure that the file or directory is not used when you remove it. If not, errors will occur.
     */
    BOOL remove();

    /**
     * @brief Remove a file
     *
     * @param[in]    strFileName    The absolute path of the file to remove.
     * @return Whether the function call succeed. TRUE indicates success, otherwise indicates failure.
     * @remarks On Linux, even if the file or directory is used by others, it can be removed too.
            So you should ensure that the file or directory is not used when you remove it. If not, errors will occur.
     */
    static BOOL remove(const Ant_String& strFileName);

    /**
     * @brief Move the file
     *
     * This function move the file to the new place or just rename it.
     *
     * @param[in]     strNewName    The absolute path of the new file.
     * @return Whether the function call succeed. TRUE indicates success.
            Otherwise indicates failure, and you can call \ref GetLastError to get detail information.
     * @remarks On Linux, even if the file or directory is used by others, it can be moved too.
            So you should ensure that the file or directory is not used when you move it. If not, errors will occur.\n
            If the new file has already existed, this function fails and the file is not moved to the new place.
     */
    BOOL moveTo(const Ant_String& strNewName);

    /**
     * @brief Move a file
     *
     * This function move a file from the old place to a new place or just rename it.
     *
     * @param[in]    strOldName    The absolute path of the old file.
     * @param[in]    strNewName    The absolute path of the new file.
     * @return Whether the function call succeed. TRUE indicates success, otherwise indicates failure.
     * @remarks On Linux, even if the file or directory is used by others, it can be moved too.
            So you should ensure that the file or directory is not used when you remove it. If not, errors will occur.\n
            If the new file has already existed, this function fails and the file is not moved to the new place.
     */
    static BOOL move(const Ant_String& strOldName, const Ant_String& strNewName);

    /**
     * @brief Copy the file
     *
     * This function copy the file to the destination place.
     *
     * @param[in]    strDstFileName    The absolute path of the destination file.
     * @return Whether the function call succeed. TRUE indicates success.
            Otherwise indicates failure, and you can call \ref GetLastError to get detail information.
     * @remarks If the destination file has already existed, this function fails and the file is not copied
            and the destination file is not be overwritten.
     */
    BOOL copyTo(const Ant_String& strDstFileName);

    /**
     * @brief Copy a file
     *
     * This function copy the source file to the destination place.
     *
     * @param[in]    strSrcFileName    The absolute path of the source file.
     * @param[in]    strDstFileName    The absolute path of the destination file.
     * @return Whether the function call succeed. TRUE indicates success, otherwise indicates failure.
     * @remarks If the destination file has already existed, this function fails and the file is not copied
            and the destination file is not be overwritten.
     */
    static BOOL copy(const Ant_String& strSrcFileName, const Ant_String& strDstFileName);

    /**
     * @brief Make a symbol link
     *
     * This function make a symbol link to the file
     *
     * @param[in]    strLinkName    The absolute path of the link file.
     * @return Whether the function call succeed. TRUE indicates success.
            Otherwise indicates failure, and you can call \ref GetLastError to get detail information.
     */
    BOOL makeLink(const Ant_String& strLinkName);

    /**
     * @brief Make a symbol link
     *
     * This function make a symbol link to a file
     *
     * @param[in]    strFileName    The absolute path of the file to be linked.
     * @param[in]    strLinkName    The absolute path of the symbol link file.
     * @return Whether the function call succeed. TRUE indicates success, otherwise indicates failure.
     */
    static BOOL makeLink(const Ant_String& strFileName, const Ant_String& strLinkName);

    /**
     * @brief Get the link target
     *
     * This function get the file name of the target that the file link to
     *
     * @return The full path of the target file. If the return string is null-string, that means some error occurs,
            and you can call \ref GetLastError to get detail information.
     */
    Ant_String linkTarget();

    /**
     * @brief Get the link target
     *
     * This function get the file name of the target that the file link to
     *
     * @param[in]    strLinkName    The absolute path of the link file.
     * @return The full path of the target file. If the return string is null-string, that means some error occurs.
     */
    static Ant_String linkTarget(const Ant_String& strLinkName);

    //LONG Size() const;
    //static LONG Size(const Ant_String& strFileName);

    /**
     * @brief Set the size of the file
     *
     * This function changes the size of a file. If the size is less than the original size of the file,
     * the file will be truncated. Otherwise, the file will be appended with zero.
     *
     * @param[in]    lSize    The size of the file.
     * @return Whether the function call succeed. TRUE indicates success.
            Otherwise indicates failure, and you can call \ref GetLastError to get detail information.
     */
    BOOL setSize(const LONG& lSize);

    /**
     * @brief Set the size of a file
     *
     * This function changes the size of a file. If the size is less than the original size of the file,
     * the file will be truncated. Otherwise, the file will be appended with zero.
     *
     * @param[in]    strFileName    The absolute path of the file.
     * @param[in]    lSize        The size of the file.
     * @return Whether the function call succeed. TRUE indicates success, otherwise indicates failure.
    */
    static BOOL setSize(const Ant_String& strFileName, const LONG& lSize);

    /**
     * @brief Set the permissions of the file
     *
     * @param[in]    dwPerms    The file permissions. See \ref Ant_FilePermission.
     * @return Whether the function call succeed. TRUE indicates success.
            Otherwise indicates failure, and you can call \ref GetLastError to get detail information.
     */
    BOOL setPermissions(const DWORD& dwPerms);

    /**
     * @brief Set the permissions of a file
     *
     * @param[in]    strFileName    The absolute path of the file.
     * @param[in]    dwPerms        The file permissions. See \ref Ant_FilePermission.
     * @return Whether the function call succeed. TRUE indicates success, otherwise indicates failure.
     */
    static BOOL setPermissions(const Ant_String& strFileName, const DWORD& dwPerms);

    /**
     * @brief Set end of a file, truncate file into current size
     *
     * @return Whether the function call succeed. TRUE indicates success, otherwise indicates failure.
     */
    BOOL setEndOfFile();

    /**
     * @brief Get the file info of the file
     *
     * @return The file info of the file. See \ref Ant_FileInfo.
     */
    Ant_FileInfo fileInfo() const;

    /**
     * @brief Get the last error code
     *
     * @return The last error code. See \ref Ant_FileError.
     */
    Ant_FileError getLastError() const;

protected:

    explicit Ant_File(Ant_FileImpl* ptr);
    Ant_FileImpl* m_ptr;

private:

};

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif // ANT_FILE_H

/*---------------------------------------------------------------------------*/
/* EOF */

