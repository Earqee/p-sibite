#include "../util/header.h"
#include "../util/log.h"
#include "../socket/socketdata.h"
#include "../socket/serversocket.h"

class Server {

private:

    int connectionStatus = INVALID;
    ServerSocket socket = ServerSocket(PORT);
    std::vector<SocketData> clientsData;
    std::vector<std::thread> clientsTransmissionThreads;
    std::vector<std::thread> clientsReceivingThreads;

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

        //while(true) {

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
            clientsData.push_back(clientData);
        //}
        return true;
    }

    bool TransmitData(SocketData &clientData, const char *data) {
        Log log("Sending data.");

        if(clientData.refSocketFD() == INVALID)
            return false;

        int bytesSent = 0,
            dataSize = sizeof(data);

        while(bytesSent != dataSize) {

            int sendStatus = send(clientData.refSocketFD(), data + bytesSent, dataSize - bytesSent, 0);

            printf("Sent: %d | Total: %d/%d\n", sendStatus
                    , sendStatus>0?sendStatus:0 + bytesSent, dataSize);

            if(sendStatus == INVALID) {
                log.logCannot();
                if(errno == EBADF)
                    log.logError("The socket argument is not a valid file descriptor.");
                if(errno == EINTR) {
                    log.logError("The operation was interrupted by a signal before any data was sent. See Interrupted Primitives.");
                    continue;
                }
                if(errno == ENOTSOCK)
                    log.logError("The descriptor socket is not a socket.");
                if(errno == EMSGSIZE)
                    log.logError("The socket type requires that the message be sent atomically, but the message is too large for this to be possible.");
                if(errno == EWOULDBLOCK) {
                    log.logError("Nonblocking mode has been set on the socket, and the write operation would block. (Normally send blocks until the operation can be completed.");
                    continue;
                }
                if(errno == ENOBUFS)
                    log.logError("There is not enough internal buffer space available.");
                if(errno == ENOTCONN)
                    log.logError("You never connected this socket.");
                if(errno == EPIPE)
                    log.logError("This socket was connected but the connection is now broken.");
                return false;
            }
            bytesSent += sendStatus;
        }

        return true;
    }

    bool ReceiveData(SocketData &clientData, int dataSize) {
        Log log("Receiving data.");

        if(clientData.refSocketFD() == INVALID)
            return false;

        char *data = new char[dataSize];

        int bytesReceived = 0;

        while(bytesReceived != dataSize) {

            int recvStatus = recv(clientData.refSocketFD(), data + bytesReceived, dataSize - bytesReceived, 0);

            printf("Received: %d | Total: %d/%d\n", recvStatus
                    , recvStatus>0?recvStatus:0 + bytesReceived, dataSize);

            if(recvStatus == INVALID) {
                if(errno == EBADF)
                    log.logError("The socket argument is not a valid file descriptor.");
                if(errno == EINTR) {
                    log.logError("The operation was interrupted by a signal before any data was sent. See Interrupted Primitives.");
                    continue;
                }
                if(errno == ENOTSOCK)
                    log.logError("The descriptor socket is not a socket.");
                if(errno == EWOULDBLOCK) {
                    log.logError("Nonblocking mode has been set on the socket, and the write operation would block.");
                    continue;
                }
                if(errno == ENOTCONN)
                    log.logError("You never connected this socket.");
                return false;
            }
            bytesReceived += recvStatus;
        }
        printf("CLIENT (%d) SENT = {{\n%s\n}}\n", ntohs(clientData.refSocketAddress().sin6_port), data);
        return true;
    }

public:
    Server() {

        ListenConnection();

        std::thread acceptConnectionThread(&Server::AcceptConnections, this);
        acceptConnectionThread.join();

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

