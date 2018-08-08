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
#include <queue>

class FaderMessageQueue
{
public:
    FaderMessageQueue();

    void sendMessage(std::string name, std::vector<double> durations);

    void start();

    void startInSeparateThread();

    void stop();
private:
    std::mutex m_Mutex;
    std::queue<std::pair<std::string, std::vector<double>>> m_PrintQueue;
    std::thread m_Thread;
    volatile bool m_Cancel = false;
};
