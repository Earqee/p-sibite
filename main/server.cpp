#include "../util/header.h"
#include "../util/log.h"
#include "../socket/socketdata.h"
#include "../socket/serversocket.h"

#include <deque>

class Server {

private:

    int connectionStatus = INVALID;
    ServerSocket socket = ServerSocket(PORT);
    std::deque<SocketData> clientsData;
    //std::set<std::thread> clientsTransmissionThreads;
    //std::set<std::thread> clientsReceivingThreads;

    bool ListenConnection() {
        Log log("Listening connections.");

        connectionStatus = listen(socket.refSocketFD(), maxNumOfConnections);

        if(connectionStatus == INVALID) {
            log.logCannot();
            if(errno == EBADF)
                log.logError("The argument socket is not a valid file descriptor.");
            if(errno == ENOTSOCK)
                log.logError("The argument socket is not a socket.");
            if(errno == EOPNOTSUPP)
                log.logError("The socket socket does not support this operation.");
            return false;
        }
        return true;
    }

    bool AcceptConnections() {
        Log log("Accepting connections.");

        if(connectionStatus == INVALID)
            return false;

        while(true) {

            SocketData clientData;

            while(clientData.refSocketFD() == INVALID) {

                clientData.refSocketFD() = accept(socket.refSocketFD(), (sockaddr*) &clientData.refSocketAddress(), &clientData.refSocketAddressLen());

                if(clientData.refSocketFD() == INVALID) {
                    log.logCannot();
                    if(errno == EBADF)
                        log.logError("The socket argument is not a valid file descriptor.");
                    if(errno == ENOTSOCK)
                        log.logError("The descriptor socket argument is not a socket.");
                    if(errno == EOPNOTSUPP)
                        log.logError("The descriptor socket does not support this operation.");
                    if(errno == EWOULDBLOCK)
                        log.logError("socket has nonblocking mode set, and there are no pending connections immediately available.");
                    return false;
                }

            }

            printf("New connection at %d.\n", ntohs(clientData.refSocketAddress().sin6_port));
            clientsData.push_back(SocketData(clientData));
        }
        return true;
    }

    bool TransmitData(SocketData &clientData, const char *data) {
        return socket.TransmitData(clientData, data);
    }

    bool ReceiveData(SocketData &clientData, int dataSize) {
        return socket.ReceiveData(clientData, dataSize);
    }

public:
    Server() {

        ListenConnection();

        std::thread acceptConnectionThread(&Server::AcceptConnections, this);
        acceptConnectionThread.detach();

        while(true) {

            for(SocketData client : clientsData) {
                //clientsTransmissionThreads.push_back(
                    std::thread newTransmission(&Server::TransmitData, this, std::ref(client), "B");
                    newTransmission.join();

                //);
                //clientsTransmissionThreads.back().join();

                //clientsReceivingThreads.push_back(
                    std::thread newReceiving(&Server::ReceiveData, this, std::ref(client), CHAR_SIZE*sizeof(char));
                    newReceiving.join();
                //);
                //clientsReceivingThreads.back().join();
            }

        }
    }

};

int main() {
    Server server;
    return 0;
}

