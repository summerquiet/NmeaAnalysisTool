/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_SOCKETPOLLERIMPL_BASE_H
#define ANT_SOCKETPOLLERIMPL_BASE_H
#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_ABSTRACTSOCKETPOLLER_H
#   include "Ant_AbstractSocketPoller.h"
#endif
#ifndef ANT_SYNCOBJ_H
#   include "Ant_SyncObj.h"
#endif
#ifndef ANT_WAITOBJ_H
#   include "Ant_WaitObj.h"
#endif
#include <vector>

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Class declare

class Ant_Socket;

/**
* @description of Ant_SocketPollerImpl_Base
*
* @details definition and interface for Ant_SocketPollerImpl_Base
*/
class Ant_SocketPollerImpl_Base: public Ant_AbstractSocketPoller, public Ant_AbstractSocketPoller::PollerEventHandler
{
public:

    static const XCHAR* COMMAND_STOP;
    static const XCHAR* COMMAND_UPDATE;

#if !defined(_WIN32)
    using Ant_AbstractSocketPoller::PollerEventHandler;
#endif

    /**
     * Get threading model of the poller.
     *
     * @retval THREADING_SYNC   it's a synchronous poller, [startPolling] will
     *                          block until [stopPolling] is called or on error
     * @retval THREADING_ASYNC  it's a asynchronous poller, [startPolling] will
     *                          return immediately.
     */
    virtual int getThreadingModel();

    /**
     * Add a socket to be polled.
     *
     * @note typically, it should be called before [startPolling], or you will
     *       have to call [updatePolling] to make it works.
     *
     * @param socket[in]    the socket to be polled.
     * @param handler[in]   callback to be called when event occurred.
     * @param events[in]    events to be polled, may be combination of the
     *                      following values:
     *                          - ANT_POLL_IN: there's incoming data.
     *                          - ANT_POLL_OUT: ready to send data.
     *                          - ANT_POLL_ERR: error occurred.
     */
    virtual BOOL addItem(Ant_AbstractSocket* socket, PollerEventHandler* handler, short events);

    /**
     * Remove an socket from the poller.
     *
     * @note typically, it should be called before [startPolling], or you will
     *       have to call [updatePolling] to make it works.
     *
     * @param socket[in]    the socket that we don't want to poll anymore.
     */
    virtual BOOL rmItem(Ant_AbstractSocket* socket);

    /**
     * Remove all socket from the poller.
     *
     * @note typically, it should be called before [startPolling], or you will
     *       have to call [updatePolling] to make it works.
     *
     * Remove any existing socket from the poller.
     */
    virtual VOID clearAll();

    /**
     * Start polling events from sockets.
     *
     * @note typically, you should have called [addItem] to add socket to the
     *       poller.
     */
    virtual BOOL startPolling();

    /**
     * Stop polling events from sockets.
     *
     * @param async[in]     Whether asnychronous mode shall be activated, in
     *                      such case, [stopPolling] will return immediately.
     *                      The poller may not have been stopped after it
     *                      returns. Otherwise, the poller shall guarantee
     *                      no event callback will be called after it returns.
     */
    virtual BOOL stopPolling(BOOL async);

    /**
     * Update the sockets being polled.
     *
     * @param async[in]     Whether asnychronous mode shall be activated, in
     *                      such case, [updatePolling] will return immediately.
     *                      The poller may not have been updated after it
     *                      returns. Otherwise, the poller shall guarantee
     *                      the socket set have been updated before it returns.
     */
    virtual BOOL updatePolling(BOOL bAsync);

protected:

    enum POLLERCOMMAND
    {
        POLLERCOMMAND_NULL,
        POLLERCOMMAND_KEEPRUNNING,
        POLLERCOMMAND_UPDATE,
        POLLERCOMMAND_STOP,
    };

    struct PollerItem
    {
        Ant_AbstractSocket* socket;
        PollerEventHandler* handler;
        short               events;
    };

    typedef std::vector<PollerItem> PollerItemList;

    Ant_Socket*             m_psocket_pub;
    Ant_Socket*             m_psocket_sub;
    POLLERCOMMAND           m_ePollerCmd;

    PollerItemList          m_cItemList;

    Ant_SyncObj             m_cSyncObjItemList;
    Ant_SyncObj             m_cSyncObjCommand;
    Ant_WaitObj             m_cWaitObj;

    volatile BOOL           m_bStartFlag;

    Ant_SocketPollerImpl_Base();
    virtual ~Ant_SocketPollerImpl_Base();

    virtual BOOL onEvent(Ant_AbstractSocket* socket, short evType);
    virtual VOID pollingImpl(PollerItemList& cItemList);

private:

    // Disable the copy constructor and operator =
    Ant_SocketPollerImpl_Base(const Ant_SocketPollerImpl_Base&);
    Ant_SocketPollerImpl_Base& operator = (const Ant_SocketPollerImpl_Base&);

};

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif  // ANT_SOCKETPOLLERIMPL_BASE_H

/*---------------------------------------------------------------------------*/
/* EOF */
