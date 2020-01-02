#ifndef ACEVPN_LOG_H
#define ACEVPN_LOG_H

#include "header.h"

class Log {

private:
    std::string begin = "started";
    std::string cannot = "could not finish";
    std::string error = "error";
    std::string end = "finished";
    std::string action;
public:
    Log(std::string action) {
        this->action = action;
        log(begin);
    }
    ~Log() {
        log(end);
    }
    void log(std::string status) {
        printf("[%s] %s\n", status.c_str(), action.c_str());
    }
    void logCannot() {
        log(cannot);
    }
    void logError(std::string message) {
        printf("[%s] %d - %s (%s)\n", error.c_str(), errno, strerror(errno), message.c_str());
    }
};


#endif

