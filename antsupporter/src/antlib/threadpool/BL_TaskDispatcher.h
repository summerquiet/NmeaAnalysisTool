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
  /**
 * @file BL_TaskDispatcher.h 
 * @brief BL_TaskDispatcher
 *
 * including definition and interface for BL_TaskDispatcher
 */

#ifndef CXX_BL_TASKDISPATCHER_H
#define CXX_BL_TASKDISPATCHER_H
#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef CXX_BL_THREADPOOL_H
#   include "BL_ThreadPool.h"
#endif

#ifndef CXX_BL_THREADPOOLMONITOR_H
#   include "BL_ThreadPoolMonitor.h"
#endif

#ifndef CXX_BL_THREADPOOLWORKTHREAD_H
#   include "BL_ThreadPoolWorkThread.h"
#endif

#include "MEM_list.h"

class BL_ThreadPoolManager;
typedef boost::shared_ptr<BL_ThreadPoolWorkThread>    BL_SPWORKTHREAD;
typedef BaseList<BL_SPWORKTHREAD>::type      WorkThreadList;

/**
* @description of BL_TaskDispatcher
*
* @details definition of BL_TaskDispatcher Implement
*/
class BL_TaskDispatcher
{
public:
    friend class BL_ThreadPoolManager;

    BL_TaskDispatcher(BL_ThreadPoolManager * pPoolMgr);

    virtual ~BL_TaskDispatcher();

    BL_SPWORKTHREAD GetTaskRunThread(BL_ThreadPoolTask* pcTaskItem);

    virtual BL_SPWORKTHREAD GetTaskRunThreadCB(WorkThreadList & cThreadList);

private:

    BL_SPWORKTHREAD GetGroupIdThread(WorkThreadList & cThreadList, INT nGroupId);
    BL_SPWORKTHREAD GetIdleThread(WorkThreadList & cThreadList);
    BL_SPWORKTHREAD GetAvailableThread(WorkThreadList & cThreadList);

    BL_ThreadPoolTask *    m_pTask;

    BL_ThreadPoolManager * m_pThreadPoolMgr;
    BL_TaskDispatcher(const BL_TaskDispatcher& c);
    BL_TaskDispatcher& operator=(const BL_TaskDispatcher& c);
};

#endif /* >>CXX_BL_TASKDISPATCHER_H<< */
/* EOF */
