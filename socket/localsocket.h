#ifndef ACEVPN_LOCAL_SOCKET_H
#define ACEVPN_LOCAL_SOCKET_H

#include "../util/header.h"
#include "../util/log.h"

class Socket {

private:
    SocketData socketData;

    void CreateSocket(int port) {
        Log log("Creation of socket '"+std::to_string(port)+"'");

        while(!CreateSocket())
            std::this_thread::sleep_for (std::chrono::seconds(1));
        while(!CreateSocketAddress(port))
            std::this_thread::sleep_for (std::chrono::seconds(1));
    }

    bool CreateSocket() {
        Log log("Opening of socket");

        /* socketData.socketFD = socket(AF_INET6, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0); */
        socketData.refSocketFD() = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);

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

    bool CreateSocketAddress(int port) {
        Log log("Creating of socket address.");

        if(socketData.refSocketFD() == INVALID)
            return false;

        socketData.configSocketAddress(AF_INET6, in6addr_loopback, htons(port));
        return true;
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
    Socket(int port){ CreateSocket(port); }
    ~Socket() { CloseSocket(); }
    /* Related to socket-closing */
    bool CloseSocketReception() { return CloseSocket(SHUT_RD); }
    bool CloseSocketTransmission() { return CloseSocket(SHUT_WR); }
    bool CloseSocket() { return CloseSocket(SHUT_RDWR); }
    /* Socket information retrieval */
    int& refSocketFD() { return socketData.refSocketFD(); }
    sockaddr_in6& refSocketAddress() { return socketData.refSocketAddress(); }
    socklen_t& refSocketAddressLen() { return socketData.refSocketAddressLen(); }
};

#endif
