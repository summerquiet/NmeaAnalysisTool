/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_LOGIMP_H
#define ANT_LOGIMP_H
#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_LOGPUBLICDEF_H
#   include "Ant_LogPublicDef.h"
#endif
#ifndef ANT_OBJECT_H
#   include "Ant_Object.h"
#endif
#ifndef ANT_SYNCOBJ_H
#   include "Ant_SyncObj.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Class declare

class Ant_LogControl;

/**
 * class Ant_LogImpl
 *
 */
class Ant_LogImpl : public virtual Ant_Object
{
public:

    /**
     * Ant_LogImpl
     *
     * Constructor
     *
     * @param null
     */
    Ant_LogImpl();

    /**
     * ~Ant_LogImpl
     *
     * Destructor
     *
     * @param null
     */
    ~Ant_LogImpl();

    /**
     * Start
     *
     * Start log
     *
     * @param DWORD dwMaxLogNum [IN]: Log record max num
     *
     * @return VOID
     */
    VOID start(DWORD dwMaxLogNum);

    /**
     * Stop
     *
     * Stop log
     *
     * @param null
     *
     * @return VOID
     */
    VOID stop();

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

private:

    Ant_SyncObj     m_cSyncObj;     // synchronous object
    Ant_LogControl* m_pLogControl;  // log control pointer

private:

    // Check the log start
    BOOL isStart();

private:

    // Disable the copy constructor and operator =
    Ant_LogImpl(const Ant_LogImpl&);
    Ant_LogImpl& operator = (const Ant_LogImpl&);

};

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif // ANT_LOGIMP_H

/*---------------------------------------------------------------------------*/
/* EOF */
