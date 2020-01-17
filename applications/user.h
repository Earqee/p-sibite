#ifndef ICVPN_USER_H
#define ICVPN_USER_H

#include "../socket/socketdata.h"
#include "../applications/organizer/serverorganizer.h"

class User
{
private:
    int ID;
    SocketData socketData;
    ServerOrganizer serverOrganizer;
public:

    User(int ID, SocketData &socketData) {
        this->ID = ID;
        this->socketData = socketData;
    }

    int getID() {
        return ID;
    }

    int& refID() {
        return ID;
    }

    SocketData& refSocketData() {
        return socketData;
    }

    std::string getOrganizerMenu() {
        return serverOrganizer.getMenu();
    }

    std::string ProcessOrganizerRequest(std::string &request) {
        return serverOrganizer.ProcessRequest(request);
    }

    bool operator<(const User &user) const {
        return this->ID < user.ID;
    }
};

#endif
