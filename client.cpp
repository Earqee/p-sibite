#include "header.h"
#include "log.h"
#include "socketdata.h"
#include "localsocket.h"

class Client {

private:
    int connectionStatus = INVALID;
    Socket socket = Socket(PORT);

public:
    void MakeConnection() {
        Log log("Making connection");

        connectionStatus = connect(socket.getSocketFD(),
                (sockaddr*) &socket.getSocketAddress(), sizeof(socket.getSocketAddress()));

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
        }
    }

    void SendData(const char *data) {
        Log log("Sending data.");

        if(connectionStatus != INVALID) {

            int bytesSent = 0,
                dataSize = sizeof(data);

            while(bytesSent != dataSize) {

                int sendStatus = send(socket.getSocketFD(), data + bytesSent, dataSize - bytesSent, 0);

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

    void ReceiveData(int dataSize) {
        Log log("Receiving data.");

        if(connectionStatus != INVALID) {

            char *data = new char[dataSize];

            int bytesReceived = 0;

            while(bytesReceived != dataSize) {

                int recvStatus = recv(socket.getSocketFD(), data + bytesReceived, dataSize - bytesReceived, 0);

                if(recvStatus == INVALID) {
                    if(errno == EBADF)
                        log.logError("The socket argument is not a valid file descriptor.");
                    if(errno == EINTR)
                        log.logError("The operation was interrupted by a signal before any data was sent. See Interrupted Primitives.");
                    if(errno == ENOTSOCK)
                        log.logError("The descriptor socket is not a socket.");
                    if(errno == EWOULDBLOCK)
                        log.logError("Nonblocking mode has been set on the socket, and the write operation would block. (Normally send blocks until the operation can be completed.");
                    if(errno == ENOTCONN)
                        log.logError("You never connected this socket.");
                    continue;
                }
                bytesReceived += recvStatus;
            }
            printf("Client: '%s'\n", data);
        }
    }

public:
    Client() {

        MakeConnection();
        while(true) {
            SendData("Hello.");
            for(int i=0; i<100000000; i++);
        }

    }
};

int main() {
    Client client;
    return 0;
}
