//****************************************************************************
// Copyright © 2018 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2018-07-27.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <string>
#include <vector>

struct FaderDefinition
{
    FaderDefinition() = default;

    int serialNumber = 0;
    int hubPort = 0;
    std::string name;
};

struct Configuration
{
    Configuration() = default;

    std::vector <FaderDefinition> faders;
    double faderChangeValueTrigger = 0.002;
    std::string receivingHost = "localhost";
    std::string receivingPort = "8750";
};

Configuration readConfiguration(const std::string& fileName);
