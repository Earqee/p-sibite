
#include "../socket/socketdata.h"
#include "../applications/organizer/organizer.h"

class User
{
private:
    int ID;
    SocketData socketData;
    OrganizerData organizer;
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
        return organizer.getMenu();
    }

    std::string ProcessOrganizerRequest(std::string &request) {
        return organizer.ProcessRequest(request);
    }

    bool operator<(const User &user) const {
        return this->ID < user.ID;
    }
};
