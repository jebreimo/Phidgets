//****************************************************************************
// Copyright © 2018 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2018-07-28.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "FaderMessageQueue.hpp"

#include "Utilities.hpp"

FaderMessageQueue::FaderMessageQueue() = default;

void FaderMessageQueue::sendMessage(std::string name,
                                    std::vector<double> durations)
{
    std::lock_guard<std::mutex> lock(m_Mutex);
    m_PrintQueue.emplace(move(name), move(durations));
}

void FaderMessageQueue::start()
{
    using namespace std::chrono_literals;

    high_resolution_clock::time_point previousMessageTime;

    while (!m_Cancel)
    {
        if (m_Mutex.try_lock())
        {
            if (!m_PrintQueue.empty())
            {
                auto name = move(m_PrintQueue.front().first);
                auto durations = move(m_PrintQueue.front().second);
                m_PrintQueue.pop();
                m_Mutex.unlock();
                printf("\n%s", name.c_str());
                for (int i = 0; i < durations.size(); ++i)
                {
                    if (i % 20 == 0)
                        printf("\n");
                    printf("%3d ", int(durations[i] * 1000));
                }
            }
            else
            {
                m_Mutex.unlock();
                std::this_thread::sleep_for(0.1s);
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
