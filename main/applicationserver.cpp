#include "../util/header.h"
#include "../util/log.h"
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

    void AuthenticateUser(ServerUser &user) {
        Log log("Started authentication");

        while(true) {

            /* Receive "HI <login> <password>" from client or
             * Received "CREATE <login> <password>" from client */
            std::string dataReceived = ReceiveData2Steps(user.refSocketData());

            std::string word;
            std::stringstream stream(dataReceived);
            stream >> word;
            if(word == "HI") {
                LoginUser(user, stream);
                return;
            }
            if(word == "CREATE")
                CreateUser(user, stream);
        }

    }

    void CreateUser(ServerUser &user, std::stringstream &stream) {
        Log log("User creation.");

        std::string login, password, message;
        stream >> login >> password;
        if(!database.count(login)) {
            database[login] = password;
            message = "SUCCESS";
            TransmitData2Steps(user.refSocketData(), message);
            return;
        }
        message = "FAILURE";
        TransmitData2Steps(user.refSocketData(), message);
    }

    void LoginUser(ServerUser &user, std::stringstream &stream) {
        Log log("User login.");

        /* Process received data */
        std::string login, password, message;
        stream >> login >> password;
        if(database[login] == password) {
            //tracker.insertAtAtOrganizer(user.refSocketData());
            //tracker.removeFromNonAuthenticated(user);
            message = "SUCCESS";
            user.refLocation() = AT_MENU;
            TransmitData2Steps(user.refSocketData(), message);
            return;
        }
        message = "FAILURE";
        TransmitData2Steps(user.refSocketData(), message);
    }

    void HandleUserInMenu(ServerUser &user) {
        Log log("Handle user at menu.");

        /* Sent menu to client */
        std::string dataSent = user.getMenu();
        TransmitData2Steps(user.refSocketData(), dataSent);

        /* Receive client input from menu options*/
        std::string dataReceived = ReceiveData2Steps(user.refSocketData());
        //std::cout << dataReceived << std::endl;

        /* Make changes in user location */
        if(dataReceived == "ORGANIZER")
            user.refLocation() = AT_ORGANIZER;
        if(dataReceived == "TORRENT")
            user.refLocation() = AT_TORRENT;
        if(dataReceived == "QUIT")
            user.refLocation() = NOT_AUTH;
    }

    void HandleUserAtOrganizer(ServerUser &user) {
        Log log("Handle user at organizer.");

        while(true) {
            /* Send organizer menu to client */
            std::string dataSent = user.getOrganizerMenu();
            TransmitData2Steps(user.refSocketData(), dataSent);

            /* Receive client input from menu options */
            std::string dataReceived = ReceiveData2Steps(user.refSocketData());
            //std::cout << dataReceived << std::endl;
            if(dataReceived == "QUIT") {
                user.refLocation() = AT_MENU;
                return;
            }

            /* Make changes in databse */
            std::string requestStatus = user.ProcessOrganizerRequest(dataReceived);
            //std::cout << requestStatus << std::endl;
            TransmitData2Steps(user.refSocketData(), requestStatus);
        }
    }

public:

    ApplicationServer() : Server() {

        HandleUserRequest();
    }

     /* Need handle client dc */
     void HandleUserRequest() {
         Log log("Handling user requests.");

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
                     if(user.refLocation() == AT_ORGANIZER)
                         HandleUserAtOrganizer(user);

                     processInQueue.erase(userRequestID);
                 }
             }

             std::this_thread::sleep_for(std::chrono::seconds(5));
         }
     }
};

int main() {
    ApplicationServer applicationServer;
    return 0;
}
