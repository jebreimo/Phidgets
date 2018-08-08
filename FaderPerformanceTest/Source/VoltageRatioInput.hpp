//****************************************************************************
// Copyright © 2018 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2018-07-25.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <cstddef>
#include <phidget22.h>

class VoltageRatioInput
{
public:
    VoltageRatioInput();

    ~VoltageRatioInput();

    explicit operator bool() const;

    operator PhidgetHandle() const;

    operator PhidgetVoltageRatioInputHandle() const;
private:
    PhidgetVoltageRatioInputHandle m_Handle = nullptr;
};


