#ifndef ACEVPN_HEADER_H
#define ACEVPN_HEADER_H

/* External dependences */
#include <string>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <vector>
#include <set>
#include <map>
#include <thread>
#include <chrono>
#include <thread>
#include <iostream>
#include <ostream>

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

#endif

