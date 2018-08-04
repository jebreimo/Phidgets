//****************************************************************************
// Copyright © 2018 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2018-07-28.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "FaderMessageQueue.hpp"

#include "tinyosc.h"
#include "UdpConnection.hpp"
#include "Utilities.hpp"

FaderMessageQueue::FaderMessageQueue(std::string hostName,
                                     std::string serviceName)
        : m_HostName(move(hostName)),
          m_ServiceName(move(serviceName))
{}

void FaderMessageQueue::sendMessage(const std::string& name, double ratio)
{
    std::lock_guard<std::mutex> lock(m_Mutex);
    m_Messages[name] = ratio;
}

void FaderMessageQueue::start()
{
    using namespace std::chrono_literals;

    high_resolution_clock::time_point previousMessageTime;
    UdpConnection connection(m_HostName, m_ServiceName);
    char buffer[256];

    while (!m_Cancel)
    {
        if (!connection)
        {
            connection.connect();
            previousMessageTime = high_resolution_clock::now();
        }
        if (m_Mutex.try_lock())
        {
            try
            {
                if (!m_Messages.empty())
                {
                    for (auto& message : m_Messages)
                    {
                        int length = tosc_writeMessage(
                                buffer, sizeof(buffer),
                                message.first.c_str(),
                                "f",
                                float(message.second));
                        if (length > 0)
                            connection.send(buffer, size_t(length));
                    }
                    m_Messages.clear();
                    previousMessageTime = high_resolution_clock::now();
                    std::this_thread::sleep_for(1ms);
                }
                else
                {
                    auto elapsedTime = elapsedSeconds(previousMessageTime);
                    if (elapsedTime < 60)
                        std::this_thread::sleep_for(1us);
                    else
                        connection.disconnect();
                }
            }
            catch (...)
            {
                m_Mutex.unlock();
                throw;
            }
            m_Mutex.unlock();
        }
    }
}

void FaderMessageQueue::startInSeparateThread()
{
    m_Cancel = false;
    m_Thread = std::thread([this](){start();});
}

void FaderMessageQueue::stop()
{
    if (m_Thread.joinable())
    {
        m_Cancel = true;
        m_Thread.join();
    }
    else
    {
        m_Cancel = true;
    }
}
