//****************************************************************************
// Copyright © 2018 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2018-07-25.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "VoltageRatioInput.hpp"
#include "PhidgetError.hpp"

VoltageRatioInput::VoltageRatioInput()
{
    CHECK_RESULT(PhidgetVoltageRatioInput_create(&m_Handle),
                 "failed to create voltage ratio input.");
}

VoltageRatioInput::~VoltageRatioInput()
{
    if (m_Handle)
    {
        Phidget_close((PhidgetHandle)m_Handle);
        PhidgetVoltageRatioInput_delete(&m_Handle);
    }
}

VoltageRatioInput::operator bool() const
{
    return m_Handle != nullptr;
}

VoltageRatioInput::operator PhidgetHandle() const
{
    return reinterpret_cast<PhidgetHandle>(m_Handle);
}

VoltageRatioInput::operator PhidgetVoltageRatioInputHandle() const
{
    return m_Handle;
}
