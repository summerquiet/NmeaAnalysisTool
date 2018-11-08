/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_WAITOBJ_H
#define ANT_WAITOBJ_H
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
#if !defined(_WIN32) && !defined(_WIN32_WCE)
#   include <pthread.h>
#endif //_LINUX

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Class declare

/**
 *  class Ant_WaitObj
 *
 */
class ANTLIB_API Ant_WaitObj : public virtual Ant_Object
{
public:

    /**
     * Construction.
     *
     * @param manual
     * - TRUE : Creates a manual-reset event object
     * - FALSE : Creates an auto-reset event object
     */
    explicit Ant_WaitObj(BOOL manual = FALSE);

    /**
     * Destruction.
     */
    virtual ~Ant_WaitObj();

    /**
     * Wait for single objects
     *
     * @param msec : Time-out interval, in milliseconds.
     *
     * @return BOOL : TRUE means succeed, and FALSE failed.
     */
    BOOL wait(DWORD msec = INFINITE);

    /**
     * Set the specified event object to the signaled state.
     */
    VOID notify();

#if defined(_WIN32) || defined(_WIN32_WCE)
    /**
     * Get the info of notifying
     *
     * @return INT : The info of notifying
     */
    inline HANDLE getHandle() const {return hEvent;}
#endif //_LINUX

    /**
     * Set the specified event object to the nonsignaled state
     */
    VOID reset();

protected:

#if defined(_WIN32) || defined(_WIN32_WCE)
    HANDLE              hEvent;
#else
    pthread_mutex_t     mutex;
    pthread_cond_t      cond;
    BOOL                signal_flag;
    BOOL                manual_flag;
#if defined(_LINUX)
    pthread_condattr_t  condattr;
#endif
#endif //_LINUX

private:

    // Disable the copy constructor and operatpr =
    Ant_WaitObj(const Ant_WaitObj& src);
    Ant_WaitObj& operator = (const Ant_WaitObj& src);

};

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif // ANT_WAITOBJ_H

/*---------------------------------------------------------------------------*/
/* EOF */
