# 

## About the application

Project Bird is an alternative to common aplications of modern world. As exposed by Edward Snowden, some countries invest heavily in international espionage, which is done through backdoors guaranteed by non-transparent legal agreements between private companies and the government. Since its creation, Project Bird has guaranteed a virtual private network of communication between the group of logged-in users, ensuring through open code and constant code inspection that no backdoor is opened for eavesdropping.

### Setup

Download the application

    git clone https://github.com/Earqee/project-bird

Move to folder ./main and create the server application on the desired computer through

    ./server

Now, not necessarily on the same computer, go to this same repository and begin the client application through

    ./client <server ipv6 address>

For example, "./client ::1" and "./client 2804:14d:1283:8e7c:a947:38d6:f03d:9e48" are valid inputs.

#### Compiling

Firstly, install the dependencies [libc6-dev](https://packages.debian.org/search?keywords=libc6-dev), [gcc](https://packages.debian.org/search?keywords=gcc) and [g++](https://packages.debian.org/search?keywords=g%2B%2B) or just install the build-essential package.

    sudo apt-get install build-essential 

For server compiling, move to folder ./main and compile through [g++](https://packages.debian.org/search?keywords=g%2B%2B) using

    g++ -pthread applicationserver.cpp -o server

For client compiling, in the same repository should work also

    g++ -pthread applicationclient.cpp -o client

## Application summary

## Security 

## Protocols

## License


