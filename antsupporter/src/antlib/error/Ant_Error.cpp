/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#include "stdafx.h"

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_ERROR_H
#   include "Ant_Error.h"
#endif
#ifndef ANT_ERRORCORE_H
#   include "Ant_ErrorCore.h"
#endif
#ifndef ANT_AUTOSYNC_H
#   include "Ant_AutoSync.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Value define

Ant_ErrorCore*  Ant_Error::s_pcErrorCore;
BOOL         Ant_Error::s_bStartFlag = FALSE;
Ant_SyncObj     Ant_Error::s_cSyncOject;

/*---------------------------------------------------------------------------*/
// Functions
VOID  Ant_Error::start(const XCHAR* szMemoryNameArray[], INT iArraySize, DWORD dwRecNum)
{
    Ant_AutoSync cAutoSync(s_cSyncOject);
    if (NULL == s_pcErrorCore) {
        s_pcErrorCore = new Ant_ErrorCore();
        s_pcErrorCore->start(szMemoryNameArray, iArraySize, dwRecNum);
        s_bStartFlag = TRUE;
    }
}

VOID Ant_Error::stop()
{
    Ant_AutoSync cAutoSync(s_cSyncOject);

    s_bStartFlag = FALSE;
    if (NULL != s_pcErrorCore) {
        s_pcErrorCore->stop();
        delete s_pcErrorCore;
        s_pcErrorCore = NULL;
    }
}

VOID  Ant_Error::outputError(const CHAR* szFileInfo, DWORD dwLineNo, LONG lErrCode, DWORD dwOption, AntErrorType eErrType)
{    
    Ant_AutoSync cAutoSync(s_cSyncOject);
    if (isStart()) {
        s_pcErrorCore->outputError(szFileInfo, dwLineNo, lErrCode, dwOption, eErrType);
    }
}

VOID Ant_Error::outputError(const WCHAR* szFileInfo, DWORD dwLineNo, LONG lErrCode, DWORD dwOption, AntErrorType eErrType)
{
    Ant_AutoSync cAutoSync(s_cSyncOject);
    if (isStart()) {
        s_pcErrorCore->outputError(szFileInfo, dwLineNo, lErrCode, dwOption, eErrType);
    }
}

BOOL Ant_Error::getErrorInfo(AntErrorType eErrType, ErrorHeader& sErrHeader, ErrorRecord* pErrRecord, DWORD dwRecNum)
{    
    Ant_AutoSync cAutoSync(s_cSyncOject);
    BOOL bRet = FALSE;
    if (isStart()) {
        bRet = s_pcErrorCore->getErrorInfo(eErrType, sErrHeader, pErrRecord, dwRecNum);
    }

    return bRet;
}

BOOL Ant_Error::clearAllErrorInfo()
{
    Ant_AutoSync cAutoSync(s_cSyncOject);
    BOOL bRet = FALSE;
    if (isStart()) {
        bRet = s_pcErrorCore->clearAllErrorInfo();
    }

    return bRet;
}

BOOL Ant_Error::initMemory(AntErrorType eErrType, BYTE* pbyErrMem)
{
    Ant_AutoSync cAutoSync(s_cSyncOject);
    BOOL bRet = FALSE;
    if (isStart()) {
        bRet = s_pcErrorCore->initMemory(eErrType, pbyErrMem);
    }

    return bRet;
}

BOOL Ant_Error::isStart()
{
    BOOL bRet = FALSE;
    if ((NULL != s_pcErrorCore) && s_bStartFlag) {
        bRet = TRUE;
    }

    return bRet;
}

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
/* EOF */
