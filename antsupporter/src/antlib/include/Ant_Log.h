/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_LOG_H
#define ANT_LOG_H

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
/**
 * class Ant_Log
 *
 */
class ANTLIB_API Ant_Log : public virtual Ant_Object
{
public:

    /**
     * Ant_Log
     *
     * Constructor
     *
     * @param null
     */
    Ant_Log();

    /**
     * ~Ant_Log
     *
     * Destructor
     *
     * @param null
     */
    ~Ant_Log();

    /**
     * Start
     *
     * Start log
     *
     * @param DWORD dwMaxLogNum [IN]: Log record max num
     *
     * @return VOID
     */
    static VOID start(DWORD dwMaxLogNum);

    /**
     * Stop
     *
     * Stop log
     *
     * @param null
     *
     * @return VOID
     */
    static VOID stop();

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
    static VOID output(const CHAR* szLogName, const CHAR* szLogInfo);

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
    static VOID output(const CHAR* szLogName, const WCHAR* szLogInfo);

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

    // Disable the copy constructor and operator =
    Ant_Log(const Ant_Log& cLog);
    Ant_Log& operator = (const Ant_Log& cLog);

};

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif // ANT_LOG_H

/*---------------------------------------------------------------------------*/
/* EOF */
