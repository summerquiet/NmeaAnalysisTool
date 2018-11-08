/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_FILELOG_H
#define ANT_FILELOG_H
#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_OBJECT_H
#   include "Ant_Object.h"
#endif
#ifndef ANT_STRING_H
#   include "Ant_String.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Value define

const INT ANT_FILE_LOG_MAX_NAME = 128;

/*---------------------------------------------------------------------------*/
// class declare

/**
 * @class Ant_FileLog
 * @brief The class to manipulate normal files
 */
class Ant_FileLog : public virtual Ant_Object
{
public:

    Ant_FileLog(const Ant_String& strFuncName, const Ant_String& strFilePath, const BOOL& bRecLog);
    virtual ~Ant_FileLog();

private:

    XCHAR       m_szFuncName[ANT_FILE_LOG_MAX_NAME + 1];
    XCHAR       m_szFilePath[ANT_FILE_LOG_MAX_NAME + 1];
    DWORD       m_dwStartCount;
    //DWORD     m_dwStartThreadTime;
    DWORD       m_dwThreadId;
    //DWORD     m_dwProcessId;
    BOOL     m_bRecLog;

private:

    // Disable the copy constructor and operator =
    Ant_FileLog(const Ant_FileLog&);
    Ant_FileLog& operator = (const Ant_FileLog&);

};

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif  // ANT_FILELOG_H

/*---------------------------------------------------------------------------*/
/* EOF */

