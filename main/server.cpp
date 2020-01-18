
#include "../util/header.h"
#include "../util/log.h"
#include "../socket/socketdata.h"
#include "../socket/serversocket.h"
#include "../socket/socketinfo.h"
#include "../socket/dummysocket.h"
#include "../applications/serveruser.h"
#include "../main/tracker.h"

class Server {

protected:

    int connectionStatus = INVALID;
    ServerSocket serverSocket = ServerSocket(PORT);
    std::vector<SocketData> clientsData;
    Tracker tracker;

public:

    bool ListenConnection() {
        Log log("Listening connections.");

        connectionStatus = listen(serverSocket.refSocketFD(), maxNumOfConnections);

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

        //while(true) {

            SocketData clientData;

            while(clientData.refSocketFD() == INVALID) {

                clientData.refSocketFD() = accept(serverSocket.refSocketFD(), (sockaddr*) &clientData.refSocketAddress(), &clientData.refSocketAddressLen());

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
            clientsData.push_back(clientData);
        //}
        return true;
    }

    bool TransmitData(SocketData &clientData, std::string &data) {
        return serverSocket.TransmitData(clientData, data);
    }

    bool ReceiveData(SocketData &clientData, std::string &data) {
        return serverSocket.ReceiveData(clientData, data, defaultMaximumDataSize);
    }

public:
    Server() {
        ListenConnection();

        AcceptConnections();

        while(true) {

            for(SocketData client : clientsData) {
                std::string data = "HI";
                std::string dataSize = std::to_string(data.size());
                formatDataSizeString(dataSize);

                ThreadTransmitData(client, dataSize);
                ThreadTransmitData(client, data);
            }
            break;

        }

    }

    void ThreadTransmitData(SocketData &clientData, std::string &data) {
        serverSocket.TransmitData(clientData, data);
    }

    std::string ThreadReceiveData(SocketData &clientData, int dataSize) {
        std::string dataReceived;
        serverSocket.ReceiveData(dataReceived, dataSize);
        return dataReceived;
    }

};

int main() {
    Server server;
    return 0;
}

