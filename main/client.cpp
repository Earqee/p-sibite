#include "../util/header.h"
#include "../util/log.h"
#include "../socket/socketdata.h"
#include "../socket/clientsocket.h"

class Client {

private:
    int connectionStatus = INVALID;
    ClientSocket socket = ClientSocket(PORT);

public:
    bool MakeConnection() {
        Log log("Making connection");

        connectionStatus = connect(socket.refSocketFD(),
                (sockaddr*) &socket.refSocketAddress(), socket.refSocketAddressLen());

        if(connectionStatus == INVALID) {
            log.logCannot();
            if(errno == EBADF)
                log.logError("The socket socket is not a valid file descriptor.");
            if(errno == ENOTSOCK)
                log.logError("File descriptor socket is not a socket.");
            if(errno == EADDRNOTAVAIL)
                log.logError("The specified address is not available on the remote machine.");
            if(errno == EAFNOSUPPORT)
                log.logError("The namespace of the addr is not supported by this socket.");
            if(errno == EISCONN)
                log.logError("The socket socket is already connected.");
            if(errno == ETIMEDOUT)
                log.logError("The attempt to establish the connection timed out.");
            if(errno == ECONNREFUSED)
                log.logError("The server has actively refused to establish the connection.");
            if(errno == ENETUNREACH)
                log.logError("The network of the given addr isn’t reachable from this host.");
            if(errno == EADDRINUSE)
                log.logError("The socket address of the given addr is already in use.");
            if(errno == EINPROGRESS)
                log.logError("The socket socket is non-blocking and the connection could not be established immediately.");
            if(errno == EALREADY)
                log.logError("The socket socket is non-blocking and already has a pending connection in progress.");
            return false;
        }
        return true;
    }

    bool TransmitData(const char *data) {
        return socket.TransmitData(data);
    }

    bool ReceiveData(int dataSize) {
        return socket.ReceiveData(dataSize);
    }

public:
    Client() {

        while(!MakeConnection());

        while(true) {

            std::thread newTransmission(&Client::TransmitData, this, "A");
            newTransmission.join();

            std::thread newReceiving(&Client::ReceiveData, this, CHAR_SIZE*sizeof(char));
            newReceiving.join();


            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

};

int main() {
    Client client;
    return 0;
}
