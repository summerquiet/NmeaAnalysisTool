/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_TIMER_H
#define ANT_TIMER_H
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
#ifndef ANT_TIMERCALLBACK_H
#include "Ant_TimerCallback.h"
#endif
#if !defined(_WIN32) && !defined(_WIN32_WCE)
#   include <pthread.h>
#endif //_LINUX

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Class declaration
class Ant_TimerRequest;
class Ant_TimerManager;

/**
 * @brief class represents a timer.
 *
 * @detail class helps set-up timer.
 */
class ANTLIB_API Ant_Timer : public virtual Ant_Object
{
public:

    friend class Ant_TimerManager;

    explicit Ant_Timer(LONG msec, BOOL iter = TRUE);
    virtual ~Ant_Timer();

    BOOL start();

    VOID stop();

    BOOL restart();

    BOOL restart(LONG tm);

    inline BOOL isActive() const {return m_dTimerid != 0;}

    inline DWORD getId() const {return used_timer_id;}

    VOID doAction();

    static BOOL isValid(INT id);

protected:

    static BOOL initialize(Ant_TimerCallback * cTimerCB = NULL);

    LONG            msec;

    DWORD           m_dTimerid;

    BOOL         m_bIterate;

    DWORD           used_timer_id;

#if defined(_WIN32) || defined(_WIN32_WCE)
    volatile LONG   signal;
#elif defined(_LINUX)
    DWORD           signal;
#else //Andriod iPhone
    sig_atomic_t    signal;
#endif
    Ant_TimerRequest* m_pRequest;

    virtual VOID onTimer();

private:

    static Ant_TimerManager& getTimerManager();

private:

    // Disable the copy constructor and operator =
    Ant_Timer(const Ant_Timer& src);
    Ant_Timer& operator = (const Ant_Timer& src);

};

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif  // ANT_TIMER_H

/*---------------------------------------------------------------------------*/
/* EOF */
