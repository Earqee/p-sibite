#include "../util/header.h"
#include "../util/log.h"
#include "../socket/socketdata.h"
#include "../socket/serversocket.h"
#include "../socket/socketinfo.h"
#include "../socket/dummysocket.h"
#include "../main/server.h"

class ApplicationServer : public Server {

    std::map<std::string, std::string> database;

    bool AuthenticateUser(User &user) {
        std::string dataReceived = ThreadReceiveData(user.refSocketData());
        std::string word;
        std::stringstream stream(dataReceived);
        stream >> word;
        if(word == "HI") {
            std::string login, password;
            stream >> login >> password;
            if(database[login] == password) {
                tracker.insertAtAtOrganizer(user.refSocketData());
                tracker.removeFromNonAuthenticated(user);
                return true;
            }
        }
        return false;
    }

    void ProcessOrganizerRequest(User &user) {
        std::string dataReceived = ThreadReceiveData(user.refSocketData());
        std::string queryStatus = user.ProcessOrganizerRequest(dataReceived);

    }

private:

    ApplicationServer() : Server() {
    }

     void ThreadHandleAutentication() {

         while(true) {
             for(User user : tracker.refNonAuthenticated()) {
                 std::thread authenticateUser(&ApplicationServer::AuthenticateUser, this,
                         std::ref(user));
                 authenticateUser.detach(); /* Obs */
             }
        }
    }


    void ThreadHandleOrganizer() {

        while(true) {
            //for(User user : usersAtOrganizer) {
        }
    }
};

int main() {

}
