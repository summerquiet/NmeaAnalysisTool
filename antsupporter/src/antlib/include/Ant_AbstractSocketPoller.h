/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_ABSTRACTSOCKETPOLLER_H
#define ANT_ABSTRACTSOCKETPOLLER_H
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

class Ant_AbstractSocket;

/**
 * Definition of abstract interface for polling data from socket(s).
 *
 * It only define interfaces, it does not provide implementation, you should
 * sub-class it to implement the interface.
 */
class ANTLIB_API Ant_AbstractSocketPoller : public virtual Ant_Object
{
public:

    enum
    {
        ANT_POLL_IN  = 1,
        ANT_POLL_OUT = 2,
        ANT_POLL_ERR = 4
    };

    enum
    {
        ANT_THREADING_SYNC,
        ANT_THREADING_ASYNC
    };

    /**
     * Definition of the interface to handle poller event.
     *
     * It only define interfaces, it does not provide implementation, you should
     * sub-class it to implement the interface.
     */
    class PollerEventHandler : public virtual Ant_Object
    {
    public:

        /**
         * The poller event callback.
         *
         * @param socket[in]    the socket who receives an event.
         * @param type[in]      type of the poller event, may be combonation of
         *                      the following values:
         *                          - ANT_POLL_IN: there's incoming data.
         *                          - ANT_POLL_OUT: ready to send data.
         *                          - ANT_POLL_ERR: error occurred.
         */
        virtual BOOL onEvent(Ant_AbstractSocket* socket, short type) = 0;

    };

    Ant_AbstractSocketPoller();
    virtual ~Ant_AbstractSocketPoller();

    /**
     * Get threading model of the poller.
     *
     * @retval THREADING_SYNC   it's a synchronous poller, [startPolling] will
     *                          block until [stopPolling] is called or on error
     * @retval THREADING_ASYNC  it's a asynchronous poller, [startPolling] will
     *                          return immediately.
     */
    virtual int getThreadingModel() = 0;

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
     *                          - POLL_IN: there's incoming data.
     *                          - POLL_OUT: ready to send data.
     *                          - POLL_ERR: error occurred.
     */
    virtual BOOL addItem(Ant_AbstractSocket* socket, PollerEventHandler* handler, short events) = 0;

    /**
     * Remove an socket from the poller.
     *
     * @note typically, it should be called before [startPolling], or you will
     *       have to call [updatePolling] to make it works.
     *
     * @param socket[in]    the socket that we don't want to poll anymore.
     */
    virtual BOOL rmItem(Ant_AbstractSocket* socket) = 0;

    /**
     * Remove all socket from the poller.
     *
     * @note typically, it should be called before [startPolling], or you will
     *       have to call [updatePolling] to make it works.
     *
     * Remove any existing socket from the poller.
     */
    virtual VOID clearAll() = 0;

    /**
     * Start polling events from sockets.
     *
     * @note typically, you should have called [addItem] to add socket to the
     *       poller.
     */
    virtual BOOL startPolling() = 0;

    /**
     * Stop polling events from sockets.
     *
     * @param async[in]     Whether asnychronous mode shall be activated, in
     *                      such case, [stopPolling] will return immediately.
     *                      The poller may not have been stopped after it
     *                      returns. Otherwise, the poller shall guarantee
     *                      no event callback will be called after it returns.
     */
    virtual BOOL stopPolling(BOOL async) = 0;

    /**
     * Update the sockets being polled.
     *
     * @param async[in]     Whether asnychronous mode shall be activated, in
     *                      such case, [updatePolling] will return immediately.
     *                      The poller may not have been updated after it
     *                      returns. Otherwise, the poller shall guarantee
     *                      the socket set have been updated before it returns.
     */
    virtual BOOL updatePolling(BOOL bAsync) = 0;

private:

    // Disable the copy constructor and operator =
    Ant_AbstractSocketPoller(const Ant_AbstractSocketPoller&);
    Ant_AbstractSocketPoller& operator = (const Ant_AbstractSocketPoller&);

};

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif  // ANT_ABSTRACTSOCKETPOLLER_H

/*---------------------------------------------------------------------------*/
/* EOF */
