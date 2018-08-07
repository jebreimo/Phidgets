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
    m_NewValues[name] = ratio;
}

void FaderMessageQueue::start()
{
    using namespace std::chrono_literals;

    high_resolution_clock::time_point previousMessageTime;
    UdpConnection connection(m_HostName, m_ServiceName);
    char buffer[1024];

    while (!m_Cancel)
    {
        if (!connection)
        {
            connection.connect();
            previousMessageTime = high_resolution_clock::now();
        }
        if (m_Mutex.try_lock())
        {
            if (!m_NewValues.empty())
            {
                tosc_bundle bundle;
                tosc_writeBundle(&bundle, 0, buffer, sizeof(buffer));
                for (auto& message : m_NewValues)
                {
                    tosc_writeNextMessage(&bundle, message.first.c_str(),
                                          "f",
                                          float(message.second));
                    m_CurrentValues[message.first] = message.second;
                }
                m_NewValues.clear();
                m_Mutex.unlock();
                connection.send(buffer, tosc_getBundleLength(&bundle));
                previousMessageTime = high_resolution_clock::now();
            }
            else
            {
                m_Mutex.unlock();
                auto elapsedTime = elapsedSeconds(previousMessageTime);
                if (elapsedTime < 30)
                {
                    std::this_thread::sleep_for(1us);
                }
                else
                {
                    connection.disconnect();
                    std::lock_guard<std::mutex> lock(m_Mutex);
                    m_NewValues = m_CurrentValues;
                }
            }
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
