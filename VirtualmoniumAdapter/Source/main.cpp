#include <iostream>
#include <phidget22.h>
#include <mutex>
#include "Configuration.hpp"
#include "PhidgetError.hpp"
#include "VoltageRatioInput.hpp"
#include "FaderMessageQueue.hpp"

struct FaderData
{
    VoltageRatioInput handle;
    FaderMessageQueue* faderMessageQueue = nullptr;
    std::string name;
    double ratio = 0.0;
};

void CCONV onVoltageRatioChangeHandler(PhidgetVoltageRatioInputHandle handle,
                                       void* ctx, double voltageRatio)
{
    auto faderData = static_cast<FaderData*>(ctx);
    faderData->faderMessageQueue->sendMessage(faderData->name, voltageRatio);
}

void CCONV onErrorHandler(PhidgetHandle handle, void* ctx,
                          Phidget_ErrorEventCode errorCode,
                          const char* errorString)
{
    int serialNumber = -1;
    Phidget_getDeviceSerialNumber(handle, &serialNumber);

    int hubPortNumber = -1;
    Phidget_getHubPort(handle, &hubPortNumber);

    std::cerr << "Error: " << errorString
              << " (Serial number: " << serialNumber
              << " hub port: " << hubPortNumber << ")\n";
}

void initFader(const VoltageRatioInput& handle, int serialNumber,
               int hubPortNumber, void* ctx)
{
    //CHECK_RESULT(Phidget_setOnAttachHandler(handle, onAttachHandler, ctx),
    //             "failed to assign on attach handler");
    //CHECK_RESULT(Phidget_setOnDetachHandler(handle, onDetachHandler, ctx),
    //             "failed to assign on detach handler");
    CHECK_RESULT(Phidget_setOnErrorHandler(handle, onErrorHandler, ctx),
                 "failed to assign on error handler.");
    CHECK_RESULT(Phidget_setIsHubPortDevice(handle, 1),
                 "failed to make device a hub port device.");
    CHECK_RESULT(Phidget_setDeviceSerialNumber(handle, serialNumber),
                 "failed to assign device serial number.");
    CHECK_RESULT(Phidget_setIsHubPortDevice(handle, 1),
                 "failed to make the device a hub port device.");
    CHECK_RESULT(Phidget_setHubPort(handle, hubPortNumber),
                 "failed to assign device port number.");
    CHECK_RESULT(
            PhidgetVoltageRatioInput_setOnVoltageRatioChangeHandler(
                    handle, onVoltageRatioChangeHandler, ctx),
            "Failed to set voltage ratio change handler.");
}

int main(int argc, char* argv[])
{
    auto configFileName = argc == 2 ? argv[1] : "VirtualmoniumAdapter.cfg";

    try
    {
        auto configuration = readConfiguration(configFileName);

        if (configuration.faders.empty())
        {
            std::cout << "The configuration file doesn't define any faders.\n";
            return 1;
        }

        FaderMessageQueue messageQueue(configuration.receivingHost,
                                       configuration.receivingPort);
        std::vector<FaderData> faders(configuration.faders.size());

        PhidgetLog_enable(PHIDGET_LOG_INFO, nullptr);

        for (size_t i = 0; i < faders.size(); ++i)
        {
            faders[i].faderMessageQueue = &messageQueue;
            auto& handle = faders[i].handle;
            auto& faderDef = configuration.faders[i];
            initFader(handle, faderDef.serialNumber, faderDef.hubPort,
                      &faders[i]);
            PhidgetVoltageRatioInput_setVoltageRatioChangeTrigger(
                    handle, configuration.faderChangeValueTrigger);
            CHECK_RESULT(Phidget_openWaitForAttachment(handle, 5000),
                         "Unable to open device with serial number "
                         + std::to_string(faderDef.serialNumber)
                         + "and hub port number "
                         + std::to_string(faderDef.hubPort));
        }
        messageQueue.start();
    }
    catch (std::exception& ex)
    {
        std::cerr << "Exception: " << ex.what() << "\n\n";
        std::cerr << "Usage: " << argv[0] << " [configuration file].\n";
        return 1;
    }
    return 0;
}
