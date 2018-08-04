//****************************************************************************
// Copyright © 2018 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2018-07-27.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "Configuration.hpp"

#include <Yson/JsonReader.hpp>
#include "PhidgetError.hpp"

std::string getFileLocation(Yson::Reader& reader)
{
    auto result = reader.fileName();
    result += ":";
    result += std::to_string(reader.lineNumber());
    result += ":";
    result += std::to_string(reader.columnNumber());
    return result;
}

FaderDefinition readFaderDefinition(Yson::Reader& reader)
{
    FaderDefinition faderDefinition;
    reader.enter();
    while (reader.nextKey())
    {
        auto key = Yson::read<std::string>(reader);
        reader.nextValue();
        if (key == "SerialNumber")
            reader.read(faderDefinition.serialNumber);
        else if (key == "HubPort")
            reader.read(faderDefinition.hubPort);
        else if (key == "Name")
            reader.read(faderDefinition.name);
        else
            throw PhidgetError(
                    "Unknown key in configuration file: " + key + " ("
                    + getFileLocation(reader) + ")");
    }
    reader.leave();
    return faderDefinition;
}

Configuration readConfiguration(const std::string& fileName)
{
    Configuration configuration;
    auto reader = Yson::makeReader(fileName);
    while (reader->nextValue())
    {
        reader->enter();
        while (reader->nextKey())
        {
            auto key = Yson::read<std::string>(*reader);
            reader->nextValue();
            if (key == "ReceivingHost")
            {
                reader->read(configuration.receivingHost);
            }
            else if (key == "ReceivingPort")
            {
                reader->read(configuration.receivingPort);
            }
            else if (key == "MinimumFaderValueChange")
            {
                reader->read(configuration.minimumFaderValueChange);
            }
            else if (key == "MinimumDataInterval")
            {
                reader->read(configuration.dataInterval);
            }
            else if (key == "MaximumDataInterval")
            {
                reader->read(configuration.maximumDataInterval);
                configuration.maximumDataInterval /= 1000;
            }
            else if (key == "Faders")
            {
                reader->enter();
                while (reader->nextValue())
                    configuration.faders.push_back(
                            readFaderDefinition(*reader));
                reader->leave();
            }
            else
            {
                throw PhidgetError(
                        "Unknown key in configuration file: " + key + " ("
                        + getFileLocation(*reader) + ")");
            }
        }
        reader->leave();
    }
    return configuration;
}
