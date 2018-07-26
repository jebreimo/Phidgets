#include <iostream>
#include <stdexcept>
#include <phidget22.h>
#include <vector>
#include <array>
#include "PhidgetError.hpp"
#include "VoltageRatioInput.hpp"

struct PhidgetDeviceId
{
    PhidgetDeviceId() {}

    PhidgetDeviceId(int serial, int channel, int port)
            : serial(serial), channel(channel), port(port)
    {}

    explicit operator bool() const {return serial != -1 && channel != -1;}

    int serial = -1;
    int channel = -1;
    int port = -1;
};

std::ostream& operator<<(std::ostream& stream, const PhidgetDeviceId& id)
{
    stream << "Serial: " << id.serial
           << " Channel: " << id.channel
           << " Port: " << id.port;
    return stream;
}

struct PhidgetData
{
    std::vector<VoltageRatioInput> handles;
    std::vector<PhidgetDeviceId> devices;
};

PhidgetDeviceId getDeviceId(PhidgetHandle phid)
{
    int serial;
    if (Phidget_getDeviceSerialNumber(phid, &serial) != EPHIDGET_OK)
    {
        std::cerr << "failed to get device serial number\n";
        return {};
    }

    int channel;
    if (Phidget_getChannel(phid, &channel) != EPHIDGET_OK)
    {
        std::cerr << "failed to get device channel number\n";
        return {};
    }

    int port;
    if (Phidget_getHubPort(phid, &port) != EPHIDGET_OK)
    {
        std::cerr << "failed to get hub port\n";
        port = -1;
    }

    return {serial, channel, port};
}

void CCONV onAttachHandler(PhidgetHandle handle, void* ctx)
{
    auto data = static_cast<PhidgetData*>(ctx);
    auto deviceId = getDeviceId(handle);
    if (deviceId)
    {
        std::cout << "Attached: " << deviceId << "\n";
        data->devices.push_back(deviceId);
    }
}

void CCONV onDetachHandler(PhidgetHandle handle, void* ctx)
{
    //auto data = static_cast<PhidgetData*>(ctx);
    auto deviceId = getDeviceId(handle);
    if (deviceId)
    {
        std::cout << "Detached: " << deviceId << "\n";
    }
}

void CCONV onVoltageRatioChangeHandler(PhidgetVoltageRatioInputHandle handle,
                                       void* ctx, double voltageRatio)
{
    //auto data = static_cast<PhidgetData*>(ctx);
    //auto deviceId = getDeviceId(phid);
    //if (deviceId)
    //{
    //    std::cout << "Detached: " << deviceId << "\n";
    //}
}

void CCONV onErrorHandler(PhidgetHandle phid, void* ctx,
                          Phidget_ErrorEventCode errorCode,
                          const char* errorString)
{
    //auto data = static_cast<PhidgetData*>(ctx);
    auto deviceId = getDeviceId(phid);
    if (deviceId)
    {
        std::cout << "Error: " << errorString << " - " << deviceId << "\n";
    }
}

void initChannel(PhidgetHandle handle, void* ctx)
{
    PhidgetReturnCode res;

    CHECK_RESULT(Phidget_setOnAttachHandler(handle, onAttachHandler, ctx),
                 "failed to assign on attach handler");
    CHECK_RESULT(Phidget_setOnDetachHandler(handle, onDetachHandler, ctx),
                 "failed to assign on detach handler");
    CHECK_RESULT(Phidget_setOnErrorHandler(handle, onErrorHandler, ctx),
                 "failed to assign on error handler");
}

int main()
{
    try
    {
        PhidgetLog_enable(PHIDGET_LOG_INFO, nullptr);

        PhidgetData data;
        data.handles.resize(24);
        for (auto& handle : data.handles)
        {
            initChannel(handle, &data);
            CHECK_RESULT(
                    PhidgetVoltageRatioInput_setOnVoltageRatioChangeHandler(
                            handle, onVoltageRatioChangeHandler, nullptr),
                    "failed to set voltage ratio change handler: %s");
        }
    }
    catch (std::exception& ex)
    {
        std::cerr << "EXCEPTION: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}
