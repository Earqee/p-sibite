
#include "../util/header.h"
#include "../util/log.h"
#include "../main/client.h"

class ApplicationClient : public Client {

private:

public:
    ApplicationClient() : Client() {
        AuthenticateUser();
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

    void AuthenticateUser() {
        printf("digite o login e senha\n");

        std::string login, password;
        std::cin >> login >> password;
        std::string formattedRequest = "HI " + login + " " + password;

        TransmitData2Steps(formattedRequest);
        std::string response = ReceiveData2Steps();
        std::cout << response << std::endl;
    }
};

int main() {
    ApplicationClient applicationClient;
    return 0;
}
