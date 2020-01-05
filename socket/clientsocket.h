#ifndef ACEVPN_LOCAL_SOCKET_H
#define ACEVPN_LOCAL_SOCKET_H

#include "../util/header.h"
#include "../util/log.h"
#include "socket.h"

class ClientSocket : public Socket {

private:

    bool CreateSocketAddress(int port) override {
        Log log("Creating of socket address.");

        if(socketData.refSocketFD() == INVALID)
            return false;

        in6_addr ipv6ServerAddress;
        int conversionStatus = inet_pton(AF_INET6, "::1", &ipv6ServerAddress);

        if(conversionStatus == INVALID) {
            log.logCannot();
            if(errno == EAFNOSUPPORT)
                log.logError("Valid network address not provided.");
            return false;
        }

        socketData.configSocketAddress(AF_INET6, ipv6ServerAddress, htons(port));

        return true;
    }

public:

    ClientSocket(int port) {
        CreateSocket(port);
    }

};

#endif
