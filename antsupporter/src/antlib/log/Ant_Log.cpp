/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#include "stdafx.h"

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_LOG_H
#   include "Ant_Log.h"
#endif
#ifndef ANT_LOGIMPL_H
#   include "Ant_LogImpl.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Functions
Ant_Log::Ant_Log()
{
   
}

Ant_Log::~Ant_Log()
{

}

Ant_LogImpl& getLogImpl()
{
    static Ant_LogImpl _instance;
    return _instance;
}

VOID Ant_Log::start(DWORD dwMaxLogNum)
{
    Ant_LogImpl& log = getLogImpl();
    log.start(dwMaxLogNum);
}

VOID Ant_Log::stop()
{
    Ant_LogImpl& log = getLogImpl();
    log.stop();
}

VOID Ant_Log::output(const CHAR* szLogName, const CHAR* szLogInfo)
{
    Ant_LogImpl& log = getLogImpl();
    log.output(szLogName, szLogInfo);
}

VOID Ant_Log::output(const CHAR* szLogName, const WCHAR* szLogInfo)
{
    Ant_LogImpl& log = getLogImpl();
    log.output(szLogName, szLogInfo);
}

BOOL Ant_Log::getAllLogInfo(Ant_LogRecord* pLogRecord, DWORD& dwGotRecordNum)
{
    Ant_LogImpl& log = getLogImpl();
    return log.getAllLogInfo(pLogRecord, dwGotRecordNum);
}

BOOL Ant_Log::getLogInfo(const CHAR* szLogName, Ant_LogRecord* pLogRrecord, DWORD& dwGotLogNum)
{
    Ant_LogImpl& log = getLogImpl();
    return log.getLogInfo(szLogName, pLogRrecord, dwGotLogNum);
}

VOID Ant_Log::clearAllLogInfo()
{
    Ant_LogImpl& log = getLogImpl();
    log.clearAllLogInfo();
}

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
/* EOF */
