#include "../util/header.h"
#include "../util/log.h"
#include "../main/server.h"

class ApplicationServer : public Server {
private:

    std::map<std::string, std::string> database;

    std::string ReceiveData2Steps(ServerUser &user) {
        std::string dataSizeReceived;
        if(!ReceiveData(user.refSocketData(), std::ref(dataSizeReceived), dataSizeStdAmountOfDigits))
            user.refStatus() = DISCONNECTED;

        std::string dataReceived;
        if(!ReceiveData(user.refSocketData(), std::ref(dataReceived), std::atoi(dataSizeReceived.c_str())))
            user.refStatus() = DISCONNECTED;

        return dataReceived;
    }

    void TransmitData2Steps(ServerUser &user, std::string &data) {
        std::string dataSize = std::to_string(data.size());
        formatDataSizeString(dataSize);
        if(!TransmitData(user.refSocketData(), std::ref(dataSize)))
            user.refStatus() = DISCONNECTED;

        if(!TransmitData(user.refSocketData(), data))
            user.refStatus() = DISCONNECTED;
    }

    void AuthenticateUser(ServerUser &user) {
        Log log("Started authentication.");

        while(true) {

            /* Critical: check if user still connected */
            if(user.refStatus() == DISCONNECTED) {
                return;
            }

            /* Receive "HI <login> <password>" from client or
             * Received "CREATE <login> <password>" from client */
            std::string dataReceived = ReceiveData2Steps(user);

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
            TransmitData2Steps(user, message);
            return;
        }
        message = "FAILURE";
        TransmitData2Steps(user, message);
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
            TransmitData2Steps(user, message);
            return;
        }
        message = "FAILURE";
        TransmitData2Steps(user, message);
    }

    void HandleUserAtMenu(ServerUser &user) {
        Log log("Handle user at menu.");

        /* Sent menu to client */
        std::string dataSent = user.getMenu();
        TransmitData2Steps(user, dataSent);

        /* Receive client input from menu options*/
        std::string dataReceived = ReceiveData2Steps(user);
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

            /* Critical: check if user still connected */
            if(user.refStatus() == DISCONNECTED)
                return;

            /* Send organizer menu to client */
            std::string dataSent = user.getOrganizerMenu();
            TransmitData2Steps(user, dataSent);

            /* Receive client input from menu options */
            std::string dataReceived = ReceiveData2Steps(user);
            //std::cout << dataReceived << std::endl;
            if(dataReceived == "QUIT") {
                user.refLocation() = AT_MENU;
                return;
            }

            /* Make changes in databse */
            std::string requestStatus = user.ProcessOrganizerRequest(dataReceived);
            //std::cout << requestStatus << std::endl;
            TransmitData2Steps(user, requestStatus);
        }
    }

public:

    ApplicationServer() : Server() {

        HandleUserRequest();
    }

    /* Need handle client dc */
    void HandleUserRequest() {
        Log log("Handling user requests.");

        while(true) {

            std::deque<ServerUser> &userQueue = tracker.refNonAuthenticated();

            while(!userQueue.empty()) {

                ServerUser *user = new ServerUser(userQueue.front());
                std::thread (&ApplicationServer::ThreadHandleUserRequest, this,
                    std::ref(userQueue), user).detach();
                userQueue.pop_front();
            }

            //std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    void ThreadHandleUserRequest(std::deque<ServerUser> &userQueue, ServerUser *user) {

        ServerUser &serverUser = *user;

        if(serverUser.refStatus() == CONNECTED) {
            /* Process user location */
            if(serverUser.refLocation() == NOT_AUTH)
                AuthenticateUser(serverUser);
            if(serverUser.refLocation() == AT_MENU)
                HandleUserAtMenu(serverUser);
            if(serverUser.refLocation() == AT_ORGANIZER)
                HandleUserAtOrganizer(serverUser);

            /* User still connected? */
            if(serverUser.refStatus() == CONNECTED) {
                std::cerr << "CONNECTED";
                userQueue.push_back(serverUser);
                return;
            }
        }
        free(user);
    }
};

int main() {
    ApplicationServer applicationServer;
    return 0;
}
