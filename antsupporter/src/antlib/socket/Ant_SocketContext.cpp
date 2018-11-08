/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#include "stdafx.h"

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_SOCKETCONTEXT_H
#   include "Ant_SocketContext.h"
#endif
#ifndef ANT_SOCKETLOG_H
#   include "Ant_SocketLog.h"
#endif
#include <stdarg.h>
#include "zmq.h"

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Value define

static void* g_zmqcontext = NULL;

/*---------------------------------------------------------------------------*/
// Functions
Ant_SocketContext::Ant_SocketContext()
{

}

Ant_SocketContext::~Ant_SocketContext()
{

}

void Ant_SocketContext::createContext()
{
    g_zmqcontext = zmq_init(5);
    ANT_SOCKET_LOGI("ZMQ context initialized.");
}

void* Ant_SocketContext::context()
{
    return g_zmqcontext;
}

void Ant_SocketContext::destroyContext()
{
    zmq_term(g_zmqcontext);
    g_zmqcontext = NULL;
    ANT_SOCKET_LOGI("ZMQ context destroyed.");
}

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
/* EOF */
