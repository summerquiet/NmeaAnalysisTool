/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_FILEIMPL_H
#define ANT_FILEIMPL_H
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

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// class declare

// external class declaration
class Ant_AbstractFileEngine;

/**
 * @class Ant_FileImpl
 * @brief The class to manipulate normal files
 */
class Ant_FileImpl : public virtual Ant_Object
{
public:

    explicit Ant_FileImpl(const BOOL& bRecLog);
    Ant_FileImpl(const Ant_String& strFileName, const BOOL& bRecLog);
    virtual ~Ant_FileImpl();

    VOID setFileName(const Ant_String& strFileName);

    virtual BOOL open(const INT& iOpenMode);
    virtual BOOL close();
    virtual BOOL isOpen() const;
    virtual DWORD read(VOID* pBuff, const DWORD& dwMaxSize);
    virtual DWORD write(const VOID* pBuff, const DWORD& dwSize);
    virtual LONG seek(const INT& iOffset, const Ant_FileSeekMode& eMode);
    virtual BOOL atEnd();
    virtual BOOL flush();

    BOOL exists();
    BOOL remove();
    BOOL moveTo(const Ant_String& strNewName);
    BOOL copyTo(const Ant_String& strDstFileName);
    BOOL makeLink(const Ant_String& strLinkName);
    Ant_String linkTarget();
    BOOL setSize(const LONG& lSize);
    BOOL setPermissions(const DWORD& dwPerms);
    BOOL setEndOfFile();
    Ant_String fileName() const {return m_strFilePath;}
    Ant_FileError getLastError() const {return m_eLastError;}

protected:

    Ant_AbstractFileEngine*  m_pFileEngine;

    VOID setLastError(const Ant_FileError& eError) {m_eLastError = eError;}
    VOID cleanLastError() {setLastError(ANT_FER_NoError);}

private:

    Ant_String      m_strFilePath;
    Ant_FileError   m_eLastError;
    BOOL         m_bRecLog;

private:

    // Disable the copy constructor and operator =
    Ant_FileImpl(const Ant_FileImpl&);
    Ant_FileImpl operator= (const Ant_FileImpl&);

};

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif // ANT_FILEIMPL_H

/*---------------------------------------------------------------------------*/
/* EOF *//*---------------------------------------------------------------------------*/
