#ifndef ICVPN_SOCKET_DATA_H
#define ICVPN_SOCKET_DATA_H

#include "header.h"

class SocketData {
private:
public:
    int socketFD;
    sockaddr_in6 socketAddress;
    socklen_t socketAddressLen;

    SocketData() {
        socketFD = INVALID;
        socketAddressLen = sizeof(socketAddress);
    }
    ~SocketData() {
    }

    void configSocketAddress(sa_family_t family, in6_addr addr, uint16_t port) {
        socketAddress.sin6_family = family;
        socketAddress.sin6_addr = addr;
        socketAddress.sin6_port = port;
    }
};

#endif
