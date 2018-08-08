//****************************************************************************
// Copyright © 2018 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2018-07-25.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <exception>
#include <string>
#include <phidget22.h>

class PhidgetError : public std::exception
{
public:
    PhidgetError(std::string msg);

    PhidgetError(PhidgetReturnCode code, std::string msg = std::string());

    const char* what() const noexcept override;
private:
    std::string m_Message;
};

#define CHECK_RESULT(code, errorMessage) \
    do { \
        auto phidget_result = (code); \
        if (phidget_result != EPHIDGET_OK) \
            throw PhidgetError(phidget_result, (errorMessage)); \
    } while (false)
