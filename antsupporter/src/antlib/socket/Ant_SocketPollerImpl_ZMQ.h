/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_SOCKETPOLLERIMPL_ZMQ_H
#define ANT_SOCKETPOLLERIMPL_ZMQ_H
#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_SOCKETPOLLERIMPL_BASE_H
#   include "Ant_SocketPollerImpl_Base.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Class declare
/**
 * Definition of abstract interface for polling data from socket(s).
 *
 * It only define interfaces, it does not provide implementation, you should
 * sub-class it to implement the interface.
 */
class Ant_SocketPollerImpl_ZMQ : public Ant_SocketPollerImpl_Base
{
public:

    Ant_SocketPollerImpl_ZMQ();
    virtual ~Ant_SocketPollerImpl_ZMQ();

    virtual VOID pollingImpl(PollerItemList & cItemList);

private:

    // Disable the copy constructor and operator =
    Ant_SocketPollerImpl_ZMQ(const Ant_SocketPollerImpl_ZMQ&);
    Ant_SocketPollerImpl_ZMQ& operator = (const Ant_SocketPollerImpl_ZMQ&);
};

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif  // ANT_SOCKETPOLLERIMPL_ZMQ_H

/*---------------------------------------------------------------------------*/
/* EOF */
