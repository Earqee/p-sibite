#ifndef PROJECT_BIRD_MESSENGER_USER_H
#define PROJECT_BIRD_MESSENGER_USER_H

#include "../../util/header.h"
#include "../../util/log.h"

class MessengerUser {

private:
    int id;
    std::string login;
    std::unordered_map<int, std::vector<std::string> > messages;

public:
    MessengerUser(int &id, std::string &login) {
        this->id = id;
        this->login = login;
    }

    int& refID() {
        return id;
    }

    std::string& refLogin() {
        return login;
    }

    std::vector<std::string> getMessagesFrom(int &id) {
        return messages[id];
    }

    void insertMessageFrom(int &id, std::string &message) {
        messages[id].push_back(message);
    }

};

#endif
