/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#include "stdafx.h"

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_LOGIMPL_H
#   include "Ant_LogImpl.h"
#endif
#ifndef ANT_LOGCONTROL_H
#   include "Ant_LogControl.h"
#endif
#ifndef ANT_AUTOSYNC_H
#   include "Ant_AutoSync.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Functions
Ant_LogImpl::Ant_LogImpl()
{
    
}

Ant_LogImpl::~Ant_LogImpl()
{

}

VOID Ant_LogImpl::start(DWORD dwMaxLogNum)
{
    Ant_AutoSync cAutoSync(m_cSyncObj);

    if (m_pLogControl == NULL) {
        m_pLogControl = new Ant_LogControl(dwMaxLogNum);
    }
}

VOID Ant_LogImpl::stop()
{
    Ant_AutoSync cAutoSync(m_cSyncObj);

    if (m_pLogControl != NULL) {
        delete m_pLogControl;
        m_pLogControl = NULL;
    }
}

VOID Ant_LogImpl::output(const CHAR* szLogName, const CHAR* szLogInfo)
{
    Ant_AutoSync cAutoSync(m_cSyncObj);

    if (!isStart()) {
        return;
    }

    m_pLogControl->output(szLogName, szLogInfo);
}

VOID Ant_LogImpl::output(const CHAR* szLogName, const WCHAR* szLogInfo)
{
    Ant_AutoSync cAutoSync(m_cSyncObj);

    if (!isStart()) {
        return;
    }

    m_pLogControl->output(szLogName, szLogInfo);
}

BOOL Ant_LogImpl::getAllLogInfo(Ant_LogRecord* pLogRecord, DWORD& dwGotRecordNum)
{
    Ant_AutoSync cAutoSync(m_cSyncObj);

    if (!isStart()) {
        return FALSE;
    }

    return m_pLogControl->getAllLogInfo(pLogRecord, dwGotRecordNum);
}

BOOL Ant_LogImpl::getLogInfo(const CHAR* szLogName, Ant_LogRecord* pLogRrecord, DWORD& dwGotLogNum)
{
    Ant_AutoSync cAutoSync(m_cSyncObj);

    if (!isStart()) {
        return FALSE;
    }

    return m_pLogControl->getLogInfo(szLogName, pLogRrecord, dwGotLogNum);
}

VOID Ant_LogImpl::clearAllLogInfo()
{
    Ant_AutoSync cAutoSync(m_cSyncObj);

    if (!isStart()) {
        return;
    }

    m_pLogControl->clearAllLogInfo();
}

BOOL Ant_LogImpl::isStart()
{
    return (m_pLogControl != NULL) ? TRUE : FALSE;
}

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
/* EOF */
