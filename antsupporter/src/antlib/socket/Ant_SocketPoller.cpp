/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#include "stdafx.h"

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_SOCKETPOLLER_H
#   include "Ant_SocketPoller.h"
#endif
#ifndef ANT_SOCKETPOLLERIMPL_ZMQ_H
#   include "Ant_SocketPollerImpl_ZMQ.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Functions
static Ant_AbstractSocketPoller * createPoller()
{
    return new Ant_SocketPollerImpl_ZMQ();
}

Ant_SocketPoller::Ant_SocketPoller()
: m_pcPollerImpl(NULL)
{
    m_pcPollerImpl = createPoller();
}

Ant_SocketPoller::~Ant_SocketPoller()
{
    delete m_pcPollerImpl;
    m_pcPollerImpl = NULL;
}

int Ant_SocketPoller::getThreadingModel()
{
    if (NULL != m_pcPollerImpl) {
        return m_pcPollerImpl->getThreadingModel();
    }

    return ANT_THREADING_SYNC;
}

BOOL Ant_SocketPoller::addItem(Ant_AbstractSocket* socket, PollerEventHandler* handler, short events)
{
    if (NULL != m_pcPollerImpl) {
        return m_pcPollerImpl->addItem(socket, handler, events);
    }

    return false;
}

BOOL Ant_SocketPoller::rmItem(Ant_AbstractSocket* socket)
{
    if (NULL != m_pcPollerImpl) {
        return m_pcPollerImpl->rmItem(socket);
    }

    return false;
}

void Ant_SocketPoller::clearAll()
{
    if (NULL != m_pcPollerImpl) {
        m_pcPollerImpl->clearAll();
    }
}

BOOL Ant_SocketPoller::startPolling()
{
    if (NULL != m_pcPollerImpl) {
        return m_pcPollerImpl->startPolling();
    }

    return false;
}

BOOL Ant_SocketPoller::stopPolling(BOOL bAsync)
{
    if (NULL != m_pcPollerImpl) {
        return m_pcPollerImpl->stopPolling(bAsync);
    }

    return false;
}

BOOL Ant_SocketPoller::updatePolling(BOOL bAsync)
{
    if (NULL != m_pcPollerImpl) {
        return m_pcPollerImpl->updatePolling(bAsync);
    }

    return false;
}

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
/* EOF */
