/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_TIMERMANAGER_UNIX_H
#define ANT_TIMERMANAGER_UNIX_H
#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_NEWTYPESDEFINE_H
#   include "Ant_NewTypesDefine.h"
#endif
#ifndef ANT_OBJECT_H
#   include "Ant_Object.h"
#endif
#ifndef ANT_LIST_H
#   include "Ant_List.h"
#endif
#ifndef ANT_SYNCOBJ_H
#   include "Ant_SyncObj.h"
#endif
#ifndef ANT_TIMERCALLBACK_H
#include "Ant_TimerCallback.h"
#endif
#include "pthread.h"

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Value define
#define WM_NTIMER           (WM_APP + 0x1000)
#define NTIMER_MAX_NUM      (256)

/*---------------------------------------------------------------------------*/
// Class declaration
class Ant_TimerManager;
class Ant_Timer;

/**
 * @brief class represents a timer request.
 *
 * @detail a timer request is created when a timer runs out.
 */
class Ant_TimerRequest : public virtual Ant_Object
{
public:

    struct timespec timeout_tick;

    VOID*           pthread;

    Ant_Timer*      timer;

    Ant_TimerRequest() {
        memset(&timeout_tick, 0,sizeof(timeout_tick));
        pthread = NULL;
        timer = NULL;
    }

    ~Ant_TimerRequest() {}

    BOOL operator == (const Ant_TimerRequest& req) const;

    BOOL operator <  (const Ant_TimerRequest& req) const;

    BOOL operator <= (const Ant_TimerRequest& req) const;

    BOOL operator >  (const Ant_TimerRequest& req) const;

    BOOL operator >= (const Ant_TimerRequest& req) const;

private:

    // Disable the copy constructor and operator =
    Ant_TimerRequest(const Ant_TimerRequest& src);
    Ant_TimerRequest& operator = (const Ant_TimerRequest& src);

};

/**
 * @brief class helps us manages timers in current process.
 *
 * @detail manages all timers in current process, and it's responsible of
 *         posting timer request when a timer runs out.
 */
class Ant_TimerManager : public Ant_SyncObj
{
public:

    Ant_TimerManager();
    virtual ~Ant_TimerManager();

    BOOL registerTimer(Ant_Timer* timer);

    VOID removeTimer(Ant_Timer* timer);

    BOOL isValid(INT id);

    VOID setInvalid(INT id);

    virtual BOOL requestSanity();

    VOID notify();

    VOID setTimerCallback(Ant_TimerCallback * cTimerCB);

protected:

    VOID addRequest(Ant_TimerRequest* new_req);

    VOID removeRequest(DWORD id);

    DWORD nextId();

    virtual void setThreadName(XCHAR* thread_name);

    virtual VOID run();

    static VOID* threadProc(VOID* p);

    inline BOOL checkQuit() {return (m_bQuitflg);}

    BYTE                        timer_table[NTIMER_MAX_NUM];
    Ant_List<Ant_TimerRequest>  m_cReqList;
    pthread_t                   m_tid;
    DWORD                       m_dwThreadID;
    pthread_attr_t              m_attr;
    pthread_mutex_t             m_mutex;
    pthread_cond_t              m_cond;
#if defined(_LINUX)
    pthread_condattr_t          m_condattr;
#endif
    DWORD                       m_dNextid;
    BOOL                        need_sanity_reply;
    BOOL                        m_bQuitflg;
    BOOL                        m_bSignalflg;
    Ant_TimerCallback*          m_ptimercallback;

private:

    // Disable the copy constructor and operator =
    Ant_TimerManager(const Ant_TimerManager& src);
    Ant_TimerManager& operator = (const Ant_TimerManager& src);

};

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif  // ANT_TIMERMANAGER_UNIX_H

/*---------------------------------------------------------------------------*/
/* EOF */
