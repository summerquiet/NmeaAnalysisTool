/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_LOGCONTROL_H
#define ANT_LOGCONTROL_H
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
#ifndef ANT_LOGPUBLICDEF_H
#   include "Ant_LogPublicDef.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Class declare

class Ant_VolatileMemory;

/**
 * class Ant_LogControl
 *
 */

class Ant_LogControl : public virtual Ant_Object
{
public:

    /**
     * Ant_LogControl
     *
     * Constructor
     *
     * @param null
     */
    Ant_LogControl(DWORD dwMaxLogNum);

    /**
     * ~Ant_LogControl
     *
     * Destructor
     *
     * @param null
     */
    ~Ant_LogControl();

    /**
    * Output
    *
    * Output log information to memory
    *
    * @param const CHAR*  szLogName [IN]: Log name
    *
    * @param const CHAR*  szLogInfo [IN]: Log information
    *
    * @return VOID
    */
    VOID output(const CHAR* szLogName, const CHAR* szLogInfo);

    /**
    * Output
    *
    * Output log information to memory
    *
    * @param const CHAR*  szLogName [IN]: Log name
    *
    * @param const WCHAR*  szLogInfo [IN]: Log information
    *
    * @return VOID
    */
    VOID output(const CHAR* szLogName, const WCHAR* szLogInfo);

    /**
    * GetAllLogInfo
    *
    * Get all log information from memory
    *
    * @param Ant_LogRecord*  pLogRecord [OUT]:  Log record array used to  save gotten log info
    *
    * @param DWORD&  dwGotRecordNum [OUT]: Log record number
    *
    * @return BOOL
    * @retval TRUE: Successful
    * @retval FALSE: Failure
    */
    BOOL getAllLogInfo(Ant_LogRecord* pLogRecord, DWORD& dwGotRecordNum);

    /**
    * GetLogInfo
    *
    * Get special log name log information
    *
    * @param const CHAR*  szLogName [IN]:
    *
    * @param Ant_LogRecord*  pLogRrecord [OUT]:  Log record array used to  save gotten log info
    *
    * @param DWORD&  dwGotLogNum [OUT]: Log record number
    *
    * @return BOOL
    * @retval TRUE: Successful
    * @retval FALSE: Failure
    */
    BOOL getLogInfo(const CHAR* szLogName, Ant_LogRecord* pLogRrecord, DWORD& dwGotLogNum);

    /**
    * ClearAllLogInfo
    *
    * Clear log information form memory
    *
    * @param null
    *
    * @return VOID
    */
    VOID clearAllLogInfo();

    // Check the special log name whether is allowed to write log information
    BOOL checkLogName(const CHAR* szLogNum);

    // Write log info
    VOID writeLog(const CHAR* szLogName, const CHAR* szLogInfo);
    VOID writeLog(const CHAR* szLogName, const WCHAR* szLogInfo);

private:

    DWORD               m_dwMaxLogNum;  // max log record number
    Ant_VolatileMemory* m_pLogInfoMem;  // share memory pointer

private:

    // Disable the copy constructor and operator =
    Ant_LogControl(const Ant_LogControl&);
    Ant_LogControl& operator = (const Ant_LogControl&);

};

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif  // ANT_LOGCONTROL_H

/*---------------------------------------------------------------------------*/
/* EOF */
