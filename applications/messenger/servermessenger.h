
#ifndef PROJECT_BIRD_SERVER_MESSENGER_H
#define PROJECT_BIRD_SERVER_MESSENGER_H

#include "../../applications/messenger/messengeruser.h"

class ServerMessenger {

private:
    std::map<int, MessengerUser> userList;

public:
    ServerMessenger() {}

    std::string getMenu() {
        return "\nEnter <number>:\n<1> Listen messages.\n<2> List available users.\n<3> List messages from specific user.\n<4> Send message to specific user.\n";
    }

    std::string ProcessRequest(int &userId, std::string &request) {

        std::string word;
        std::stringstream stream(request);
        stream >> word;

        if(word == "ENROLL") {

        }

        if(word == "USERS") {
            std::string response;
            for(auto element : userList) {
                MessengerUser &user = element.second;
                response.append(
                        "ID <"+ std::to_string(user.refID())+"> : "+
                        user.refLogin());
            }
            return response;
        }

        if(word == "FROM") {
            int destId;
            stream >> destId;
            if(userList.count(userId)) {
                 std::vector<std::string>
                    messages = userList[userId].getMessagesFrom(destId);
                 std::string response;
                 for(std::string message : messages)
                    response.append(message+"\n");
                 return response;
            }
        }

        if(word == "SEND") {
            int otherId;
            std::string message;
            stream >> otherId;
            getline(stream, message);
            if(userList.count(userId)) {
                userList[userId].insertMessageFrom(otherId, message);
                return "SUCCESS";
            }
        }

        return "ERROR";
    }

};


#endif

