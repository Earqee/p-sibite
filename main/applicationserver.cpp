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
                user.refLocation() = AT_MENU;
                TransmitData2Steps(user.refSocketData(), message);
                return true;
            }
        }
        message = "FAILURE";
        TransmitData2Steps(user.refSocketData(), message);
        return false;
    }

    void HandleUserInMenu(ServerUser &user) {
        std::string message = user.getOrganizerMenu();
        TransmitData2Steps(user.refSocketData(), message);
    }

    void ProcessOrganizerRequest(ServerUser &serverUser) {
        std::string dataReceived = ReceiveData2Steps(serverUser.refSocketData());
        //std::string queryStatus = serverUser.ProcessOrganizerRequest(dataReceived);
    }

public:

    ApplicationServer() : Server() {
        database["abc"] = "def";
        database["abcd"] = "defg";

        HandleUserRequest();
    }

     /* Need handle client dc */
     void HandleUserRequest() {
         Log log("Started listening users at organizer.");

         std::set<int> processInQueue;

         while(true) {

             for(ServerUser user : tracker.refNonAuthenticated()) {

                 int &userRequestID = user.refSocketData().refSocketFD();

                 if(!processInQueue.count(userRequestID))  {

                     processInQueue.insert(userRequestID);

                     if(user.refLocation() == NOT_AUTH)
                         AuthenticateUser(user);
                     if(user.refLocation() == AT_MENU)
                         HandleUserInMenu(user);
                     //if(user.refLocation() == AT_ORGANIZER)
                         //ProcessOrganizerRequest(user);

                     processInQueue.erase(userRequestID);
                 }

             }

             std::this_thread::sleep_for(std::chrono::seconds(5));
         }
     }


};

int main() {

    ApplicationServer applicationServer;
}
