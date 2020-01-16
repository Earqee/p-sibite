#ifndef ACEVPN_DUMMY_SOCKET_H
#define ACEVPN_DUMMY_SOCKET_H

#include "../util/header.h"
#include "../util/log.h"
#include "socket.h"

class DummySocket : public Socket {

private:

    bool CreateSocketAddress(int port) override {
        return false;
    }

public:

    DummySocket(int port) : Socket(port) {
        CreateSocket();
    }

};

#endif
