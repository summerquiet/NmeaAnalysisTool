/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_SOCKETPOLLER_H
#define ANT_SOCKETPOLLER_H
#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_ABSTRACTSOCKETPOLLER_H
#   include "Ant_AbstractSocketPoller.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Class declare

/**
 * Implementation of abstract interface for polling data from socket(s).
 *
 * The underlying socket-system will be determined at compile time (ZMQ).
 */
class ANTLIB_API Ant_SocketPoller : public Ant_AbstractSocketPoller
{
public:

    Ant_SocketPoller();
    virtual ~Ant_SocketPoller();

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

private:

    Ant_AbstractSocketPoller*   m_pcPollerImpl;

private:

    // Disable the copy constructor and operator =
    Ant_SocketPoller(const Ant_SocketPoller&);
    Ant_SocketPoller& operator = (const Ant_SocketPoller&);

};

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif  // ANT_SOCKETPOLLER_H

/*---------------------------------------------------------------------------*/
/* EOF */
