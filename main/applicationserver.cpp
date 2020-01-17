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
        std::string dataReceived = ThreadReceiveData(serverUser.refSocketData());
        std::string word;
        std::stringstream stream(dataReceived);
        stream >> word;
        if(word == "HI") {
            std::string login, password;
            stream >> login >> password;
            if(database[login] == password) {
                tracker.insertAtAtOrganizer(serverUser.refSocketData());
                tracker.removeFromNonAuthenticated(serverUser);
                std::cout <<"Inseriu";
                return true;
            }
        }
        return false;
    }

    void ProcessOrganizerRequest(ServerUser &serverUser) {
        std::string dataReceived = ThreadReceiveData(serverUser.refSocketData());
        std::string queryStatus = serverUser.ProcessOrganizerRequest(dataReceived);

    }

public:


    ApplicationServer() : Server() {

        database["abc"] = "def";

        std::thread acceptConnections(&Server::AcceptConnections,this);
        acceptConnections.join();

        std::thread handleAuthentication(&ApplicationServer::ThreadHandleAutentication, this);
        handleAuthentication.join();
    }

     void ThreadHandleAutentication() {
         //while(true) {
             for(ServerUser serverUser : tracker.refNonAuthenticated()) {
                 std::thread authenticateUser(&ApplicationServer::AuthenticateUser, this,
                         std::ref(serverUser));
                 authenticateUser.join(); /* Obs */
                 std::cout << tracker.refNonAuthenticated().size() << std::endl;
             }


        //}
    }


     /*
    void ThreadHandleOrganizer() {

        while(true) {

            //for(User user : usersAtOrganizer) {
        }
    }

    */
};

int main() {

    ApplicationServer applicationServer;
}
