
#include "../util/header.h"
#include "../util/log.h"
#include "../main/client.h"

class ApplicationClient : public Client {
private:

public:
    ApplicationClient() : Client() {
        std::thread authenticateUser(&ApplicationClient::AuthenticateUser, this);
        authenticateUser.join();
    }

    void AuthenticateUser() {
        printf("digite o login e senha\n");
        std::string login, password;
        std::cin >> login >> password;
        std::string formattedRequest = "HI " + login + " " + password;
        ThreadTransmitData(formattedRequest);
    }

};

int main() {
    ApplicationClient applicationClient;
    return 0;
}
