//****************************************************************************
// Copyright © 2018 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2018-07-25.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "PhidgetError.hpp"

PhidgetError::PhidgetError(std::string msg)
        : m_Message(move(msg))
{}

PhidgetError::PhidgetError(PhidgetReturnCode code,
                           std::string msg)
        : m_Message(move(msg))
{
    const char* phidgetMsg = nullptr;
    if (Phidget_getErrorDescription(code, &phidgetMsg) == EPHIDGET_OK)
    {
        if (!m_Message.empty())
        {
            m_Message += " (";
            m_Message += phidgetMsg;
            m_Message += ")";
        }
        else
            m_Message = phidgetMsg;
    }
}

const char* PhidgetError::what() const noexcept
{
    return m_Message.c_str();
}
