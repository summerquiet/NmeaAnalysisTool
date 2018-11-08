/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_TIMERMANAGER_WIN_H
#define ANT_TIMERMANAGER_WIN_H
#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_NEWTYPESDEFINE_H
#   include "Ant_NewTypesDefine.h"
#endif
#ifndef ANT_LIST_H
#   include "Ant_List.h"
#endif
#ifndef ANT_THREAD_H
#   include "Ant_Thread.h"
#endif
#ifndef ANT_SYNCOBJ_H
#   include "Ant_SyncObj.h"
#endif
#ifndef ANT_TIMERCALLBACK_H
#include "Ant_TimerCallback.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Value define
#define ANT_WM_TIMER            (WM_APP + 0x1000)
#define ANT_TIMER_MAX_NUM       (256)

/*---------------------------------------------------------------------------*/
// Class declaration
class Ant_TimerManager;
class Ant_Timer;
class Ant_TimerRequest;

/**
 * @brief class represents a timer request.
 *
 * @detail a timer request is created when a timer runs out.
 */
class Ant_TimerRequest : public virtual Ant_Object
{
public:

    DWORD       timeout_tick;
    DWORD       thread_id;
    Ant_Timer*  timer;

    Ant_TimerRequest(){timer = NULL;}
    ~Ant_TimerRequest(){}

    BOOL operator == (const Ant_TimerRequest& req) const;

    BOOL operator <  (const Ant_TimerRequest& req) const;

    BOOL operator <= (const Ant_TimerRequest& req) const;

    BOOL operator >  (const Ant_TimerRequest& req) const;

    BOOL operator >= (const Ant_TimerRequest& req) const;

private:

    // Disable the copy constructor and operator =
    Ant_TimerRequest(const Ant_TimerRequest& src);
    Ant_TimerRequest& operator =(const Ant_TimerRequest& src);

};

/**
 * @brief class helps us manages timers in current process.
 *
 * @detail manages all timers in current process, and it's responsible of
 *         posting timer request when a timer runs out.
 */
class Ant_TimerManager : public Ant_Thread
{
public:

    Ant_TimerManager();
    virtual ~Ant_TimerManager();

    virtual BOOL registerTimer(Ant_Timer* timer);

    virtual VOID removeTimer(Ant_Timer* timer);

    virtual BOOL isValid(INT id);

    virtual VOID setInvalid(INT id);

    virtual BOOL requestSanity();

    VOID setTimerCallback(Ant_TimerCallback * cTimerCB);

protected:

    virtual VOID addRequest(Ant_TimerRequest* new_req);

    virtual VOID removeRequest(DWORD id);

    virtual DWORD nextID();

    virtual VOID run();

    BYTE                        timer_table[ANT_TIMER_MAX_NUM];
    Ant_List <Ant_TimerRequest> m_cReqList;
    HANDLE                      m_evHandle;
    DWORD                       m_dNextid;
    BOOL                        need_sanity_reply;
    Ant_SyncObj                 m_cSyncObj;
    Ant_TimerCallback*          m_ptimercallback;

private:

    // Disable the copy constructor and operator =
    Ant_TimerManager(const Ant_TimerManager& src);
    Ant_TimerManager& operator =(const Ant_TimerManager& src);

};

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif  // ANT_TIMERMANAGER_WIN_H

/*---------------------------------------------------------------------------*/
/* EOF */
