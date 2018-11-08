/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#include "stdafx.h"

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_LOGCONTROL_H
#   include "Ant_LogControl.h"
#endif
#ifndef ANT_VOLATILEMEMORY_H
#   include "Ant_VolatileMemory.h"
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
// Functions
Ant_LogControl::Ant_LogControl(DWORD dwMaxLogNum)
: m_dwMaxLogNum(dwMaxLogNum)
, m_pLogInfoMem(NULL)
{
    if (dwMaxLogNum == 0) {
        return;
    }

    // Log information buf size
    const DWORD s_dwLogInofMemSize = sizeof(Ant_LogRecordHeader) + sizeof(Ant_LogRecord)*dwMaxLogNum;

    // Create share memory for log info
    if (m_pLogInfoMem == NULL) {
        m_pLogInfoMem = new antsupporter::Ant_VolatileMemory(VMEM_LOG_BUF, Ant_String(PMEM_LOG_BUF), s_dwLogInofMemSize);
    }

    //Set log info buf as 0;
    if (m_pLogInfoMem != NULL) {
        m_pLogInfoMem->synchronizeStart();
        memset(m_pLogInfoMem->getAddress(), 0, s_dwLogInofMemSize);
        m_pLogInfoMem->synchronizeEnd();
    }
}

Ant_LogControl::~Ant_LogControl()
{
    if (m_pLogInfoMem != NULL) {
        delete m_pLogInfoMem;
        m_pLogInfoMem = NULL;
    }
}

VOID Ant_LogControl::output(const CHAR* szLogName, const CHAR* szLogInfo)
{
    //Check log name whether allow to write log information
    if (!checkLogName(szLogName)) {
        return;
    }

    if (szLogInfo == NULL) {
        return;
    }

    //Write log information to memory
    if (m_pLogInfoMem != NULL) {
        m_pLogInfoMem->synchronizeStart();
        writeLog(szLogName, szLogInfo);
        m_pLogInfoMem->synchronizeEnd();
    }
}

VOID Ant_LogControl::output(const CHAR* szLogName, const WCHAR* szLogInfo)
{
    if (!checkLogName(szLogName)) {
        return;
    }

    if (szLogInfo == NULL) {
        return;
    }

    if (m_pLogInfoMem != NULL) {
        m_pLogInfoMem->synchronizeStart();
        writeLog(szLogName, szLogInfo);
        m_pLogInfoMem->synchronizeEnd();
    }
}

BOOL Ant_LogControl::getAllLogInfo(Ant_LogRecord* pLogRecord, DWORD& dwGotRecordNum)
{
    dwGotRecordNum = 0;

    if (m_pLogInfoMem == NULL) {
        return FALSE;
    }

    BYTE* pbyAddress = m_pLogInfoMem->getAddress();
    if (pbyAddress == NULL) {
        return FALSE;
    }

    Ant_LogRecordHeader* pLogHeader = reinterpret_cast<Ant_LogRecordHeader*>(pbyAddress);
    Ant_LogRecord* pLogRecordMem = reinterpret_cast<Ant_LogRecord*>(pbyAddress + sizeof(Ant_LogRecordHeader));
    if (pLogHeader == NULL || pLogRecord == NULL) {
        return FALSE;
    }

    m_pLogInfoMem->synchronizeStart();
    //Copy log record to out memory
    dwGotRecordNum = pLogHeader->dwRecordNum;
    memcpy(pLogRecord, pLogRecordMem, sizeof(Ant_LogRecord)*dwGotRecordNum);

    //Clear memory
    pLogHeader->dwRecordNum = 0;
    pLogHeader->dwNextPos   = 0;
    memset(pLogRecordMem, 0, sizeof(Ant_LogRecord)*m_dwMaxLogNum);

    m_pLogInfoMem->synchronizeEnd();

    return  TRUE;
}

BOOL Ant_LogControl::getLogInfo(const CHAR* szLogName, Ant_LogRecord* pLogRrecord, DWORD& dwGotLogNum)
{
#if 1
    dwGotLogNum = 0;

    if (!checkLogName(szLogName)) {
        return FALSE;
    }

    if (m_pLogInfoMem == NULL) {
        return FALSE;
    }

    BYTE* pbyAddress = m_pLogInfoMem->getAddress();
    if (pbyAddress == NULL) {
        return FALSE;
    }

    Ant_LogRecordHeader* pLogHeader = reinterpret_cast<Ant_LogRecordHeader*>(pbyAddress);
    Ant_LogRecord* pLogRecordArray = reinterpret_cast<Ant_LogRecord*>(pbyAddress + sizeof(Ant_LogRecordHeader));
    if (pLogHeader == NULL || pLogRecordArray == NULL) {
        return FALSE;
    }

    m_pLogInfoMem->synchronizeStart();

    //Calculate the end log record's next position
    DWORD dwEndPosition = 0;
    if (pLogHeader->dwRecordNum == m_dwMaxLogNum) {
        dwEndPosition = m_dwMaxLogNum;
    }
    else {
        if (pLogHeader->dwNextPos > 0) {
            dwEndPosition = pLogHeader->dwNextPos;
        }
        else {
            dwEndPosition = 0;
        }
    }

    for (DWORD i = 0;  i < dwEndPosition;) {
        if (strcmp(szLogName, pLogRecordArray[i].szLogName) == 0) {
            //Copy log record to out memory
            pLogRrecord[dwGotLogNum] = pLogRecordArray[i];
            ++dwGotLogNum;

            //The log record is last.
            if (i == dwEndPosition - 1) {
                memset(&pLogRecordArray[i], 0, sizeof(Ant_LogRecord));
                --dwEndPosition;
            }
            //The log record is not last;
            else {
                //move memory
                memcpy(&pLogRecordArray[i], &pLogRecordArray[i + 1], (dwEndPosition - i - 1)*sizeof(Ant_LogRecord));
                --dwEndPosition;
                memset(&pLogRecordArray[dwEndPosition], 0, sizeof(Ant_LogRecord));
            }
        }
        else {
            i++;
        }
    }

    if (dwGotLogNum > 0) {
        pLogHeader->dwNextPos = dwEndPosition;
        pLogHeader->dwRecordNum -= dwGotLogNum;
    }

    m_pLogInfoMem->synchronizeEnd();
#endif


    return  TRUE;
}

VOID Ant_LogControl::clearAllLogInfo()
{
    if (NULL == m_pLogInfoMem) {
        return;
    }

    BYTE* pbyAddress = m_pLogInfoMem->getAddress();
    if (pbyAddress == NULL) {
        return;
    }

    Ant_LogRecordHeader* pLogHeader = reinterpret_cast<Ant_LogRecordHeader*>(pbyAddress);
    if (pLogHeader ==  NULL) {
        return;
    }

    m_pLogInfoMem->synchronizeStart();
    pLogHeader->dwRecordNum = 0;
    pLogHeader->dwNextPos   = 0;
    memset(pbyAddress+sizeof(Ant_LogRecordHeader), 0, sizeof(Ant_LogRecord)*m_dwMaxLogNum);
    m_pLogInfoMem->synchronizeEnd();
}

BOOL Ant_LogControl::checkLogName(const CHAR* szLogNum)
{
    if (szLogNum == NULL) {
        return FALSE;
    }

    BOOL bRet = FALSE;

    for (INT i = 0;;i++) {
        if (s_szLogNameTable[i] == NULL) {
            break;
        }

        if (strcmp(szLogNum, s_szLogNameTable[i]) == 0) {
            bRet = TRUE;
            break;
        }
    }

    return bRet;
}

VOID Ant_LogControl::writeLog(const CHAR* szLogName, const CHAR* szLogInfo)
{
    //Get memory address
    BYTE* pbyAddress = m_pLogInfoMem->getAddress();
    if (pbyAddress == NULL) {
        return;
    }

    Ant_LogRecordHeader* pLogHeader = reinterpret_cast<Ant_LogRecordHeader*>(pbyAddress);
    Ant_LogRecord* pLoRecord = reinterpret_cast<Ant_LogRecord*>(pbyAddress + sizeof(Ant_LogRecordHeader));

    if (pLogHeader == NULL || pLoRecord == NULL) {
        return;
    }

    //Get the current log record position
    DWORD dwUpdatePos = pLogHeader->dwNextPos;
    //Get the current log record number
    pLoRecord[dwUpdatePos].dwLogNum = pLogHeader->dwLogNum;

    //Copy log name to memory
    strncpy(pLoRecord[dwUpdatePos].szLogName, szLogName, LOG_LOGNAME_LEN);

    //Copy log information to memory
    strncpy(pLoRecord[dwUpdatePos].szLogInfo, szLogInfo, LOG_LOGINFO_LEN);

    //Get current date and time
    Ant_Time cTime;
    pLoRecord[dwUpdatePos].cLogDate.wYear  = cTime.getYear();
    pLoRecord[dwUpdatePos].cLogDate.wMonth = cTime.getMonth();
    pLoRecord[dwUpdatePos].cLogDate.wDay   = cTime.getDay();
    pLoRecord[dwUpdatePos].cLogDate.wReserved = 0;
    pLoRecord[dwUpdatePos].cLogTime.wHour  = cTime.getHour();
    pLoRecord[dwUpdatePos].cLogTime.wMin   = cTime.getMinute();
    pLoRecord[dwUpdatePos].cLogTime.wSec   = cTime.getSecond();
    pLoRecord[dwUpdatePos].cLogTime.wMSec  = cTime.getMillisecond();

    pLoRecord[dwUpdatePos].dwTickCount     = GetTickCount();

    if (m_dwMaxLogNum > pLogHeader->dwRecordNum) {
        ++(pLogHeader->dwRecordNum);
    }

    //Next log record position update
    if (m_dwMaxLogNum <= (++(pLogHeader->dwNextPos))) {
        pLogHeader->dwNextPos = 0;
    }

    //Log number
    if (LOG_MAX_NUM == pLogHeader->dwLogNum) {
        pLogHeader->dwLogNum = 0;
    }
    else {
        ++(pLogHeader->dwLogNum);
    }
}

VOID Ant_LogControl::writeLog(const CHAR* szLogName, const WCHAR* szLogInfo)
{
    Ant_String cLogInfo(szLogInfo);
    CHAR szTempLogInfo[LOG_LOGINFO_LEN];
    memset(szTempLogInfo, 0, sizeof(szTempLogInfo));
    if (0 != cLogInfo.getString(ANT_CP_UTF8, szTempLogInfo, LOG_LOGINFO_LEN)) {
        writeLog(szLogName, szTempLogInfo);
    }
}

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
/* EOF */
