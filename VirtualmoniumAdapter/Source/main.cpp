#include <cmath>
#include <csignal>
#include <iostream>
#include <mutex>
#include <phidget22.h>
#include "Configuration.hpp"
#include "FaderMessageQueue.hpp"
#include "PhidgetError.hpp"
#include "VoltageRatioInput.hpp"
#include "Utilities.hpp"

struct FaderData
{
    VoltageRatioInput handle;
    FaderMessageQueue* faderMessageQueue = nullptr;
    std::string name;
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
    CHECK_RESULT(Phidget_setOnErrorHandler(handle, onErrorHandler, ctx),
                 "failed to assign on error handler.");
    CHECK_RESULT(Phidget_setDeviceSerialNumber(handle, serialNumber),
                 "failed to assign device serial number.");
    CHECK_RESULT(Phidget_setIsHubPortDevice(handle, 1),
                 "failed to make the device a hub port device.");
    CHECK_RESULT(Phidget_setHubPort(handle, hubPortNumber),
                 "failed to assign device port number.");
    CHECK_RESULT(PhidgetVoltageRatioInput_setOnVoltageRatioChangeHandler(
                     handle, onVoltageRatioChangeHandler, ctx),
                 "Failed to set voltage ratio change handler.");
}

std::unique_ptr<FaderMessageQueue> messageQueue;

void signalHandler(int signal)
{
    messageQueue->stop();
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

        messageQueue = std::make_unique<FaderMessageQueue>(
                configuration.receivingHost,
                configuration.receivingPort);

        std::vector<FaderData> faders(configuration.faders.size());

        PhidgetLog_enable(PHIDGET_LOG_INFO, nullptr);

        for (size_t i = 0; i < faders.size(); ++i)
        {
            faders[i].faderMessageQueue = messageQueue.get();
            auto& handle = faders[i].handle;
            auto& faderDef = configuration.faders[i];
            faders[i].name = faderDef.name;
            initFader(handle, faderDef.serialNumber, faderDef.hubPort,
                      &faders[i]);
            CHECK_RESULT(Phidget_openWaitForAttachment(handle, 5000),
                         "Unable to open device with serial number "
                         + std::to_string(faderDef.serialNumber)
                         + "and hub port number "
                         + std::to_string(faderDef.hubPort));
            Phidget_setDataInterval(handle, configuration.dataInterval);
            PhidgetVoltageRatioInput_setVoltageRatioChangeTrigger(
                    handle, configuration.minimumFaderValueChange);
        }

        std::signal(SIGINT, signalHandler);

        messageQueue->start();
    }
    catch (std::exception& ex)
    {
        std::cerr << "Exception: " << ex.what() << "\n\n";
        std::cerr << "Usage: " << argv[0] << " [configuration file].\n";
        return 1;
    }
    return 0;
}
