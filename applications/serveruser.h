#ifndef ICVPN_SERVER_USER_H
#define ICVPN_SERVER_USER_H

#include "../socket/socketdata.h"
#include "../applications/organizer/serverorganizer.h"

class ServerUser {
private:
    int ID, location, status;
    SocketData socketData;
    ServerOrganizer serverOrganizer;
public:

    ServerUser(int ID, int location, int status,
            SocketData &socketData) {
        this->ID = ID;
        this->location = location;
        this->status = status;
        this->socketData = socketData;
    }

    int getID() {
        return ID;
    }

    int& refID() {
        return ID;
    }

    int& refLocation() {
        return location;
    }

    int& refStatus() {
        return status;
    }

    SocketData& refSocketData() {
        return socketData;
    }

    std::string getMenu() {
        return "Enter <number> to proceed:\n<1> Organizer\n<2> Torrent\nInput: ";
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
