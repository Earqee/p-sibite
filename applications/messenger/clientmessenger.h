#ifndef PROJECT_BIRD_CLIENT_MESSENGER_H
#define PROJECT_BIRD_CLIENT_MESSENGER_H

class ClientMessenger {

private:

public:
    ClientMessenger() {}

    std::string HandleMenu() {

        std::string input;
        std::cin >> input;

        if(input == "1") return enroll();
        if(input == "2") return availableUsers();
        if(input == "3") return privateMessageFrom();
        if(input == "4") return sendPrivateMessage();
        return quit();
    }

    std::string enroll() {
        return "ENROLL";
    }

    std::string availableUsers() {
        return "USERS";
    }

    std::string privateMessageFrom() {
        std::string id;
        std::cin >> id;
        return "FROM" + id;
    }

    std::string sendPrivateMessage() {
        std::string login, message;
        std::cin >> login;
        getline(std::cin, message);
        return "SEND " + login + " " + message;
    }

    std::string quit() {
        return "QUIT";
    }
};

#endif
