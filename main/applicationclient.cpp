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

        while(true) {

            printf("Welcome!\nEnter <number> to proceed:\n<1> Login.\n<?> Create account.\nInput: ");

            std::string input; std::cin >> input;
            std::string dataSent;
            if(input == "1") {
                dataSent = LoginUser();
                TransmitData2Steps(dataSent);
                std::string response = ReceiveData2Steps();
                if(response == "SUCCESS")
                    return true;
            }
            else {
                dataSent = CreateUser();
                TransmitData2Steps(dataSent);
                std::string response = ReceiveData2Steps();
                if(response == "SUCCESS")
                    continue;
            }
            return false;
        }
    }

    std::string CreateUser() {
        Log log("Started creating user.");

        std::string login, password;

        printf("Desired login: ");
        std::cin >> login;
        printf("Desired password: ");
        std::cin >> password;

        return "CREATE " + login + " " + password;
    }

    std::string LoginUser() {
        Log log("Started user login.");

        std::string login, password;

        printf("Login: ");
        std::cin >> login;
        printf("Password: ");
        std::cin >> password;

        return "HI " + login + " " + password;
    }

    std::string HandleUserAtMenu() {
        Log log("Handling user at menu.");

        /* Receive menu */
        std::string menuReceived = ReceiveData2Steps();
        std::cout << menuReceived;

        /* Process user input */
        std::string menuRequest = user.HandleMenu();
        //std::cout << menuRequest << std::endl;

        /*Transmit request to server */
        TransmitData2Steps(menuRequest);
        return menuRequest;
    }

    bool HandleUserAtOrganizer() {
        Log log("Handling user at organizer menu.");

        /* Receive organizer menu */
        std::string menuReceived = ReceiveData2Steps();
        std::cout << menuReceived;

        /* Process user input */
        std::string menuRequest = user.HandleOrganizerMenu();
        //std::cout << menuRequest << std::endl;

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
