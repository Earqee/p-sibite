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

    std::string ReceiveData2Steps(SocketData &socketData) {
        std::string dataSizeReceived;
        ReceiveData(socketData, std::ref(dataSizeReceived), dataSizeStdAmountOfDigits);

        std::string dataReceived;
        ReceiveData(socketData, std::ref(dataReceived), std::atoi(dataSizeReceived.c_str()));
        return dataReceived;
    }

    void TransmitData2Steps(SocketData &socketData, std::string &data) {
        std::string dataSize = std::to_string(data.size());
        formatDataSizeString(dataSize);
        TransmitData(socketData, std::ref(dataSize));

        TransmitData(socketData, data);
    }

    bool AuthenticateUser(ServerUser &user) {
        Log log("Started authentication");

        std::string dataReceived = ReceiveData2Steps(user.refSocketData());

        std::string word, message;
        std::stringstream stream(dataReceived);
        stream >> word;
        if(word == "HI") {
            std::string login, password;
            stream >> login >> password;
            if(database[login] == password) {
                tracker.insertAtAtOrganizer(user.refSocketData());
                tracker.removeFromNonAuthenticated(user);
                message = "SUCCESS";
                TransmitData2Steps(user.refSocketData(), message);
                return true;
            }
        }
        message = "FAILURE";
        TransmitData2Steps(user.refSocketData(), message);
        return false;
    }

    /*
    void ProcessOrganizerRequest(ServerUser &serverUser) {
        std::string dataReceived = ThreadReceiveData(serverUser.refSocketData());
        std::string queryStatus = serverUser.ProcessOrganizerRequest(dataReceived);
    }

    */

public:

    ApplicationServer() : Server() {
        database["abc"] = "def";
        database["abcd"] = "defg";

        ThreadHandleAutentication();
    }

    /* Fix later */
     void ThreadHandleAutentication() {

        Log log("Starting listening non authenticated");
        std::set<int> socketsInQueue;

        std::cout << tracker.refNonAuthenticated().size();

         while(true) {

             for(ServerUser user : tracker.refNonAuthenticated()) {

                 if(!socketsInQueue.count(user.refSocketData().refSocketFD()))  {
                     socketsInQueue.insert(user.refSocketData().refSocketFD());
                     AuthenticateUser(user);
                    break;
                 }
             }
             break;
        }
    }


};

int main() {

    ApplicationServer applicationServer;
}
