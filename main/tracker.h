#ifndef PROJECT_BIRD_TRACKER_H
#define PROJECT_BIRD_TRACKER_H

#include "../util/header.h"
#include "../applications/serveruser.h"

class Tracker {

private:
    int nextNonAuthenticatedID = 0, nextAtOrganizedID = 0;
    std::deque<ServerUser> nonAuthenticated;
    std::deque<ServerUser> atOrganizer;
public:

    void insertAtNonAuthenticated(SocketData &socketData) {
        nonAuthenticated.push_back(ServerUser(nextNonAuthenticatedID++, NOT_AUTH, CONNECTED, socketData));
    }

    void insertAtAtOrganizer(SocketData &socketData) {
        atOrganizer.push_back(ServerUser(nextNonAuthenticatedID++, NOT_AUTH, CONNECTED, socketData));
    }

    std::deque<ServerUser>& refNonAuthenticated() {
        return nonAuthenticated;
    }

    std::deque<ServerUser>& refAtOrganizer() {
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
