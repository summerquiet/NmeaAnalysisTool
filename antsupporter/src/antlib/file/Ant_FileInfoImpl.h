/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_FILEINFO_IMPLH
#define ANT_FILEINFO_IMPLH
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
// class declare

// external class declaration
class Ant_AbstractFileEngine;

/**
 * @class Ant_FileInfoImpl
 * @brief The class to manipulate normal files
 */
class Ant_FileInfoImpl : public virtual Ant_Object
{
public:

    explicit Ant_FileInfoImpl(const BOOL& bRecLog);
    Ant_FileInfoImpl(const Ant_String& strFileName, const BOOL& bRecLog);
    virtual ~Ant_FileInfoImpl();

    VOID setFileName(const Ant_String& strFileName);
    Ant_String fileName() const {return m_strFilePath;}
    BOOL exists();
    BOOL isReadable();
    BOOL isWritable();
    BOOL isExecutable();
    BOOL isHidden();
    DWORD getFileType();
    BOOL isFile();
    BOOL isDir();
    BOOL isLink();
    Ant_Time creationTime();
    Ant_Time lastWriteTime();
    Ant_Time lastAccessTime();
    Ant_String user();
    DWORD userID();
    Ant_String group();
    DWORD groupID();
    LONGLONG size();
    DWORD entryNum(const DWORD& dwFilters);

    Ant_FileError getLastError() const {return m_eLastError;}

protected:

    Ant_AbstractFileEngine* m_pFileEngine;

    VOID setLastError(const Ant_FileError& eError) { m_eLastError = eError;}
    VOID cleanLastError() {setLastError(ANT_FER_NoError);}

private:

    Ant_String      m_strFilePath;
    Ant_FileError   m_eLastError;
    BOOL         m_bRecLog;

private:

    // Disable the copy constructor and operator =
    Ant_FileInfoImpl(const Ant_FileInfoImpl&);
    Ant_FileInfoImpl& operator= (const Ant_FileInfoImpl&);

};

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif  //ANT_FILEINFOIMPL_H

/*---------------------------------------------------------------------------*/
/* EOF */
