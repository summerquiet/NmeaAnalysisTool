/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_DIRIMPL_H
#define ANT_DIRIMPL_H
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
#ifndef ANT_FILEINFO_H
#   include "Ant_FileInfo.h"
#endif
#ifndef ANT_LIST_H
#   include "Ant_List.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// class declare
// external class declaration
class Ant_AbstractFileEngine;
class Ant_AbstractFileEngineIterator;
class Ant_DirIterator;
class Ant_DirIteratorImpl;

/**
 * @class Ant_DirImpl
 * @brief The class to manipulate directories
 *
 */
class Ant_DirImpl : public virtual Ant_Object
{
public:

    explicit Ant_DirImpl(const BOOL& bRecLog);
    Ant_DirImpl(const Ant_String& strDirName, const BOOL& bRecLog);
    ~Ant_DirImpl();

    VOID setWorkDir(const Ant_String& strDirName);
    BOOL exists(const Ant_String& strFileName);
    BOOL move(const Ant_String& strOldName, const Ant_String& strNewName);
    BOOL copy(const Ant_String& strSrcName, const Ant_String& strDstName);
    BOOL remove(const Ant_String& strFileName, const BOOL& bForce);
    BOOL makeDir(const Ant_String& strDirName);
    BOOL makePath(const Ant_String& strPathName);
    DWORD entryNum(const DWORD& dwFilters);
    Ant_DirIterator* entryIterator(const DWORD& dwFilters);
    Ant_FileError getLastError() const {return m_eLastError;}

protected:

    VOID setLastError(const Ant_FileError& eError) {m_eLastError = eError;}
    VOID cleanLastError() {setLastError(ANT_FER_NoError);}

private:

    Ant_String      m_strDirPath;
    Ant_FileError   m_eLastError;
    BOOL         m_bRecLog;

};

/**
 * @class Ant_DirIteratorImpl
 * @brief The class to manipulate directories
 *
 */
class Ant_DirIteratorImpl : public virtual Ant_Object
{
public:

    Ant_DirIteratorImpl(const Ant_String& strDirPath, const DWORD& dwFilters, const BOOL& bRecLog);
    ~Ant_DirIteratorImpl();

    Ant_String currentFileName();
    Ant_FileInfo currentFileInfo();
    BOOL end() const;
    Ant_DirIteratorImpl& operator ++ ();
    VOID reset();

protected:

    BOOL initEntryList(const DWORD& dwFilters);
    BOOL match(const DWORD& dwFileFlag, const DWORD& dwFilters);

private:

    Ant_String              m_strDirPath;
    Ant_List<Ant_String>    m_cEntryList;
    INT                     m_iCur;
    BOOL                    m_bRecLog;

};

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif  //ANT_DIRIMPL_H

/*---------------------------------------------------------------------------*/
/* EOF */
