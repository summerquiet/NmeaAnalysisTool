#include "stdafx.h"
#ifndef CXX_BL_THREADPOOL_H
#   include "BL_ThreadPool.h"
#endif

#ifndef CXX_BL_THREADPOOLMANAGER_H
#   include "BL_ThreadPoolManager.h"
#endif

BL_ThreadPoolManager* BL_ThreadPool::m_cManager = NULL;

CL_BOOL BL_ThreadPool::Initialize(BL_ThreadPoolConfig &config)
{
    if (NULL == m_cManager)
    {
        m_cManager = new(MEM_Base) BL_ThreadPoolManager(config);
        if (NULL != m_cManager)
        {
            m_cManager->RegisterName(XTEXT("BL_ThreadPoolMgr"));
            m_cManager->Start();
        }
    }
    return CL_TRUE;
}

CL_BOOL BL_ThreadPool::UnInitialize()
{
    if (NULL != m_cManager)
    {
        m_cManager->Stop();
        delete m_cManager;
        m_cManager = NULL;
    }
    return CL_TRUE;
}

CL_BOOL BL_ThreadPool::StartTask(BL_ThreadPoolTask* pcTask)
{
    if (NULL != m_cManager) {
        return m_cManager->StartTask(pcTask);
    }
    else {
        return CL_FALSE;
    }
}

VOID BL_ThreadPool::CancelTaskByType(BL_TASKTYPE nTaskType)
{
    if (NULL != m_cManager) {
        m_cManager->CancelTaskByType(nTaskType);
    }
}

VOID BL_ThreadPool::CancelTaskByID(BL_TASKID nTaskID)
{
    if (NULL != m_cManager) {
        m_cManager->CancelTaskByID(nTaskID);
    }
}

BL_String BL_ThreadPool::dumpStatus()
{
    BL_String sStatus;
    if (NULL != m_cManager) {
        sStatus = m_cManager->dumpStatus();
    }
    return sStatus;
}