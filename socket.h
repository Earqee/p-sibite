#ifndef ACEVPN_SOCKET_H
#define ACEVPN_SOCKET_H

#include "header.h"
#include "log.h"

class Socket {

private:
    SocketData socketData;

    void CreateSocket(int port) {
        Log log("Creation of socket '"+std::to_string(port)+"'");

        while(!CreateSocket());
        while(!CreateSocketAddress(port));
    }

    bool CreateSocket() {
        Log log("Opening of socket");

        /*
        socket.socketFD = socket(AF_INET6, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0);
        */

        socketData.socketFD = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);

        if(socketData.socketFD == INVALID) {
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

        if(socketData.socketFD != INVALID) {

            socketData.configSocketAddress
                (AF_INET6, in6addr_any, htons(port));

            int bindStatus = bind(socketData.socketFD,
                    (sockaddr*) &socketData.socketAddress, socketData.socketAddressLen);

            if(bindStatus == INVALID) {
                log.logCannot();
                if(errno == EBADF)
                    log.logError("The socket argument is not a valid file descriptor.");
                if(errno == ENOTSOCK)
                    log.logError("The descriptor socket is not a socket.");
                if(errno == EADDRNOTAVAIL)
                    log.logError("The specified address is not available on this machine.");
                if(errno == EADDRINUSE)
                    log.logError("Some other socket is already using the specified address.");
                if(errno == EINVAL)
                    log.logError("The socket socket already has an address.");
                if(errno == EACCES)
                    log.logError("You do not have permission to access the requested address.");
                return false;
            }
        }
        return true;
    }

    void CloseSocket(int action) {
        Log log("Closing of socket.");

        if(socketData.socketFD != INVALID) {

            int socketStatus = shutdown(socketData.socketFD, action);

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
            }

            socketData.socketFD = INVALID;
        }
    }

public:

    Socket(int port){
        CreateSocket(port);
    }
    ~Socket() {
        CloseSocket();
    }

    void CloseSocketReception() {
        CloseSocket(SHUT_RD);
    }

    void CloseSocketTransmission() {
        CloseSocket(SHUT_WR);
    }

    void CloseSocket() {
        CloseSocket(SHUT_RDWR);
    }

    int& getSocketFD() {
        return socketData.socketFD;
    }

    sockaddr_in6& getSocketAddress() {
        return socketData.socketAddress;
    }

};

#endif
