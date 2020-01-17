#include "../util/header.h"

class Tracker {

private:
    int nextNonAuthenticatedID = 0, nextAtOrganizedID = 0;
    std::set<User> nonAuthenticated;
    std::set<User> atOrganizer;
public:

    void insertAtNonAuthenticated(SocketData &socketData) {
        nonAuthenticated.insert(User(nextNonAuthenticatedID++, socketData));
    }

    void removeFromNonAuthenticated(User &user) {
        nonAuthenticated.erase(user);
    }

    void insertAtAtOrganizer(SocketData &socketData) {
        atOrganizer.insert(User(nextNonAuthenticatedID++, socketData));
    }

    void removeFromAtAtOrganizer(User &user) {
        atOrganizer.erase(user);
    }

    std::set<User>& refNonAuthenticated() {
        return nonAuthenticated;
    }

    std::set<User>& refAtOrganizer() {
        return atOrganizer;
    }

    int countNonAuthenticated() {
        return nonAuthenticated.size();
    }

    int countAtOrganizer() {
        return atOrganizer.size();
    }


};
