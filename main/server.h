#include "../util/header.h"
#include "../util/log.h"
#include "../socket/socketdata.h"
#include "../socket/serversocket.h"
#include "../socket/socketinfo.h"
#include "../socket/dummysocket.h"

#include <deque>

class Server {

private:

    int connectionStatus = INVALID;
    ServerSocket serverSocket = ServerSocket(PORT);
    std::deque<SocketData> clientsData;

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

        while(true) {

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
            clientsData.push_back(SocketData(clientData));
        }
        return true;
    }

    bool TransmitData(SocketData &clientData, std::string &data) {
        return serverSocket.TransmitData(clientData, data);
    }

    

    bool ReceiveData(SocketData &clientData, std::string &data) {
        return serverSocket.ReceiveData(clientData, data, defaultMaximumDataSize);
    }

    

    bool TransmitHTTPtoWeb(std::string host, std::string port) {
        Log log("Transmitting HTTP to Web");

        /*

        SocketInfo socketInfo = SocketInfo(host, port);

        int addressQueryStatus =
            getaddrinfo(host.c_str(),
                    port.c_str(),
                    &socketInfo.refSpecification(),
                    &socketInfo.refResult());

        if(addressQueryStatus != 0) {
            log.logCannot();
            printf("No such host.");
            return false;
        }

        for(addrinfo *addressResult = socketInfo.refResult();
                addressResult != NULL;
                addressResult = addressResult->ai_next) {

            DummySocket hostSocket = DummySocket(80);

            int hostConnectionStatus =
                connect(hostSocket.refSocketFD(),
                        addressResult->ai_addr,
                        addressResult->ai_addrlen);

            if(hostConnectionStatus == INVALID) {
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
                break;
            }

            std::string data = "GET / HTTP/1.1 \r\nHost: "+ host +" \r\nConnection: close \r\n";
            hostSocket.TransmitData(data);
            std::string dataReceived;
            hostSocket.ReceiveData(dataReceived, 1024);

            int addressBindConStatus =
                bind(hostSocket.refSocketFD(),
                        (sockaddr*) &hostSocket.refSocketAddress(),
                        hostSocket.refSocketAddressLen());

            if(addressBindConStatus == INVALID) {
                printf("Could not bind");
                continue;
            }
        }


        DummySocket hostSocket(80);

        std::string data = "GET / HTTP/1.1 \r\nHost: "+ host +" \r\nConnection: close \r\n";
        hostSocket.TransmitData(data);
        std::string dataReceived;
        hostSocket.ReceiveData(dataReceived, 1024);
        */
        return true;
    }

public:
    Server() {

        ListenConnection();

        std::thread acceptConnectionThread(&Server::AcceptConnections, this);
        acceptConnectionThread.detach();

        while(true) {
            for(SocketData client : clientsData)
                ThreadReceiveData(client);
            std::this_thread::sleep_for(std::chrono::seconds(10));
        }
    }
    void ThreadTransmitData(SocketData &clientData, std::string &data) {
        std::thread newTransmission(&Server::TransmitData, this, std::ref(clientData), std::ref(data));
        newTransmission.join();
    }
    std::string ThreadReceiveData(SocketData &clientData) {
        std::string dataReceived;
        std::thread newReceivingOfData(&Server::ReceiveData, this,
            std::ref(clientData),
            std::ref(dataReceived));
        newReceivingOfData.join();
        return dataReceived;
    }
};

