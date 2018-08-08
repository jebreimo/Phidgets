//****************************************************************************
// Copyright © 2018 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2018-07-28.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once

#include <cfloat>
#include <string>

class UdpConnection
{
public:
    UdpConnection();

    UdpConnection(const std::string& hostName,
                  const std::string& serviceName);

    UdpConnection(const UdpConnection&) = delete;

    UdpConnection(UdpConnection&&) noexcept = default;

    ~UdpConnection();

    UdpConnection& operator=(const UdpConnection&) = delete;

    UdpConnection& operator=(UdpConnection&&) noexcept = default;

    explicit operator bool() const;

    bool connect(double timeoutSecs = 0);

    void disconnect();

    void send(const void* data, size_t size);
private:
    bool connectImpl();

    std::string m_HostName;
    std::string m_ServiceName;
    int m_Socket = -1;
};
