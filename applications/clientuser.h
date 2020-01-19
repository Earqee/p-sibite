#ifndef PROJECT_BIRD_CLIENT_USER_H
#define PROJECT_BIRD_CLIENT_USER_H

#include "../util/header.h"
#include "../util/log.h"
#include "../applications/organizer/clientorganizer.h"

class ClientUser {
private:

    ClientOrganizer clientOrganizer;
public:

    ClientUser() {}

    std::string HandleMenu() {
        std::string input;
        std::cin >> input;
        if(input == "1") return "ORGANIZER";
        if(input == "2") return "TORRENT";
        return "QUIT";
    }

    std::string HandleOrganizerMenu() {
        return clientOrganizer.HandleMenu();
    }
};

#endif
