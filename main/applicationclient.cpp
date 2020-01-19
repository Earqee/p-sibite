
#include "../util/header.h"
#include "../util/log.h"
#include "../main/client.h"
#include "../applications/clientuser.h"

class ApplicationClient : public Client {

private:
    ClientUser user;

public:
    ApplicationClient() : Client() {
        while(AuthenticateUser()) {

            std::string application = HandleUserAtMenu();

            if(application == "ORGANIZER")
                while(HandleUserAtOrganizer());
        }
    }

    std::string ReceiveData2Steps() {
        std::string dataSizeReceived;
        ReceiveData(std::ref(dataSizeReceived), dataSizeStdAmountOfDigits);

        std::string dataReceived;
        ReceiveData(std::ref(dataReceived), std::atoi(dataSizeReceived.c_str()));
        return dataReceived;
    }

    void TransmitData2Steps(std::string &data) {
        std::string dataSize = std::to_string(data.size());
        formatDataSizeString(dataSize);

        TransmitData(dataSize);
        TransmitData(data);
    }

    bool AuthenticateUser() {
        Log log("Authentication began.");

        std::string login, password;

        printf("Please, enter your login: ");
        std::cin >> login;

        printf("Please, enter your password: ");
        std::cin >> password;

        std::string formattedRequest = "HI " + login + " " + password;
        TransmitData2Steps(formattedRequest);

        std::string response = ReceiveData2Steps();
        if(response == "SUCCESS")
            return true;

        return false;
    }

    std::string HandleUserAtMenu() {
        Log log("Handling user at menu.");

        /* Receive menu */
        std::string dataReceived = ReceiveData2Steps();
        std::cout << dataReceived << std::endl;

        /* Process user input */
        std::string menuRequest = user.HandleMenu();
        std::cout << menuRequest << std::endl;

        /*Transmit request to server */
        TransmitData2Steps(menuRequest);
        return menuRequest;
    }

    bool HandleUserAtOrganizer() {
        Log log("Handling user at organizer menu.");

        /* Receive organizer menu */
        std::string dataReceived = ReceiveData2Steps();
        std::cout << dataReceived << std::endl;

        /* Process user input */
        std::string menuRequest = user.HandleOrganizerMenu();
        std::cout << menuRequest << std::endl;

        /*Transmit request to server */
        TransmitData2Steps(menuRequest);
        if(menuRequest == "QUIT")
            return false;

        /* Receive solicitation */
        std::string requestReceived = ReceiveData2Steps();
        std::cout << requestReceived << std::endl;
        return true;
    }

};

int main() {
    ApplicationClient applicationClient;
    return 0;
}
