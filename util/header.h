#ifndef ACEVPN_HEADER_H
#define ACEVPN_HEADER_H

/* External dependences */
#include <bits/stdc++.h>
#include <errno.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

/*Common namespaces */
#define INVALID -1
#define CHAR_SIZE 8

/* Server related */
#define PORT 7002
#define maxNumOfConnections 64
#define dataSizeStdAmountOfDigits 5
#define defaultMaximumDataSize 256

const int defaultFamily = AF_INET6;
const int defaultSocketType = SOCK_STREAM;
const int defaultProtocol = getprotobyname("tcp")->p_proto;

void formatDataSizeString(std::string &data) {
    while(data.size()<dataSizeStdAmountOfDigits)
        data.insert(data.begin(), '0');
}

#endif

