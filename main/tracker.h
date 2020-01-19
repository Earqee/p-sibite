#ifndef ICVPN_TRACKER_H
#define ICVPN_TRACKER_H

#include "../util/header.h"
#include "../applications/serveruser.h"

class Tracker {

private:
    int nextNonAuthenticatedID = 0, nextAtOrganizedID = 0;
    std::set<ServerUser> nonAuthenticated;
    std::set<ServerUser> atOrganizer;
public:

    void insertAtNonAuthenticated(SocketData &socketData) {
        nonAuthenticated.insert(ServerUser(nextNonAuthenticatedID++, socketData));
    }

    void insertAtAtOrganizer(SocketData &socketData) {
        atOrganizer.insert(ServerUser(nextNonAuthenticatedID++, socketData));
    }

    //

    void removeFromNonAuthenticated(ServerUser &serverUser) {
        nonAuthenticated.erase(serverUser);
    }


    void removeFromAtAtOrganizer(ServerUser &serverUser) {
        atOrganizer.erase(serverUser);
    }

    std::set<ServerUser>& refNonAuthenticated() {
        return nonAuthenticated;
    }

    std::set<ServerUser>& refAtOrganizer() {
        return atOrganizer;
    }

    int countNonAuthenticated() {
        return nonAuthenticated.size();
    }

    int countAtOrganizer() {
        return atOrganizer.size();
    }


};

#endif
