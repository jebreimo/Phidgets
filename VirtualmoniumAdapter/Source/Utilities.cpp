//****************************************************************************
// Copyright © 2018 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2018-07-31.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Utilities.hpp"

double elapsedSeconds(high_resolution_clock::time_point& startTime)
{
    using namespace std::chrono;
    return duration<double>(high_resolution_clock::now() - startTime).count();
}
