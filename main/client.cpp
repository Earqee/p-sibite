#include "../util/header.h"
#include "../util/log.h"
#include "../socket/socketdata.h"
#include "../socket/clientsocket.h"
#include "../socket/dummysocket.h"

class Client {

private:
    int connectionStatus = INVALID;
    ClientSocket socket = ClientSocket(PORT);

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

    bool TransmitData(std::string &data) {
            return socket.TransmitData(data);
    }

    void ThreadTransmitData(std::string &data) {
        std::thread newTransmissionOfData(&Client::TransmitData, this,
                    std::ref(data));
        newTransmissionOfData.join();
    }

    bool ReceiveData(std::string &data) {
        return socket.ReceiveData(data, defaultMaximumDataSize);
    }

    std::string ThreadReceiveData() {
        std::string dataReceived;
        std::thread newReceiving(&Client::ReceiveData, this, std::ref(dataReceived));
        newReceiving.join();
        return dataReceived;
    }

public:

    Client() {

        while(!MakeConnection());

    }

    std::string formattedRequest(std::string link,
            std::string host,
            std::string content) {

        std::string request;

        if(1) request.append("GET " + link + " HTTP/1.0\r\n");
        if(1) request.append("Host: " + host + "\r\n");
        if(1) request.append("Content-type: application/x-www-form-urlencoded\r\n");
        if(1) request.append("Content-length: "+ std::to_string(content.size()) +"\r\n\r\n");

        if(1) request.append(content + "\r\n");

        return request;
    }
};

int main() {
    Client client;
    return 0;
}
