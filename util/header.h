#ifndef PROJECT_BIRD_HEADER_H
#define PROJECT_BIRD_HEADER_H

/* External dependences */
#include <bits/stdc++.h>
#include <errno.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

/* Debugging */
#define DEBUG 1

/*Common namespaces */
#define INVALID -1
#define CHAR_SIZE 8

/* Server related */
#define PORT 7002
#define maxNumOfConnections 64
#define dataSizeStdAmountOfDigits 5
#define defaultMaximumDataSize 256

/* ServerUser LOCATION */
#define NOT_AUTH -1
#define AT_MENU 0
#define AT_ORGANIZER 1
#define AT_TORRENT 2

/* ServerUser STATUS*/
#define CONNECTED 0
#define DISCONNECTED 1

const int defaultFamily = AF_INET6;
const int defaultSocketType = SOCK_STREAM;
const int defaultProtocol = getprotobyname("tcp")->p_proto;

void formatDataSizeString(std::string &data) {
    while(data.size()<dataSizeStdAmountOfDigits)
        data.insert(data.begin(), '0');
}

#endif

