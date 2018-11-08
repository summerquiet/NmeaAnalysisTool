/**
 * Copyright @ 2014 - 2017 Personal
 * All Rights Reserved.
 */

#ifndef ANT_ABSTRACTSOCKET_H
#define ANT_ABSTRACTSOCKET_H
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
#ifndef ANT_ABSTRACTDATABLOCK_H
#   include "Ant_AbstractDataBlock.h"
#endif

/*---------------------------------------------------------------------------*/
// Namespace
namespace antsupporter {

/*---------------------------------------------------------------------------*/
// Class declare

class Ant_AbstractSocketMonitor;

/**
 * Definition of abstract interface to communicate with remote end-point.
 *
 * It only define interfaces, it does not provide implementation, you should
 * sub-class it to implement the interface.
 */
class ANTLIB_API Ant_AbstractSocket : public virtual Ant_Object
{
public:

    enum SocketType
    {
        SOCKTYPE_PUB,
        SOCKTYPE_SUB,
        SOCKTYPE_PAIR,
        SOCKTYPE_DEALER,
        SOCKTYPE_ROUTER
    };

    enum SocketCapability
    {
        CAPABILITY_EMPTY_BLOCK  // send/receive data block of 0-byte size
    };

    enum
    {
        OPTION_MORE_BLOCK = 1,
        OPTION_NO_WAIT = 2
    };

    enum MonitorEvent
    {
        kEventConnected         = 1,
        kEventConnectDelayed    = 2,
        kEventConnectRetried    = 4,
        kEventListening         = 8,
        kEventBindFailed        = 16,
        kEventAccepted          = 32,
        kEventAcceptFailed      = 64,
        kEventClosed            = 128,
        kEventCloseFailed       = 256,
        kEventDisconnected      = 512,
    };

    struct MonitorInfo
    {
        uint32_t event;
        int fd;
    };

    Ant_AbstractSocket();
    virtual ~Ant_AbstractSocket();

    /**
     * Get the underlying socket object.
     */
    virtual VOID* getSocket() = 0;

    /**
     * Bind the socket at specific address, so we could accept incoming
     * connections from the address.
     *
     * @param addr[in]  the address to bind at.
     */
    virtual VOID bind(const XCHAR* addr) = 0;

    /**
     * Connect to a remote address. The remote side shall [Bind] on the same
     * address, so we could connect to it.
     *
     * @param addr[in]  the address to connect to.
     */
    virtual VOID connect(const XCHAR* addr) = 0;

    /**
     * Send data to the remote side.
     *
     * @param data[in]  pointer to the data buffer.
     * @param size[in]  size of the data to be sent, in bytes.
     * @param options[in] combination of the following values could be used:
     *                      - OPTION_MORE_BLOCK: there's more data for current data frame.
     *                      - OPTION_NO_WAIT: do not block
     */
    virtual BOOL send(const char* data, size_t size, uint32_t options = 0) = 0;

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
    virtual BOOL recv(Ant_AbstractDataBlock** block, uint32_t options = 0) = 0;

    /**
     * Check if there is more data blocks in current data frame.
     *
     * @retval TRUE there're more data blocks to be received.
     * @retval FALS there's no more data blocks in current data frame.
     */
    virtual BOOL hasMoreData() = 0;

    /**
     * Establish a new message filter on a SOCKTYPE_SUB socket. Newly created
     * SOCKTYPE_SUB sockets shall filter out all incoming messages, therefore
     * you should call this option to establish an initial message filter.
     *
     * @param msg[in]   If it's empty, all incoming messages will be accepted.
     *                  If it's non-empty, all messages prefixed with [msg]
     *                  will be accepted.
     */
    virtual VOID subscribe(const XCHAR* msg) = 0;

    /**
     * Remove an existing message filter on a SOCKTYPE_SUB socket.
     *
     * @param msg[in]   The filter to be removed. It must match an existing
     *                  filter previously established with [Subscribe]. If the
     *                  socket has several instances of the same filter
     *                  attached it will remove only one instance, leaving the
     *                  rest in place and functional.
     */
    virtual VOID unSubscribe(const XCHAR* msg) = 0;

    /**
     * Test the socket's capability.
     *
     * @param capability[in] the capability to be tested, could be one of the
     *                       following values.
     *
     * @retval TRUE the socket is capable of doing such operation.
     * @retval FALSE the socket is NOT capable of doing such operation.
     */
    virtual BOOL isCapableOf(SocketCapability capability) = 0;

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
    virtual BOOL monitor(const XCHAR* addr, uint32_t events) = 0;

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
    virtual BOOL recvMonitorInfo(MonitorInfo* info, uint32_t options = 0) = 0;

private:

    // Disable the copy constructor and operator =
    Ant_AbstractSocket(const Ant_AbstractSocket&);
    Ant_AbstractSocket& operator = (const Ant_AbstractSocket&);

};

/*---------------------------------------------------------------------------*/
// Namespace
}

/*---------------------------------------------------------------------------*/
#endif  // ANT_ABSTRACTSOCKET_H

/*---------------------------------------------------------------------------*/
/* EOF */
