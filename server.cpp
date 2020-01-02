
#include "header.h"
#include "log.h"
#include "socketdata.h"
#include "serversocket.h"

class Server {

private:

    int connectionStatus = INVALID;
    Socket socket = Socket(PORT);
    std::vector<SocketData> clientsData;

    void ListenConnection() {
        Log log("Listening connections.");

        connectionStatus = listen(socket.getSocketFD(), maxNumOfConnections);

        if(connectionStatus == INVALID) {
            log.logCannot();
            if(errno == EBADF)
                log.logError("The argument socket is not a valid file descriptor.");
            if(errno == ENOTSOCK)
                log.logError("The argument socket is not a socket.");
            if(errno == EOPNOTSUPP)
                log.logError("The socket socket does not support this operation.");
        }
    }

    void AcceptConnection() {
        Log log("Accepting connections.");

        if(connectionStatus != INVALID) {

            SocketData clientData;

            while(clientData.socketFD == INVALID) {

                clientData.socketFD = accept(socket.getSocketFD(), (sockaddr*) &clientData.socketAddress, &clientData.socketAddressLen);

                if(clientData.socketFD == INVALID) {
                    log.logCannot();
                    if(errno == EBADF)
                        log.logError("The socket argument is not a valid file descriptor.");
                    if(errno == ENOTSOCK)
                        log.logError("The descriptor socket argument is not a socket.");
                    if(errno == EOPNOTSUPP)
                        log.logError("The descriptor socket does not support this operation.");
                    if(errno == EWOULDBLOCK)
                        log.logError("socket has nonblocking mode set, and there are no pending connections immediately available.");
                }
            }

            clientsData.push_back(clientData);
            printf("New connection at %d.\n", ntohs(clientData.socketAddress.sin6_port));
        }
    }

    void SendData(SocketData &clientData, const char *data) {
        Log log("Sending data.");

        if(clientData.socketFD != INVALID) {

            int bytesSent = 0,
                dataSize = sizeof(data);

            while(bytesSent != dataSize) {

                int sendStatus = send(clientData.socketFD, data + bytesSent, dataSize - bytesSent, 0);

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
                    return;
                }
                bytesSent += sendStatus;
            }
        }
    }

    void ReceiveData(SocketData &clientData, int dataSize) {
        Log log("Receiving data.");

        if(clientData.socketFD != INVALID) {

            char *data = new char[dataSize];

            int bytesReceived = 0;

            while(bytesReceived != dataSize) {

                int recvStatus = recv(clientData.socketFD, data + bytesReceived, dataSize - bytesReceived, 0);

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
                    return;
                }
                bytesReceived += recvStatus;
            }
            data[dataSize-1] = '\0';
            printf("Server: '%s'\n", data);
        }
    }

public:
    Server() {}

    void run() {
        ListenConnection();
        AcceptConnection();

        while(true) {
            for(SocketData client: clientsData)
                ReceiveData(client, CHAR_SIZE*sizeof("Hello."));
            for(int i=0; i<100000000; i++);
        }
    }
};

int main() {
    Server server;
    server.run();
    return 0;
}

