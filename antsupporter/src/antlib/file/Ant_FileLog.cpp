/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#include "stdafx.h"

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_FILELOG_H
#   include "Ant_FileLog.h"
#endif
#ifndef ANT_LOG_H
#   include "Ant_Log.h"
#endif
#ifndef ANT_TIME_H
#   include "Ant_Time.h"
#endif
#if defined(_WIN32) || defined(_WIN32_WCE)
#   include <Tchar.h>
#else   // For Linux
#   include <pthread.h>
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Functions
Ant_FileLog::Ant_FileLog(const Ant_String& strFuncName, const Ant_String& strFilePath, const BOOL& bRecLog)
: m_dwStartCount(0)
//, m_dwStartThreadTime(0)
, m_dwThreadId(0)
//, m_dwProcessId(0)
, m_bRecLog(bRecLog)
{
    memset(m_szFuncName, 0, sizeof(m_szFuncName));
    memset(m_szFilePath, 0, sizeof(m_szFilePath));
    strFuncName.getString(m_szFuncName, ANT_FILE_LOG_MAX_NAME);
    strFilePath.getString(m_szFilePath, ANT_FILE_LOG_MAX_NAME);

    Ant_Time cTime;
    m_dwStartCount = cTime.getTickCount();
#if defined(_WIN32) || defined(_WIN32_WCE)
    m_dwThreadId = ::GetCurrentThreadId();
#else // Linux
    m_dwThreadId = (DWORD)::pthread_self();
#endif
    if( m_bRecLog ) {
        TCHAR szLogInfo[antsupporter::LOG_LOGINFO_LEN];
        memset(szLogInfo, 0, sizeof(szLogInfo));
        _sntprintf(szLogInfo, antsupporter::LOG_LOGINFO_LEN, TEXT("CAL,%.50s(),%.50s,Th:%x,S:%lu"), m_szFuncName, m_szFilePath, static_cast<INT>(m_dwThreadId), m_dwStartCount);
        antsupporter::Ant_Log::output("CLFILE", szLogInfo);
    }
}

Ant_FileLog::~Ant_FileLog()
{
    if( 0 != m_dwThreadId && m_bRecLog ) {
        Ant_Time cTime;
        DWORD dwEndCount = cTime.getTickCount();
        TCHAR szLogInfo[antsupporter::LOG_LOGINFO_LEN];
        memset(szLogInfo, 0, sizeof(szLogInfo));
        _sntprintf(szLogInfo, antsupporter::LOG_LOGINFO_LEN, TEXT("RET,%.50s(),%.50s,Th:%x,S:%lu,E:%lu,TC:%lu"),
            m_szFuncName, m_szFilePath, static_cast<INT>(m_dwThreadId), m_dwStartCount, dwEndCount, (dwEndCount - m_dwStartCount));
        antsupporter::Ant_Log::output("CLFILE", szLogInfo);
    }
}

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
/* EOF */
