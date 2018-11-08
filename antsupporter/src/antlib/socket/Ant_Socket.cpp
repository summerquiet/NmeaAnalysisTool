/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#include "stdafx.h"

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_SOCKET_H
#   include "Ant_Socket.h"
#endif
#ifndef ANT_SOCKETLOG_H
#   include "Ant_SocketLog.h"
#endif
#ifndef ANT_SOCKETIMPL_ZMQ_H
#   include "Ant_SocketImpl_ZMQ.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Functions
static Ant_AbstractSocket* createSocket(Ant_Socket::SocketType type, const char* addr)
{
    return new Ant_SocketImpl_ZMQ(type);
}

Ant_Socket::Ant_Socket(SocketType type)
: Ant_AbstractSocket()
, m_type(type)
, m_pcImpl(NULL)
{

}

Ant_Socket::~Ant_Socket()
{
    delete m_pcImpl;
    m_pcImpl = NULL;
}

VOID* Ant_Socket::getSocket()
{
    if (NULL != m_pcImpl) {
        return m_pcImpl->getSocket();
    }

    return NULL;
}

VOID Ant_Socket::bind(const char* addr)
{
    if (NULL == m_pcImpl) {
        m_pcImpl = createSocket(m_type, addr);
    }

    if (NULL != m_pcImpl) {
        return m_pcImpl->bind(addr);
    }
}

VOID Ant_Socket::connect(const char* addr)
{
    if (NULL == m_pcImpl) {
        m_pcImpl = createSocket(m_type, addr);
    }

    if (NULL != m_pcImpl) {
        return m_pcImpl->connect(addr);
    }
}

BOOL Ant_Socket::send(const char* data, size_t size, uint32_t options)
{
    if (NULL != m_pcImpl) {
        return m_pcImpl->send(data, size, options);
    }

    return false;
}

BOOL Ant_Socket::recv(Ant_AbstractDataBlock** block, uint32_t options)
{
    if (NULL != m_pcImpl) {
        return m_pcImpl->recv(block, options);
    }

    return FALSE;
}

BOOL Ant_Socket::hasMoreData()
{
    if (NULL != m_pcImpl) {
        return m_pcImpl->hasMoreData();
    }

    return false;
}

VOID Ant_Socket::subscribe(const char* msg)
{
    ANT_SOCKET_LOGV("(%p) Subscribe \"%s\"", this, msg);
    if (NULL != m_pcImpl) {
        return m_pcImpl->subscribe(msg);
    }
}

VOID Ant_Socket::unSubscribe(const char* msg)
{
    ANT_SOCKET_LOGV("(%p) UnSubscribe \"%s\"", this, msg);
    if (NULL != m_pcImpl) {
        return m_pcImpl->unSubscribe(msg);
    }
}

BOOL Ant_Socket::isCapableOf(SocketCapability capability)
{
    if (NULL != m_pcImpl) {
        return m_pcImpl->isCapableOf(capability);
    }

    return FALSE;
}

BOOL Ant_Socket::monitor(const XCHAR* addr, uint32_t events)
{
    if (NULL != m_pcImpl) {
        return m_pcImpl->monitor(addr, events);
    }

    return FALSE;
}

BOOL Ant_Socket::recvMonitorInfo(MonitorInfo* info, uint32_t options)
{
    if (NULL != m_pcImpl) {
        return m_pcImpl->recvMonitorInfo(info, options);
    }

    return FALSE;
}

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
/* EOF */
