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

    bool ReceiveData(SocketData &clientData, std::string &data, int dataSize) {
        return socket.ReceiveData(clientData, data, dataSize);
    }

public:
    Server() {

        ListenConnection();

        std::thread acceptConnectionThread(&Server::AcceptConnections, this);
        acceptConnectionThread.detach();

        while(true) {

            for(SocketData client : clientsData) {

                //std::thread newTransmission(&Server::TransmitData, this, std::ref(client), "B");
                //newTransmission.join();

                /* Get size of data */
                std::string dataSizeReceived;
                //ReceiveData(client, dataSizeReceived,  CHAR_SIZE*dataSizeStdAmountOfDigits);
                std::thread newReceivingOfDataSize(&Server::ReceiveData, this, std::ref(client), std::ref(dataSizeReceived), CHAR_SIZE*dataSizeStdAmountOfDigits);
                newReceivingOfDataSize.join();


                /* Get  data */
                //printf("%d\n", CHAR_SIZE*std::atoi(dataSizeReceived.c_str()));
                std::string dataReceived;
                std::thread newReceivingOfData(&Server::ReceiveData, this, std::ref(client), std::ref(dataReceived), CHAR_SIZE*std::atoi(dataSizeReceived.c_str()));
                newReceivingOfData.join();
                //ReceiveData(client, dataReceived, CHAR_SIZE*std::atoi(dataSizeReceived.c_str()));
            }

            std::this_thread::sleep_for(std::chrono::seconds(1));

        }
    }

};

int main() {
    Server server;
    return 0;
}

