/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#include "stdafx.h"

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_SOCKETPOLLERIMPL_ZMQ_H
#   include "Ant_SocketPollerImpl_ZMQ.h"
#endif
#ifndef ANT_ABSTRACTSOCKET_H
#   include "Ant_AbstractSocket.h"
#endif
#include <zmq.h>

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Functions
Ant_SocketPollerImpl_ZMQ::Ant_SocketPollerImpl_ZMQ()
{

}

Ant_SocketPollerImpl_ZMQ::~Ant_SocketPollerImpl_ZMQ()
{
    stopPolling(false);
}

VOID Ant_SocketPollerImpl_ZMQ::pollingImpl(PollerItemList & cItemList)
{
    int count = cItemList.size();
    zmq_pollitem_t* items = new zmq_pollitem_t[count];
    if (NULL == items) {
        return;
    }

    int index = 0;
    for (PollerItemList::iterator it = cItemList.begin(); it != cItemList.end(); ++it, ++index) {
        items[index].socket = it->socket->getSocket();
        items[index].fd = 0;
        items[index].events = 0;
        items[index].revents = 0;
        if ((it->events) & ANT_POLL_IN) {
            items[index].events |= ZMQ_POLLIN;
        }
        if ((it->events) & ANT_POLL_OUT) {
            items[index].events |= ZMQ_POLLOUT;
        }
        if ((it->events) & ANT_POLL_ERR) {
            items[index].events |= ZMQ_POLLERR;
        }
    }

    bool run_loop = true;
    while (run_loop) {
        int nfds = zmq_poll(items, count, -1);
        if (0 >= nfds) {
            continue;
        }
        for (index = 0; index < count; ++index) {
            int events = 0;
            if ((items[index].revents & ZMQ_POLLIN) == ZMQ_POLLIN) {
                events |= ANT_POLL_IN;
            }
            if ((items[index].revents & ZMQ_POLLOUT) == ZMQ_POLLOUT) {
                events |= ANT_POLL_OUT;
            }
            if ((items[index].revents & ZMQ_POLLERR) == ZMQ_POLLERR) {
                events |= ANT_POLL_ERR;
            }
            items[index].revents = 0;

            if (0 != events) {
                const PollerItem& item = cItemList[index];
                run_loop = item.handler->onEvent(item.socket, events);
                if (!run_loop) {
                    break;
                }
            }
        }
    }
    delete[] items;
    items = NULL;
}

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
/* EOF */
