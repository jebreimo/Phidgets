//****************************************************************************
// Copyright © 2018 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2018-08-04.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include <iostream>
#include <csignal>
#include <cstdlib>
//#include <arpa/inet.h>
//#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>

constexpr size_t BUFSIZE = 1024;
volatile bool g_Stop = false;

void signalHandler(int signal)
{
    g_Stop = true;
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "usage: " << argv[0] << " <port number>\n";
        return 1;
    }

    auto port = strtol(argv[1], nullptr, 0);
    if (port == 0)
    {
        std::cerr << "Invalid port: " << argv[1] << "\n";
        return 1;
    }

    auto socketFD = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketFD < 0)
        throw std::runtime_error("Error opening socket");

    // setsockopt: Handy debugging trick that lets
    // us rerun the server immediately after we kill it;
    // otherwise we have to wait about 20 secs.
    // Eliminates "ERROR on binding: Address already in use" error.
    int optval = 1;
    setsockopt(socketFD, SOL_SOCKET, SO_REUSEADDR,
               &optval, sizeof(int));

    // build the server's Internet address
    sockaddr_in serverAddr; /* server's addr */
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(port);

    // bind: associate the parent socket with a port
    if (bind(socketFD, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) < 0)
    {
        std::cerr << "Unable to bind socket to port.\n";
        return 1;
    }

    std::signal(SIGINT, signalHandler);

    char buffer[BUFSIZE];
    while (!g_Stop)
    {
        memset(buffer, 0, BUFSIZE);
        sockaddr_in clientAddr;
        auto clientLen = socklen_t(sizeof(clientAddr));
        pollfd pollFD = {socketFD, POLLRDNORM, 0};
        if (poll(&pollFD, 1, 0) == 0)
        {
            timespec rqtp = {0, 1000};
            nanosleep(&rqtp, nullptr);
            continue;
        }
        auto n = recvfrom(socketFD, buffer, BUFSIZE, 0,
                          (sockaddr*)&clientAddr, &clientLen);
        std::cout << "Received " << n << " bytes.\n";
    }

    return 0;
}
