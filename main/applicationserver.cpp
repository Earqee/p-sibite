#include "../util/header.h"
#include "../util/log.h"
#include "../socket/socketdata.h"
#include "../socket/serversocket.h"
#include "../socket/socketinfo.h"
#include "../socket/dummysocket.h"
#include "../main/server.h"

class ApplicationServer : public Server {
private:

    std::map<std::string, std::string> database;

    bool AuthenticateUser(ServerUser &serverUser) {
        Log log("Started authentication");

        std::cout << ntohs(serverUser.refSocketData().refSocketAddress().sin6_port);
        //std::cout << serverUser.refSocketData().refSocketFD() << std::endl;
        std::string dataReceived = ThreadReceiveData(serverUser.refSocketData());
        std::string word, message;
        std::stringstream stream(dataReceived);
        stream >> word;
        if(word == "HI") {
            std::string login, password;
            stream >> login >> password;
            if(database[login] == password) {
                tracker.insertAtAtOrganizer(serverUser.refSocketData());
                tracker.removeFromNonAuthenticated(serverUser);
                message = "SUCCESS";
                ThreadTransmitData(serverUser.refSocketData(), message);
                return true;
            }
        }
        message = "FAILURE";
        ThreadTransmitData(serverUser.refSocketData(), message);
        return false;
    }

    void ProcessOrganizerRequest(ServerUser &serverUser) {
        std::string dataReceived = ThreadReceiveData(serverUser.refSocketData());
        std::string queryStatus = serverUser.ProcessOrganizerRequest(dataReceived);
    }

public:


    ApplicationServer() : Server() {
        database["abc"] = "def";
        database["abcd"] = "defg";

        //std::thread (&Server::AcceptConnections,this).detach();
        AcceptConnections();

        //std::thread (&ApplicationServer::ThreadHandleAutentication, this).join();
        //ThreadHandleAutentication();

        for(SocketData client : clientsData) {
            std::string dataReceived = ThreadReceiveData(client);
        }
    }

    /* Fix later */
     void ThreadHandleAutentication() {

        Log log("Starting listening non authenticated");
        std::set<int> socketsInQueue;

         while(true) {
             for(ServerUser serverUser : tracker.refNonAuthenticated()) {
                 if(!socketsInQueue.count(serverUser.refSocketData().refSocketFD()))  {
                     socketsInQueue.insert(serverUser.refSocketData().refSocketFD());



                    std::thread (&ApplicationServer::AuthenticateUser, this,
                        std::ref(serverUser)).join();
                    break;
                 }
             }
        }
    }


    void ThreadHandleOrganizer() {

        std::set<int> socketsInQueue;

        while(true) {

             for(ServerUser serverUser : tracker.refAtOrganizer()) {

                 if(!socketsInQueue.count(serverUser.refSocketData().refSocketFD()))  {

                     socketsInQueue.insert(serverUser.refSocketData().refSocketFD());

                    std::thread authenticateUser(&ApplicationServer::AuthenticateUser, this,
                         std::ref(serverUser));
                    authenticateUser.join();
                    break;
                 }
             }
        }
            //for(User user : usersAtOrganizer) {
    }

};

int main() {

    ApplicationServer applicationServer;
}
