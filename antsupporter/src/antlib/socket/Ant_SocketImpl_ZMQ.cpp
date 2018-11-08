/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#include "stdafx.h"

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_SOCKETIMPL_ZMQ_H
#   include "Ant_SocketImpl_ZMQ.h"
#endif
#ifndef ANT_SOCKETLOG_H
#   include "Ant_SocketLog.h"
#endif
#ifndef ANT_SOCKETCONTEXT_H
#   include "Ant_SocketContext.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Functions
Ant_SocketImpl_ZMQ::DataBlock::DataBlock()
: Ant_AbstractDataBlock()
, zmq_msg_t()
{
    zmq_msg_init(this);
}

Ant_SocketImpl_ZMQ::DataBlock::~DataBlock()
{
    zmq_msg_close(this);
}

size_t Ant_SocketImpl_ZMQ::DataBlock::getLength()
{
    return zmq_msg_size(this);
}

const void* Ant_SocketImpl_ZMQ::DataBlock::getData()
{
    return zmq_msg_data(this);
}

Ant_SocketImpl_ZMQ::Ant_SocketImpl_ZMQ(SocketType type)
: Ant_AbstractSocket()
, m_pSocket(NULL)
{
    INT realtype = ZMQ_PAIR;
    switch (type) {
    case SOCKTYPE_PUB:
        realtype = ZMQ_PUB;
        break;
    case SOCKTYPE_SUB:
        realtype = ZMQ_SUB;
        break;
    case SOCKTYPE_DEALER:
        realtype = ZMQ_DEALER;
        break;
    case SOCKTYPE_ROUTER:
        realtype = ZMQ_ROUTER;
        break;
    default:
    case SOCKTYPE_PAIR:
        realtype = ZMQ_PAIR;
        break;
    }

    void* context = Ant_SocketContext::context();
    if (NULL != context) {
        m_pSocket = zmq_socket(context, realtype);
    }
}

Ant_SocketImpl_ZMQ::~Ant_SocketImpl_ZMQ()
{
    if (NULL != m_pSocket) {
        zmq_close(m_pSocket);
        m_pSocket = NULL;
    }
}

VOID* Ant_SocketImpl_ZMQ::getSocket()
{
    return m_pSocket;
}

void Ant_SocketImpl_ZMQ::bind(const char* addr)
{
    if (NULL != m_pSocket) {
        zmq_bind(m_pSocket, addr);
    }
}

void Ant_SocketImpl_ZMQ::connect(const char* addr)
{
    if (NULL != m_pSocket) {
        zmq_connect(m_pSocket, addr);
    }
}

BOOL Ant_SocketImpl_ZMQ::send(const char* data, size_t size, uint32_t options)
{
    if (NULL == m_pSocket) {
        return FALSE;
    }

    int realoptions = 0;
    if (options & OPTION_MORE_BLOCK) {
        realoptions |= ZMQ_SNDMORE;
    }
    if (options & OPTION_NO_WAIT) {
        realoptions |= ZMQ_DONTWAIT;
    }

    const int rc = zmq_send(m_pSocket, data, size, realoptions);
    if (static_cast<size_t>(rc) != size) {
        return FALSE;
    }

    return TRUE;
}

BOOL Ant_SocketImpl_ZMQ::recv(Ant_AbstractDataBlock** block, uint32_t options)
{
    if (NULL == block) {
        return FALSE;
    }

    *block = NULL;

    void* s = getSocket();
    if (NULL == s) {
        return FALSE;
    }

    DataBlock * data = new DataBlock();
    if (NULL == data) {
        return FALSE;
    }

    int flag = 0;
    if (options & OPTION_NO_WAIT) {
        flag |= ZMQ_DONTWAIT;
    }

    int rc = zmq_msg_recv(data, s, flag);
    if (rc < 0) {
        *block = NULL;
        delete data;
        return FALSE;
    }

    *block = data;
    return TRUE;
}

BOOL Ant_SocketImpl_ZMQ::hasMoreData()
{
    void* s = getSocket();
    if (NULL == s) {
        return FALSE;
    }

    int64_t more = 0;
    size_t more_size = sizeof(more);
    int rc = zmq_getsockopt(s, ZMQ_RCVMORE, &more, &more_size);
    if (rc != 0) {
        return FALSE;
    }

    return more != 0 ? TRUE : FALSE;
}

void
Ant_SocketImpl_ZMQ::subscribe(const char* msg)
{
    void* s = getSocket();
    if (NULL != s && NULL != msg) {
        zmq_setsockopt(s, ZMQ_SUBSCRIBE, msg, strlen(msg));
    }
}

void
Ant_SocketImpl_ZMQ::unSubscribe(const char* msg)
{
    void* s = getSocket();
    if (NULL != s && NULL != msg) {
        zmq_setsockopt(s, ZMQ_UNSUBSCRIBE, msg, strlen(msg));
    }
}

BOOL Ant_SocketImpl_ZMQ::isCapableOf(SocketCapability capability)
{
    switch (capability) {
    case CAPABILITY_EMPTY_BLOCK:
        return TRUE;

    default:
        return FALSE;
    }
}

BOOL Ant_SocketImpl_ZMQ::monitor(const XCHAR* addr, uint32_t events)
{
    void *s = getSocket();
    if (NULL == s || NULL == addr || 0 == xcslen(addr)) {
        return FALSE;
    }

    int realevents = 0;
    if (events & kEventConnected) {
        realevents |= ZMQ_EVENT_CONNECTED;
    }
    if (events & kEventConnectDelayed) {
        realevents |= ZMQ_EVENT_CONNECT_DELAYED;
    }
    if (events & kEventConnectRetried) {
        realevents |= ZMQ_EVENT_CONNECT_RETRIED;
    }
    if (events & kEventListening) {
        realevents |= ZMQ_EVENT_LISTENING;
    }
    if (events & kEventBindFailed) {
        realevents |= ZMQ_EVENT_BIND_FAILED;
    }
    if (events & kEventAccepted) {
        realevents |= ZMQ_EVENT_ACCEPTED;
    }
    if (events & kEventAcceptFailed) {
        realevents |= ZMQ_EVENT_ACCEPT_FAILED;
    }
    if (events & kEventClosed) {
        realevents |= ZMQ_EVENT_CLOSED;
    }
    if (events & kEventCloseFailed) {
        realevents |= ZMQ_EVENT_CLOSE_FAILED;
    }
    if (events & kEventDisconnected) {
        realevents |= ZMQ_EVENT_DISCONNECTED;
    }

    int rc = zmq_socket_monitor(s, addr, realevents);

    return rc < 0 ? FALSE : TRUE;
}

BOOL Ant_SocketImpl_ZMQ::recvMonitorInfo(MonitorInfo* info, uint32_t options)
{
    if (NULL == info) {
        return FALSE;
    }

    Ant_AbstractDataBlock * block = NULL;
    if (recv(&block, options) && NULL != block) {
        const zmq_event_t* event = reinterpret_cast<const zmq_event_t*>(block->getData());
        if (NULL == event) {
            return FALSE;
        }

        switch (event->event) {
        case ZMQ_EVENT_CONNECTED:
            info->event = kEventConnected;
            break;
        case ZMQ_EVENT_CONNECT_DELAYED:
            info->event = kEventConnectDelayed;
            break;
        case ZMQ_EVENT_CONNECT_RETRIED:
            info->event = kEventConnectRetried;
            break;
        case ZMQ_EVENT_LISTENING:
            info->event = kEventListening;
            break;
        case ZMQ_EVENT_BIND_FAILED:
            info->event = kEventBindFailed;
            break;
        case ZMQ_EVENT_ACCEPTED:
            info->event = kEventAccepted;
            break;
        case ZMQ_EVENT_ACCEPT_FAILED:
            info->event = kEventAcceptFailed;
            break;
        case ZMQ_EVENT_CLOSED:
            info->event = kEventClosed;
            break;
        case ZMQ_EVENT_CLOSE_FAILED:
            info->event = kEventCloseFailed;
            break;
        case ZMQ_EVENT_DISCONNECTED:
            info->event = kEventDisconnected;
            break;
        default:
            // unknown event
            return FALSE;
        }

        info->fd = event->data.disconnected.fd;
        return TRUE;
    }

    return FALSE;
}

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
/* EOF */
