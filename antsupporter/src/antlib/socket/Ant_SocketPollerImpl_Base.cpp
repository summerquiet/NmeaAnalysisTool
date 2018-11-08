/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#include "stdafx.h"

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_SOCKETPOLLERIMPL_BASE_H
#   include "Ant_SocketPollerImpl_Base.h"
#endif
#ifndef ANT_SOCKET_H
#   include "Ant_Socket.h"
#endif
#ifndef ANT_SOCKETLOG_H
#   include "Ant_SocketLog.h"
#endif
#ifndef ANT_AUTOSYNC_H
#   include "Ant_AutoSync.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Value define
const XCHAR * Ant_SocketPollerImpl_Base::COMMAND_STOP     = XTEXT("COMMAND_STOP");
const XCHAR * Ant_SocketPollerImpl_Base::COMMAND_UPDATE   = XTEXT("COMMAND_UPDATE");

/*---------------------------------------------------------------------------*/
// Functions
Ant_SocketPollerImpl_Base::Ant_SocketPollerImpl_Base()
: m_ePollerCmd(POLLERCOMMAND_KEEPRUNNING)
, m_bStartFlag(FALSE)
{
    // initialize pub sub socket
    char socketaddr[MAX_PATH] = { 0 };
    sprintf(socketaddr, "inproc://transit/Ant_PollerImpl_%p", this);

    m_psocket_pub = new Ant_Socket(Ant_Socket::SOCKTYPE_PUB);
    if (NULL != m_psocket_pub) {
        m_psocket_pub->bind(socketaddr);
    }

    m_psocket_sub = new Ant_Socket(Ant_Socket::SOCKTYPE_SUB);
    if (NULL != m_psocket_sub) {
        m_psocket_sub->connect(socketaddr);
        m_psocket_sub->subscribe(COMMAND_STOP);
        m_psocket_sub->subscribe(COMMAND_UPDATE);
    }
}

Ant_SocketPollerImpl_Base::~Ant_SocketPollerImpl_Base()
{
    if (NULL != m_psocket_pub) {
        delete m_psocket_pub;
        m_psocket_pub = NULL;
    }
    if (NULL != m_psocket_sub) {
        delete m_psocket_sub;
        m_psocket_sub = NULL;
    }
}

int Ant_SocketPollerImpl_Base::getThreadingModel()
{
    return ANT_THREADING_SYNC;
}

BOOL Ant_SocketPollerImpl_Base::addItem(Ant_AbstractSocket* socket, PollerEventHandler* handler, short events)
{
    Ant_AutoSync sync(m_cSyncObjItemList);

    if (NULL == socket || NULL == handler || 0 == events) {
        return FALSE;
    }

    PollerItem item;
    item.socket  = socket;
    item.handler = handler;
    item.events  = events;

    m_cItemList.push_back(item);

    return TRUE;
}

BOOL Ant_SocketPollerImpl_Base::rmItem(Ant_AbstractSocket* socket)
{
    Ant_AutoSync sync(m_cSyncObjItemList);

    if (NULL == socket) {
        return FALSE;
    }

    for (PollerItemList::iterator it = m_cItemList.begin(); it != m_cItemList.end(); ++it) {
        PollerItem item = *it;
        if (item.socket == socket) {
            m_cItemList.erase(it);
            break;
        }
    }
    return TRUE;
}

void Ant_SocketPollerImpl_Base::clearAll()
{
    Ant_AutoSync sync(m_cSyncObjItemList);
    m_cItemList.clear();
}


BOOL Ant_SocketPollerImpl_Base::startPolling()
{
    m_bStartFlag = TRUE;
    ANT_SOCKET_LOGV("Socket poller started.");

    do {

        m_ePollerCmd = POLLERCOMMAND_NULL;

        // copy the m_cItemList to local list
        PollerItemList cItemList;
        {
            PollerItem item;
            item.socket   = m_psocket_sub;
            item.handler  = this;
            item.events   = ANT_POLL_IN;
            cItemList.push_back(item);

            Ant_AutoSync sync(m_cSyncObjItemList);

            std::copy(m_cItemList.begin(), m_cItemList.end(), std::back_inserter(cItemList));
        }

        // notify update
        m_cWaitObj.notify(); 

        pollingImpl(cItemList);

    } while (POLLERCOMMAND_UPDATE == m_ePollerCmd);

    m_bStartFlag = FALSE;

    // notify stop
    m_cWaitObj.notify();

    ANT_SOCKET_LOGV("Quiting socket poller.");
    return TRUE;
}

BOOL Ant_SocketPollerImpl_Base::stopPolling(BOOL bAsync)
{
    // check the startflag,
    // for preventing deadlock when call stopPolling but not startpolling before.
    if (!m_bStartFlag) {
        return TRUE;
    }

    // CAUTION: do NOT hold the lock while waiting for the event, it may cause
    //          deadlock if stopPolling/updatePolling is called in [onEvent]
    //          callback.
    {
        Ant_AutoSync sync(m_cSyncObjCommand);
        if (!bAsync) {
            m_cWaitObj.reset();
        }
        m_psocket_pub->send(COMMAND_STOP, xcslen(COMMAND_STOP) * sizeof(XCHAR), Ant_AbstractSocket::OPTION_NO_WAIT);
    }

    if (!bAsync) {
        m_cWaitObj.wait();
    }

    return TRUE;
}

BOOL Ant_SocketPollerImpl_Base::updatePolling(BOOL bAsync)
{
    // check the startflag,
    // for preventing deadlock when call updatePolling but not startpolling before.
    if (!m_bStartFlag) {
        return TRUE;
    }

    // CAUTION: do NOT hold the lock while waiting for the event, it may cause
    //          deadlock if stopPolling/updatePolling is called in [onEvent]
    //          callback.
    {
        Ant_AutoSync sync(m_cSyncObjCommand);
        if (!bAsync) {
            m_cWaitObj.reset();
        }
        m_psocket_pub->send(COMMAND_UPDATE, xcslen(COMMAND_UPDATE) * sizeof(XCHAR), Ant_AbstractSocket::OPTION_NO_WAIT);
    }

    if (!bAsync) {
        m_cWaitObj.wait();
    }

    return TRUE;
}

BOOL Ant_SocketPollerImpl_Base::onEvent(Ant_AbstractSocket* socket, short evType)
{
    // this method only return FALSE,
    // means received poller command, POLLERCOMMAND_STOP or POLLERCOMMAND_UPDATE
    if (0 == (evType & ANT_POLL_IN)) {
        return FALSE;
    }

    Ant_AbstractDataBlock * data = NULL;
    if (!socket->recv(&data, 0) || NULL == data) {
        return FALSE;
    }

    if (0 == xcsncmp(reinterpret_cast<const XCHAR*>(data->getData()), COMMAND_STOP, data->getLength())) {
        m_ePollerCmd = POLLERCOMMAND_STOP;
    }
    else if (0 == xcsncmp(reinterpret_cast<const XCHAR*>(data->getData()), COMMAND_UPDATE, data->getLength())) {
        m_ePollerCmd = POLLERCOMMAND_UPDATE;
    }
    else {
    }
    return FALSE;
}

void Ant_SocketPollerImpl_Base::pollingImpl(PollerItemList& cItemList)
{

}

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
/* EOF */