#ifndef ICVPN_SERVER_ORGANIZER_H
#define ICVPN_SERVER_ORGANIZER_H

#include "../../applications/organizer/week.h"

class ServerOrganizer {

protected:
    Week week;

public:

    ServerOrganizer() {}

    std::string getMenu() {
        std::string output = "Welcome\n";
        output.append("Enter <number> to proceed:\n<1> Add task.\n<2> Remove task.\n<3> Edit task.\n<4> View day schedule.\n<5> View week schedule.\n<6> Exit organizer.\nInput: ");
        return output;
    }

    std::string ProcessRequest(std::string &request) {

        std::string word;
        std::stringstream stream(request);
        stream >> word;

        if(word == "ADD") {
            int day; std::string title;
            stream >> day;
            getline(stream, title);
            if(week.addTask(title, day))
                return "SUCCESS";
        }

        if(word == "DEL") {
            int day, index;
            stream >> day >> index;
            if(week.removeTask(day, index))
                return "SUCCESS";
        }

        if(word == "EDIT") {
            int day, index; std::string title;
            stream >> day >> index;
            getline(stream, title);
            if(week.editTask(day, index, title))
                return "SUCCESS";
        }

        if(word == "DAY") {
            int day; stream >> day;
            return week.getDaySchedule(day);
        }

        if(word == "WEEK")
            return week.getWeekSchedule();

        return "ERROR";
    }
};

#endif
