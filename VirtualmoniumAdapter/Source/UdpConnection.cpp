//****************************************************************************
// Copyright © 2018 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2018-07-28.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "UdpConnection.hpp"
#include <cerrno>
#include <iostream>
#include <thread>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "Utilities.hpp"

UdpConnection::UdpConnection()
{}

UdpConnection::UdpConnection(const std::string& hostName,
                             const std::string& serviceName)
        : m_HostName(hostName),
          m_ServiceName(serviceName)
{}

UdpConnection::~UdpConnection()
{
    if (m_Socket != -1)
        close(m_Socket);
}

UdpConnection::operator bool() const
{
    return m_Socket != -1;
}

bool UdpConnection::connect(double timeoutSecs)
{
    auto startTime = high_resolution_clock::now();
    while (timeoutSecs == 0 || elapsedSeconds(startTime) < timeoutSecs)
    {
        if (connectImpl())
            return true;
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1s);
    }
    return false;
}

void UdpConnection::disconnect()
{
    if (m_Socket != -1)
    {
        close(m_Socket);
        m_Socket = -1;
    }
}

void UdpConnection::send(const void* data, size_t size)
{
    if (m_Socket != -1)
        send(data, size);
}

bool UdpConnection::connectImpl()
{
    if (m_Socket != -1)
        return false;

    if (m_HostName.empty() || m_ServiceName.empty())
        return false;

    addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = 0;
    hints.ai_flags = AI_ADDRCONFIG;
    struct addrinfo* firstAddrInfo = 0;
    int err = getaddrinfo(m_HostName.c_str(), m_ServiceName.c_str(),
                          &hints, &firstAddrInfo);
    if (err != 0)
    {
        std::clog << "Failed to resolve remote socket address (err="
                  << err << ")";
        return false;
    }

    std::string causeOfFailure;
    for (auto addrInfo = firstAddrInfo; addrInfo; addrInfo = addrInfo->ai_next)
    {
        m_Socket = socket(addrInfo->ai_family, addrInfo->ai_socktype,
                          addrInfo->ai_protocol);
        if (m_Socket == -1)
        {
            if (causeOfFailure.empty())
                causeOfFailure = "While opening socket: "
                                 + std::string(strerror(errno));
            continue;
        }
        if (::connect(m_Socket, addrInfo->ai_addr, addrInfo->ai_addrlen)
            == -1)
        {
            causeOfFailure = "While connecting socket: "
                             + std::string(strerror(errno));
            close(m_Socket);
            m_Socket = -1;
            continue;
        }
        return true;
    }
    std::clog << "Unable to connect. Cause: " << causeOfFailure << "\n";
    return false;
}
