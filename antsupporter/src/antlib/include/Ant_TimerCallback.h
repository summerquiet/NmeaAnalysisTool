/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_TIMERCALLBACK_H
#define ANT_TIMERCALLBACK_H
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

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Class declare

/**
 * Ant_TimerCallback is base calss of Timer Callback interface.
 *
 * User could implement the function Callback
 * to deal with the timermanager request for the timer.
 */
class Ant_TimerCallback : public virtual Ant_Object
{
public:

    Ant_TimerCallback() {}
    virtual ~Ant_TimerCallback() {}

    virtual VOID callback(LPVOID ptr) = 0;

private:

    // Disable the copy constructor and operator =
    Ant_TimerCallback(const Ant_TimerCallback& src);
    Ant_TimerCallback& operator = (const Ant_TimerCallback& src);

};

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif  // ANT_TIMERCALLBACK_H

/*---------------------------------------------------------------------------*/
/* EOF */
