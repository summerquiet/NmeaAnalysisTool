/**
 * Copyright @ 2013 - 2015 Suntec Software(Shanghai) Co., Ltd.
 * All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are NOT permitted except as agreed by
 * Suntec Software(Shanghai) Co., Ltd.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 */

#include "stdafx.h"
#ifndef CXX_BL_TASKDISPATCHER_H
#   include "BL_TaskDispatcher.h"
#endif
#include "BL_ThreadPoolManager.h"

#define CHECKIDLE_WORKTHREAD_TASKCOUNT 16

BL_TaskDispatcher::BL_TaskDispatcher(BL_ThreadPoolManager * pPoolMgr)
: m_pThreadPoolMgr(pPoolMgr) 
, m_pTask(NULL)
{
}

BL_TaskDispatcher::~BL_TaskDispatcher()
{
}

BL_SPWORKTHREAD BL_TaskDispatcher::GetTaskRunThread(BL_ThreadPoolTask* pcTaskItem)
{
    m_pTask = pcTaskItem;
    return m_pThreadPoolMgr->GetThread(this, &BL_TaskDispatcher::GetTaskRunThreadCB);
}

BL_SPWORKTHREAD BL_TaskDispatcher::GetTaskRunThreadCB(WorkThreadList & cThreadList)
{
    BL_SPWORKTHREAD pcWorkThread;

    if (NULL == m_pThreadPoolMgr || NULL == m_pTask) {
        return pcWorkThread;
    }

    if (m_pTask->GetTaskGroupId() > 0) {
        pcWorkThread = GetGroupIdThread(cThreadList, m_pTask->GetTaskGroupId());
    }

    if (!pcWorkThread) {
        pcWorkThread = GetIdleThread(cThreadList);
    }

    if (!pcWorkThread) {
        pcWorkThread = m_pThreadPoolMgr->AddWorkThread();
    }

    if (!pcWorkThread) {
        pcWorkThread = GetAvailableThread(cThreadList);
    }

    return pcWorkThread;
}

BL_SPWORKTHREAD BL_TaskDispatcher::GetGroupIdThread(WorkThreadList & cThreadList, INT nGroupId)
{
    BL_SPWORKTHREAD spWorkThread;
    WorkThreadList::iterator it;
    for (it = cThreadList.begin(); it != cThreadList.end(); ++it) {
        if ((*it)->hasTaskOfGroup(nGroupId)) {
            return *it;
        }
    }
    return spWorkThread;
}

BL_SPWORKTHREAD BL_TaskDispatcher::GetIdleThread(WorkThreadList & cThreadList)
{
    BL_SPWORKTHREAD spWorkThread;
    WorkThreadList::iterator it;
    for (it = cThreadList.begin(); it != cThreadList.end(); ++it) {
        if ((*it)->GetRunTaskCnt() == 0) {
            return *it;
        }
    }
    return spWorkThread;
}

BL_SPWORKTHREAD BL_TaskDispatcher::GetAvailableThread(WorkThreadList & cThreadList)
{
    BL_SPWORKTHREAD spWorkThread;
    INT nMinCntOfIdleThreadTask = 0;

    WorkThreadList::iterator it = cThreadList.begin();
    if (it != cThreadList.end()) {
        spWorkThread = *it;
        nMinCntOfIdleThreadTask = (*it)->GetRunTaskCnt();
        for (; it != cThreadList.end(); ++it) {
            INT nCurrentRunCount = (*it)->GetRunTaskCnt();
            if (nCurrentRunCount == 0) {
                spWorkThread = *it;
                nMinCntOfIdleThreadTask = nCurrentRunCount;
                break;
            }
            else {
                if (nCurrentRunCount < nMinCntOfIdleThreadTask) {
                    spWorkThread = *it;
                    nMinCntOfIdleThreadTask = nCurrentRunCount;
                }
            }
        }
    }

    if (nMinCntOfIdleThreadTask > CHECKIDLE_WORKTHREAD_TASKCOUNT) {
        spWorkThread.reset();
    }

    return spWorkThread;
}


/* EOF */