#include <array>
#include <iostream>
#include <stdexcept>
#include <unistd.h>
#include <vector>
#include <phidget22.h>
#include "PhidgetError.hpp"
#include "VoltageRatioInput.hpp"

struct PhidgetDeviceId
{
    PhidgetDeviceId() {}

    PhidgetDeviceId(int serial, int channel, int port)
            : serial(serial), channel(channel), port(port)
    {}

    explicit operator bool() const {return serial != -1 && channel != -1;}

    int serial = 0;
    int channel = 0;
    int port = 0;
};

std::array<PhidgetDeviceId, 24> deviceIds {{
        {495445, 0, 0},
        {495450, 0, 0},
        {497201, 0, 0},
        {497044, 0, 0},
        {495445, 0, 1},
        {495445, 0, 2},
        {495445, 0, 3},
        {495445, 0, 4},
        {495445, 0, 5},
        {495450, 0, 1},
        {495450, 0, 2},
        {495450, 0, 3},
        {495450, 0, 4},
        {495450, 0, 5},
        {497201, 0, 1},
        {497201, 0, 2},
        {497201, 0, 3},
        {497201, 0, 4},
        {497201, 0, 5},
        {497044, 0, 1},
        {497044, 0, 2},
        {497044, 0, 3},
        {497044, 0, 4},
        {497044, 0, 5}
}};

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

PhidgetDeviceId getDeviceId(PhidgetHandle handle)
{
    int serial;
    if (Phidget_getDeviceSerialNumber(handle, &serial) != EPHIDGET_OK)
    {
        std::cerr << "failed to get device serial number\n";
        return {};
    }

    int channel;
    if (Phidget_getChannel(handle, &channel) != EPHIDGET_OK)
    {
        std::cerr << "failed to get device channel number\n";
        return {};
    }

    int port;
    if (Phidget_getHubPort(handle, &port) != EPHIDGET_OK)
    {
        std::cerr << "failed to get hub port\n";
        port = -1;
    }

    return {serial, channel, port};
}

void setDeviceId(PhidgetHandle handle, const PhidgetDeviceId& id)
{
    CHECK_RESULT(Phidget_setDeviceSerialNumber(handle, id.serial),
                 "failed to assign device serial number");
    CHECK_RESULT(Phidget_setChannel(handle, id.channel),
                 "failed to assign device channel number");
    CHECK_RESULT(Phidget_setHubPort(handle, id.port),
                 "failed to assign device port number");
}

void CCONV onAttachHandler(PhidgetHandle handle, void* ctx)
{
    std::clog << __func__ << "\n";
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
    std::clog << __func__ << "\n";
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
    std::clog << __func__ << "\n";
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
    std::clog << __func__ << "\n";
    //auto data = static_cast<PhidgetData*>(ctx);
    auto deviceId = getDeviceId(phid);
    if (deviceId)
    {
        std::cout << "Error: " << errorString << " - " << deviceId << "\n";
    }
}

void initChannel(PhidgetHandle handle, void* ctx,
                 const PhidgetDeviceId& deviceId)
{
    PhidgetReturnCode res;

    CHECK_RESULT(Phidget_setOnAttachHandler(handle, onAttachHandler, ctx),
                 "failed to assign on attach handler");
    CHECK_RESULT(Phidget_setOnDetachHandler(handle, onDetachHandler, ctx),
                 "failed to assign on detach handler");
    CHECK_RESULT(Phidget_setOnErrorHandler(handle, onErrorHandler, ctx),
                 "failed to assign on error handler");
    CHECK_RESULT(Phidget_setIsHubPortDevice(handle, 1),
                 "failed to make device a hub port device");
    setDeviceId(handle, deviceId);
}

int main()
{
    try
    {
        PhidgetLog_enable(PHIDGET_LOG_DEBUG, nullptr);

        PhidgetData data;
        data.handles.resize(4);
        for (size_t i = 0; i < data.handles.size(); ++i)
        {
            auto& handle = data.handles[i];
            initChannel(handle, &data, deviceIds[i]);
            //CHECK_RESULT(
            //        PhidgetVoltageRatioInput_setOnVoltageRatioChangeHandler(
            //                handle, onVoltageRatioChangeHandler, nullptr),
            //        "failed to set voltage ratio change handler: %s");
            std::clog << "Waiting for device to attach.\n";
            //CHECK_RESULT(Phidget_openWaitForAttachment(handle, 5000),
            //             "device failed to attach");
            CHECK_RESULT(Phidget_open(handle),
                         "device failed to attach");
            std::clog << "Device attached.\n";
        }
        usleep(10 * 1000 * 1000);
    }
    catch (std::exception& ex)
    {
        std::cerr << "EXCEPTION: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}
