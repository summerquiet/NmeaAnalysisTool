/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_USECOUNTER_H
#define ANT_USECOUNTER_H
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
 *  class Ant_UseCounter
 *
 */
class ANTLIB_API Ant_UseCounter : public virtual Ant_Object
{
public:

    /**
     * Construction.
     */
    Ant_UseCounter();

    /**
     * Destruction.
     */
    virtual ~Ant_UseCounter();

    /**
     * Lock
     *
     * @param msec : The time of locking
     *
     * @return BOOL : TRUE means lock succeed, and FALSE failed.
     */
    BOOL lock(LONG msec = INFINITE);

    /**
     * Unlock.
     */
    VOID unlock();

    /**
     * Reference count increase.
     */
    VOID use();

    /**
     * Reference count decrease.
     */
    VOID unuse();

    /**
     * Get the use state.
     *
     * @return BOOL
     * - TRUE : Using
     * - FALSE : Unused
     */
    inline BOOL isUsed() const {return (use_count != 0) ? TRUE : FALSE;}

protected:

    LONG                use_count;

#if defined(_WIN32) || defined(_WIN32_WCE)
    CRITICAL_SECTION    cs;
#else
    pthread_mutex_t     mutex;
    pthread_mutexattr_t attr;
#endif //_LINUX

private:

    // Disable the copy constructor and operator =
    Ant_UseCounter(const Ant_UseCounter& src);
    Ant_UseCounter& operator = (const Ant_UseCounter& src);

};

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif // ANT_USECOUNTER_H

/*---------------------------------------------------------------------------*/
/* EOF */
