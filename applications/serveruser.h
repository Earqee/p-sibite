#ifndef ICVPN_SERVER_USER_H
#define ICVPN_SERVER_USER_H

#include "../socket/socketdata.h"
#include "../applications/organizer/serverorganizer.h"

class ServerUser
{
private:
    int ID;
    SocketData socketData;
    ServerOrganizer serverOrganizer;
public:

    ServerUser(int ID, SocketData &socketData) {
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

    bool operator<(const ServerUser &serverUser) const {
        return this->ID < serverUser.ID;
    }
};

#endif
