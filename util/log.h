#ifndef ICVPN_LOG_H
#define ICVPN_LOG_H

#include "header.h"

class Log {

private:
    std::string action;

    void log(std::string status) {
        if(DEBUG)
            printf("[%s] %s\n", status.c_str(), action.c_str());
    }
public:
    Log(std::string action) {
        this->action = action;
        log("begin");
    }
    ~Log() {
        log("end");
    }
    void logCannot() {
        log("could not finish");
    }
    void logError(std::string message) {
        if(DEBUG)
            printf("[error] %d - %s (%s)\n", errno, strerror(errno), message.c_str());
    }
};


#endif

