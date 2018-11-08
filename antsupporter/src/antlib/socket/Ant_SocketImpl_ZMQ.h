/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_SOCKETIMPL_ZMQ_H
#define ANT_SOCKETIMPL_ZMQ_H
#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

/*---------------------------------------------------------------------------*/
// Include files

#ifndef ANT_ABSTRACTDATABLOCK_H
#   include "Ant_AbstractDataBlock.h"
#endif
#ifndef ANT_ABSTRACTSOCKET_H
#   include "Ant_AbstractSocket.h"
#endif
#include "zmq.h"

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Class declare
/**
 * Implementation of interface Ant_AbstractSocket.
 *
 * Implement interface Ant_AbstractSocket with ZMQ.
 */
class Ant_SocketImpl_ZMQ : public Ant_AbstractSocket
{
public:

    Ant_SocketImpl_ZMQ(SocketType type);
    virtual ~Ant_SocketImpl_ZMQ();

    /**
     * Get the underlying socket object.
     */
    VOID* getSocket();

    /**
     * Bind the socket at specific address, so we could accept incoming
     * connections from the address.
     *
     * @param addr[in]  the address to bind at.
     */
    virtual VOID bind(const XCHAR* addr);

    /**
     * Connect to a remote address. The remote side shall [Bind] on the same
     * address, so we could connect to it.
     *
     * @param addr[in]  the address to connect to.
     */
    virtual VOID connect(const XCHAR* addr);

    /**
     * Send data to the remote side.
     *
     * @param data[in]  pointer to the data buffer.
     * @param size[in]  size of the data to be sent, in bytes.
     * @param options[in] one of the following values could be used:
     *                      - OPTION_MORE_BLOCK: there's more data for current data frame.
     *                      - OPTION_LAST_BLOCK: this is the last block of a data frame.
     */
    virtual BOOL send(const char* data, size_t size, uint32_t options = 0);

    /**
     * Receive a data block from the remote side.
     *
     * @param block[out]    the received data block. if the return value is not
     *                      NULL, it's the caller's responsibility to delete it.
     * @param options[in]   combination of the following values:
     *                          - OPTION_NO_WAIT: do not block
     *
     * @retval TRUE a data block is successfully received.
     * @retval FALSE no data block received.
     */
    virtual BOOL recv(Ant_AbstractDataBlock** block, uint32_t options = 0);

    /**
     * Check if there is more data blocks in current data frame.
     *
     * @retval TRUE there're more data blocks to be received.
     * @retval FALS there's no more data blocks in current data frame.
     */
    virtual BOOL hasMoreData();

    /**
     * Establish a new message filter on a SOCKTYPE_SUB socket. Newly created
     * SOCKTYPE_SUB sockets shall filter out all incoming messages, therefore
     * you should call this option to establish an initial message filter.
     *
     * @param msg[in]   If it's empty, all incoming messages will be accepted.
     *                  If it's non-empty, all messages prefixed with [msg]
     *                  will be accepted.
     */
    virtual VOID subscribe(const XCHAR* msg);

    /**
     * Remove an existing message filter on a SOCKTYPE_SUB socket.
     *
     * @param msg[in]   The filter to be removed. It must match an existing
     *                  filter previously established with [Subscribe]. If the
     *                  socket has several instances of the same filter
     *                  attached it will remove only one instance, leaving the
     *                  rest in place and functional.
     */
    virtual VOID unSubscribe(const XCHAR* msg);

    /**
     * Test the socket's capability.
     *
     * @param capability[in] the capability to be tested, could be one of the
     *                       following values.
     *
     * @retval TRUE the socket is capable of doing such operation.
     * @retval FALSE the socket is NOT capable of doing such operation.
     */
    virtual BOOL isCapableOf(SocketCapability capability);

    /**
     * Start monitoring socket events and publish events to a specific address.
     *
     * @note for each socket, it should be called only once. The behavior is
     *       undefined if multiple call to this function is made.
     *
     * @param addr[in]      the address to publish socket events.
     * @param events[in]    the events to be monitored, could be combination
     *                      of values in [MonitorEvent].
     *
     * @retval TRUE successfully started publishing events to the address.
     * @retval FALS failed to start publishing events to the address.
     */
    virtual BOOL monitor(const XCHAR* addr, uint32_t events);

    /**
     * Get a socket event (like connects) from the socket monitor.
     *
     * @note it works correctly only if it's connected to the address passed
     *       to a successful [Monitor] call.
     *
     * @param info[out]     the socket event if successful.
     * @param options[in]   combination of the following values:
     *                      - Ant_AbstractSocket::OPTION_NO_WAIT: do not block
     *
     * @retval TRUE - a socket event is returned.
     * @retval FALSE - nothing is returned.
     */
    virtual BOOL recvMonitorInfo(MonitorInfo* info, uint32_t options = 0);

protected:

    void* m_pSocket;

private:

    /**
     * Implementation of interface Ant_AbstractDataBlock.
     *
     * Implementation of interface Ant_AbstractDataBlock based on zmq_msg_t.
     */
    class DataBlock : public Ant_AbstractDataBlock, public zmq_msg_t
    {
    public:

        DataBlock();
        virtual ~DataBlock();

        /**
         * Get size of the data block.
         *
         * @retval size of the data block in bytes.
         */
        size_t getLength();

        /**
         * Get pointer to raw data of the data block.
         *
         * @retval pointer to the data of the data block.
         */
        const void* getData();
    };

private:

    // Disable the copy constructor and operator =
    Ant_SocketImpl_ZMQ(const Ant_SocketImpl_ZMQ&);
    Ant_SocketImpl_ZMQ& operator = (const Ant_SocketImpl_ZMQ&);

};

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif  // ANT_SOCKETIMPL_ZMQ_H

/*---------------------------------------------------------------------------*/
/* EOF */
