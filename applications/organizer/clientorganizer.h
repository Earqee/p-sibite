#ifndef ICVPN_CLIENT_ORGANIZER_H
#define ICVPN_CLIENT_ORGANIZER_H

#include "../../util/header.h"
#include "../../util/log.h"

class ClientOrganizer {
public:

    ClientOrganizer(){}

    std::string askMenu() {
        return "MENU";
    }

    std::string HandleMenu() {

        std::string input; std::cin >> input;

        if(input == "1") return addTask();
        if(input == "2") return removeTask();
        if(input == "3") return editTask();
        if(input == "4") return viewDay();
        if(input == "5") return viewWeek();
        return quit();
    }

    std::string addTask() {
        printf("digite o titulo e o dia");
        std::string title, day;
        std::cin >> title >> day;
        return "ADD " + day + " " + title;
    }

    std::string removeTask() {
        printf("digite o dia e o indice");
        std::string day, index;
        std::cin >> day >> index;
        return "DEL " + day + " "+ index;
    }

    std::string editTask() {
        printf("digite o dia, o indice e o titulo");
        std::string title, day, index;
        std::cin >> day >> index >> title;
        return "EDIT " + day + " " + index + " " + title;
    }

    std::string viewDay() {
        printf("digite o dia");
        std::string day;
        std::cin >> day;
        return "DAY " + day;
    }

    std::string viewWeek() {
        return "WEEK";
    }

    std::string quit() {
        return "QUIT";
    }

    std::string sendData() {
        std::cout << "What you want to do?\nAdd a Task [1]\nRemove a Task [2]\nEdit a Task [3]\nView a day [4]\nView a week [5]\nExit[6]\n";

        std::string response; std::cin >> response;
        std::string formatedRequest;

        if(response == "1")
            formatedRequest = addTask();
        else if(response == "2")
            formatedRequest =removeTask();
        else if(response == "3")
            formatedRequest =editTask();
        else if(response == "4")
            formatedRequest =viewDay();
        else if(response == "5")
            formatedRequest = viewWeek();

        return formatedRequest;
    }

};

#endif
