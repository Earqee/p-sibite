#ifndef ACEVPN_SOCKET_H
#define ACEVPN_SOCKET_H

#include "../util/header.h"
#include "../util/log.h"
#include "socketdata.h"

class Socket {

protected:

    SocketData socketData;

    void CreateSocket(int port) {
        Log log("Creation of socket '"+std::to_string(port)+"'");

        while(!CreateSocket())
            std::this_thread::sleep_for (std::chrono::seconds(1));
        while(!CreateSocketAddress(port))
            std::this_thread::sleep_for (std::chrono::seconds(1));
    }

private:

    bool CreateSocket() {
        Log log("Opening of socket");

        /* socketData.socketFD = socket(AF_INET6, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0); */
        socketData.refSocketFD() = socket(PF_INET6, SOCK_STREAM, IPPROTO_TCP);

        if(socketData.refSocketFD() == INVALID) {
            log.logCannot();
            if(errno == EACCES)
                log.logError("Permission to create a socket of the specified type and/or protocol is denied.");
            if(errno == EAFNOSUPPORT)
                log.logError("The implementation does not support the specified address family.");
            if(errno == EINVAL)
                log.logError("Unknown protocol, or protocol family not available.");
            if(errno == EINVAL)
                log.logError("Invalid flags in type.");
            if(errno == EMFILE)
                log.logError("The per-process limit on the number of open file descriptors has been reached.");
            if(errno == ENFILE)
                log.logError("The system-wide limit on the total number of open files has been reached.");
            if(errno == ENOBUFS || errno == ENOMEM)
                log.logError("Insufficient memory is available.  The socket cannot be created until sufficient resources are freed.");
            if(errno == EPROTONOSUPPORT)
                log.logError("The protocol type or the specified protocol is not supported within this domain.");
            return false;
        }
        return true;
    }

    virtual bool CreateSocketAddress(int port) {
        Log log("Creating of socket address.");
        return false;
    }

    bool CloseSocket(int action) {
        Log log("Closing of socket.");

        if(socketData.refSocketFD() == INVALID)
            return false;

        int socketStatus = shutdown(socketData.refSocketFD(), action);

        if(socketStatus == INVALID) {
            log.logCannot();
            if(errno == EBADF)
                log.logError("sockfd is not a valid file descriptor.");
            if(errno == EINVAL)
                log.logError("An invalid value was specified in how (but see BUGS).");
            if(errno == ENOTCONN)
                log.logError("The specified socket is not connected.");
            if(errno == ENOTSOCK)
                log.logError("The file descriptor sockfd does not refer to a socket.");
            return false;
        }

        socketData.refSocketFD() = INVALID;
        return true;
    }

public:
    /* Construction and destruction */
    Socket() { }

    ~Socket() {
        CloseSocket();
    }

    /* Related to socket-closing */
    bool CloseSocketReception() {
        return CloseSocket(SHUT_RD);
    }

    bool CloseSocketTransmission() {
        return CloseSocket(SHUT_WR);
    }

    bool CloseSocket() {
        return CloseSocket(SHUT_RDWR);
    }

    /* Socket information retrieval */
    int& refSocketFD() {
        return socketData.refSocketFD();
    }

    sockaddr_in6& refSocketAddress() {
        return socketData.refSocketAddress();
    }

    socklen_t& refSocketAddressLen() {
        return socketData.refSocketAddressLen();
    }

    bool TransmitData(const char *data) {
        return TransmitData(this->socketData, data);
    }

    bool TransmitData(SocketData &socketData, const char *data) {
        Log log("Sending data.");

        if(socketData.refSocketFD() == INVALID)
            return false;

        int bytesSent = 0,
            dataSize = sizeof(data);

        while(bytesSent != dataSize) {

            int sendStatus = send(socketData.refSocketFD(), data + bytesSent, dataSize - bytesSent, 0);

            printf("Sent: %d | Total: %d/%d\n", sendStatus
                    , (sendStatus>0?sendStatus:0) + bytesSent, dataSize);

            if(sendStatus == INVALID) {
                log.logCannot();
                if(errno == EBADF)
                    log.logError("The socket argument is not a valid file descriptor.");
                if(errno == EINTR) {
                    log.logError("The operation was interrupted by a signal before any data was sent. See Interrupted Primitives.");
                    continue;
                }
                if(errno == ENOTSOCK)
                    log.logError("The descriptor socket is not a socket.");
                if(errno == EMSGSIZE)
                    log.logError("The socket type requires that the message be sent atomically, but the message is too large for this to be possible.");
                if(errno == EWOULDBLOCK) {
                    log.logError("Nonblocking mode has been set on the socket, and the write operation would block. (Normally send blocks until the operation can be completed.");
                    continue;
                }
                if(errno == ENOBUFS)
                    log.logError("There is not enough internal buffer space available.");
                if(errno == ENOTCONN)
                    log.logError("You never connected this socket.");
                if(errno == EPIPE)
                    log.logError("This socket was connected but the connection is now broken.");
                socketData.refSocketFD() = INVALID;
                return false;
            }
            bytesSent += sendStatus;
        }
        return true;
    }

    bool ReceiveData(std::string &data, int dataSize) {
        return ReceiveData(this->socketData, data, dataSize);
    }

    bool ReceiveData(SocketData &socketData, std::string &data, int dataSize) {
        Log log("Receiving data.");

        if(socketData.refSocketFD() == INVALID)
            return false;

        char dummyData[dataSize];

        int bytesReceived = 0;

        while(bytesReceived != dataSize) {

            int recvStatus = recv(socketData.refSocketFD(), &dummyData + bytesReceived, dataSize - bytesReceived, 0);

            printf("Received: %d | Total: %d/%d\n", recvStatus
                    , (recvStatus>0?recvStatus:0) + bytesReceived, dataSize);

            if(recvStatus == INVALID) {
                if(errno == EBADF)
                    log.logError("The socket argument is not a valid file descriptor.");
                if(errno == EINTR) {
                    log.logError("The operation was interrupted by a signal before any data was sent. See Interrupted Primitives.");
                    continue;
                }
                if(errno == ENOTSOCK)
                    log.logError("The descriptor socket is not a socket.");
                if(errno == EWOULDBLOCK) {
                    log.logError("Nonblocking mode has been set on the socket, and the write operation would block.");
                    continue;
                }
                if(errno == ENOTCONN)
                    log.logError("You never connected this socket.");
                socketData.refSocketFD() = INVALID;
                return false;
            }
            bytesReceived += recvStatus;
        }
        printf("(%d) SENT = {{\n%s\n}}\n", ntohs(socketData.refSocketAddress().sin6_port), dummyData);
        data = dummyData;
        return true;
    }

};
#endif
