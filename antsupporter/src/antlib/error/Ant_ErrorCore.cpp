/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#include "stdafx.h"

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_ERRORCORE_H
#   include "Ant_ErrorCore.h"
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
#ifndef ANT_DEBUG_H
#   include "Ant_Debug.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Functions
Ant_ErrorCore::Ant_ErrorCore():m_dwMaxRecNum(0)
{
    for (INT i = 0; i < ANT_ERROR_TYPE_NUM; ++i) {
        m_pcErrMemory[i] = NULL;
    }
}

Ant_ErrorCore::~Ant_ErrorCore()
{
    for (INT i = 0; i < ANT_ERROR_TYPE_NUM; ++i) {
        m_pcErrMemory[i] = NULL;
    }
    m_dwMaxRecNum = 0;
}

VOID Ant_ErrorCore::start(const XCHAR* szMemName[], INT iMemNum, DWORD dwRecNum)
{
    if ((NULL == szMemName) || (iMemNum <= 0) || (dwRecNum <= 0)) {
        return;
    }

    m_dwMaxRecNum = dwRecNum;
    const DWORD dwErrorDataSize = sizeof(ErrorHeader) + sizeof(ErrorRecord)*dwRecNum;

    VolatileMemoryID eMemIDArray[ANT_ERROR_TYPE_NUM] = {VMEM_ERROR_ERROR, VMEM_ERROR_DEBUG, VMEM_ERROR_FATAL};

    for (INT i = 0; i < iMemNum && i < ANT_ERROR_TYPE_NUM; ++i) {
        if (NULL != szMemName[i] && m_pcErrMemory[i] == NULL) {
            m_pcErrMemory[i] = new Ant_VolatileMemory(eMemIDArray[i], Ant_String(szMemName[i]), dwErrorDataSize);
        }
    }
}

VOID Ant_ErrorCore::stop()
{
    for (INT i = 0; i < ANT_ERROR_TYPE_NUM; ++i) {
        if (NULL != m_pcErrMemory[i]) {
            delete m_pcErrMemory[i];
            m_pcErrMemory[i] = NULL;
        }
    }
}

VOID Ant_ErrorCore::outputError(const CHAR* pkszFileInfo, DWORD dwLineNo, LONG iErrorCode, DWORD dwOption, AntErrorType eErrType)
{
    Ant_VolatileMemory* pMemory = NULL;
    BYTE* pErrorBuf = NULL;

    pMemory = m_pcErrMemory[eErrType];

    if (pMemory == NULL) {
        ANT_PERROR("[Ant_ErrorCore]OutputError(%d, %ld) - cann't get memory!", eErrType, iErrorCode);
        return;
    }
    pErrorBuf = pMemory->getAddress();
    pMemory->synchronizeStart();
    writeError(pkszFileInfo, dwLineNo, iErrorCode, dwOption, pErrorBuf);
    pMemory->synchronizeEnd();
}

VOID Ant_ErrorCore::outputError(const WCHAR* pkszFileInfo, DWORD dwLineNo, LONG iErrorCode, DWORD dwOption, AntErrorType eErrType)
{
    Ant_VolatileMemory* pMemory = NULL;
    BYTE* pErrorBuf = NULL;

    pMemory = m_pcErrMemory[eErrType];

    if (pMemory == NULL) {
        ANT_PERROR("[Ant_ErrorCore]OutputError(%d, %ld) - cann't get memory!", eErrType, iErrorCode);
        return;
    }
    pErrorBuf = pMemory->getAddress();
    pMemory->synchronizeStart();
    writeError(pkszFileInfo, dwLineNo, iErrorCode, dwOption, pErrorBuf);
    pMemory->synchronizeEnd();
}

BOOL Ant_ErrorCore::getErrorInfo(AntErrorType eType, ErrorHeader& sErrorHeader, ErrorRecord* pErrorRecord, DWORD dwRecNum)
{
    if (NULL == pErrorRecord) {
        return FALSE;
    }

    memset(&sErrorHeader, 0, sizeof(ErrorHeader));

    DWORD dwGetRecNum = dwRecNum;
    if (dwGetRecNum > m_dwMaxRecNum) {
        dwGetRecNum = m_dwMaxRecNum;
    }

    Ant_VolatileMemory* pMemory = NULL;
    BYTE* pErrorBuf = NULL;

    pMemory = m_pcErrMemory[eType];

    if (pMemory == NULL) {
        ANT_PERROR("[Ant_ErrorCore]GetErrorInfo(%d) - cann't get memory!", eType);
        return FALSE;
    }
    pErrorBuf = pMemory->getAddress();
    ErrorHeader* pErrHeader = reinterpret_cast<ErrorHeader*>(pErrorBuf);
    ErrorRecord* pErrRecord = reinterpret_cast<ErrorRecord*>(pErrorBuf + sizeof(ErrorHeader));
    if (NULL == pErrHeader || NULL == pErrRecord ) {
        return FALSE;
    }

    pMemory->synchronizeStart();
    if (pErrHeader->eMemState == ANT_ERROR_MEM_UPDATE) {
        pErrHeader->eMemState =  ANT_ERROR_MEM_GOTTEN;
        sErrorHeader = *pErrHeader;
        memcpy(pErrorRecord, pErrRecord, sizeof(ErrorRecord)*dwGetRecNum);
        pMemory->synchronizeEnd();
        return TRUE;
    }

    pMemory->synchronizeEnd();

    return FALSE;
}

VOID Ant_ErrorCore::writeError(const CHAR* pkszFileInfo, DWORD dwLineNo, LONG iErrorCode, DWORD dwOption, BYTE* pbyErrBuf)
{
    if (NULL == pbyErrBuf) {
        return;
    }

    ErrorHeader* pErrHeader = reinterpret_cast<ErrorHeader*>(pbyErrBuf);
    ErrorRecord* pErrRecord = reinterpret_cast<ErrorRecord*>(pbyErrBuf + sizeof(ErrorHeader));

    if ((NULL == pErrHeader) || (NULL == pErrRecord)) {
        return;
    }

    Ant_Time cTime;
    cTime.getNow();
    DWORD dwUpdatePos = pErrHeader->dwNextPos;
    pErrRecord[dwUpdatePos].dwErrorNum = pErrHeader->dwErrNum;
    memcpy(pErrRecord[dwUpdatePos].szFileInfo, pkszFileInfo, ERROR_FILEINFO_SIZE);
    pErrRecord[dwUpdatePos].dwErrLine = dwLineNo;
    pErrRecord[dwUpdatePos].lErrCode = iErrorCode;
    pErrRecord[dwUpdatePos].dwOption = dwOption;
    pErrRecord[dwUpdatePos].sErrDate.wYear = cTime.getYear();
    pErrRecord[dwUpdatePos].sErrDate.wMon  = cTime.getMonth();
    pErrRecord[dwUpdatePos].sErrDate.wDay  = cTime.getDay();
    pErrRecord[dwUpdatePos].sErrDate.wPading = 0;
    pErrRecord[dwUpdatePos].sErrTime.wHour = cTime.getHour();
    pErrRecord[dwUpdatePos].sErrTime.wMin  = cTime.getMinute();
    pErrRecord[dwUpdatePos].sErrTime.wSec  = cTime.getSecond();
    pErrRecord[dwUpdatePos].sErrTime.wMSec = cTime.getMillisecond();

    if (m_dwMaxRecNum > pErrHeader->dwRecordNum) {
        ++(pErrHeader->dwRecordNum);
    }

    if (m_dwMaxRecNum <= (++(pErrHeader->dwNextPos))) {
        pErrHeader->dwNextPos = 0;
        pErrHeader->bMaxRecNumFlag = TRUE;
    }

    if (ERROR_ERRORNUM_MAX == pErrHeader->dwErrNum) {
        pErrHeader->dwErrNum = 0;
    }
    else {
        ++(pErrHeader->dwErrNum);
    }
    
    pErrHeader->eMemState = ANT_ERROR_MEM_UPDATE;
}

VOID Ant_ErrorCore::writeError(const WCHAR* pkszFileInfo, DWORD dwLineNo, LONG iErrorCode, DWORD dwOption, BYTE* pbyErrBuf)
{
    if (NULL == pbyErrBuf) {
        return;
    }

    Ant_String strFileInfo(pkszFileInfo);
    CHAR szFileInfo[ERROR_MAX_PATH] = {0};

    if (0 != strFileInfo.getString(ANT_CP_UTF8, szFileInfo, ERROR_MAX_PATH)) {
        writeError(szFileInfo, dwLineNo, iErrorCode, dwOption, pbyErrBuf);
    }
}

BOOL Ant_ErrorCore::clearAllErrorInfo()
{
    const DWORD dwErrorDataSize = sizeof(ErrorHeader) + sizeof(ErrorRecord)*m_dwMaxRecNum;

    for (INT i = 0; i < ANT_ERROR_TYPE_NUM; ++i) {
        if (m_pcErrMemory[i] != NULL) {
            m_pcErrMemory[i]->synchronizeStart();
            BYTE* pbyErrBuf = m_pcErrMemory[i]->getAddress();
            if (pbyErrBuf != NULL) {
                memset(pbyErrBuf, 0, dwErrorDataSize);
            }
            m_pcErrMemory[i]->synchronizeEnd();
        }
    }

    return TRUE;
}

BOOL Ant_ErrorCore::initMemory(AntErrorType eErrType , BYTE *pbyErrMem)
{
    const DWORD dwErrorDataSize = sizeof(ErrorHeader) + sizeof(ErrorRecord)*m_dwMaxRecNum;

    if (m_pcErrMemory[eErrType] != NULL) {
        m_pcErrMemory[eErrType]->synchronizeStart();
        BYTE* pbyErrBuf = m_pcErrMemory[eErrType]->getAddress();
        if (pbyErrBuf != NULL) {
            memcpy(pbyErrBuf, pbyErrMem, dwErrorDataSize);
        }

        m_pcErrMemory[eErrType]->synchronizeEnd();
    }

    return TRUE;
}

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
/* EOF */
