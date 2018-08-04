//****************************************************************************
// Copyright © 2018 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2018-07-28.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>

class FaderMessageQueue
{
public:
    FaderMessageQueue(std::string hostName,
                      std::string serviceName);

    void sendMessage(const std::string& name, double ratio);

    void start();

    void startInSeparateThread();

    void stop();
private:
    std::mutex m_Mutex;
    std::unordered_map<std::string, double> m_Messages;
    std::thread m_Thread;
    std::string m_HostName;
    std::string m_ServiceName;
    volatile bool m_Cancel = false;
};
