#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <phidget22.h>
#include "tinyosc.h"

#ifndef _WIN32
    #include <unistd.h>
#else
    #include <Windows.h>
#endif

typedef struct FaderId_s
{
    int channelNo;
    int serialNo;
    int hubPort;
} FaderId;

FaderId g_FaderIds[] = {
        {1,  497201, 6},
        {2,  497201, 2},
        {3,  497201, 4},
        {4,  497201, 3},
        {5,  497201, 5},
        {6,  497201, 1},
        {7,  495445, 1},
        {8,  495445, 2},
        {9,  495445, 3},
        {10, 495445, 6},
        {11, 495445, 4},
        {12, 495445, 5},
        {13, 495450, 4},
        {14, 495450, 6},
        {15, 495450, 3},
        {16, 495450, 2},
        {17, 495450, 1},
        {18, 495450, 5},
        {19, 497044, 6},
        {20, 497044, 3},
        {21, 497044, 4},
        {22, 497044, 1},
        {23, 497044, 5},
        {24, 497044, 2}};

#define FADER_COUNT (sizeof(g_FaderIds) / sizeof(*g_FaderIds))

double g_FaderRatios[FADER_COUNT];

#define THRESHOLD 0.002

static void CCONV ssleep(int);

static void CCONV onAttachHandler(PhidgetHandle phid, void* ctx)
{
    PhidgetReturnCode res;
    int hubPort;
    int channel;
    int serial;

    res = Phidget_getDeviceSerialNumber(phid, &serial);
    if (res != EPHIDGET_OK)
    {
        fprintf(stderr, "failed to get device serial number\n");
        return;
    }

    res = Phidget_getChannel(phid, &channel);
    if (res != EPHIDGET_OK)
    {
        fprintf(stderr, "failed to get channel number\n");
        return;
    }

    res = Phidget_getHubPort(phid, &hubPort);
    if (res != EPHIDGET_OK)
    {
        fprintf(stderr, "failed to get hub port\n");
        hubPort = -1;
    }

    if (hubPort == -1)
        printf("channel %d on device %d attached\n", channel, serial);
    else
        printf("channel %d on device %d hub port %d attached\n", channel, serial, hubPort);
}

static void CCONV onDetachHandler(PhidgetHandle phid, void* ctx)
{
    PhidgetReturnCode res;
    int hubPort;
    int channel;
    int serial;

    res = Phidget_getDeviceSerialNumber(phid, &serial);
    if (res != EPHIDGET_OK)
    {
        fprintf(stderr, "failed to get device serial number\n");
        return;
    }

    res = Phidget_getChannel(phid, &channel);
    if (res != EPHIDGET_OK)
    {
        fprintf(stderr, "failed to get channel number\n");
        return;
    }

    res = Phidget_getHubPort(phid, &hubPort);
    if (res != EPHIDGET_OK)
        hubPort = -1;

    if (hubPort != -1)
        printf("channel %d on device %d detached\n", channel, serial);
    else
        printf("channel %d on device %d hub port %d detached\n", channel, hubPort, serial);
}

static void CCONV errorHandler(PhidgetHandle phid,
                               void* ctx,
                               Phidget_ErrorEventCode errorCode,
                               const char* errorString)
{

    fprintf(stderr, "Error: %s (%d)\n", errorString, errorCode);
}

static void CCONV onVoltageRatioChangeHandler(
        PhidgetVoltageRatioInputHandle ch,
        void* ctx,
        double ratio)
{
    printf("Voltage Ratio Changed: %.3g\n", ratio);
}

/*
 * Creates and initializes the channel.
 */
static PhidgetReturnCode CCONV initChannel(PhidgetHandle ch)
{
    PhidgetReturnCode res;

    res = Phidget_setOnAttachHandler(ch, onAttachHandler, NULL);
    if (res != EPHIDGET_OK)
    {
        fprintf(stderr, "failed to assign on attach handler\n");
        return (res);
    }

    res = Phidget_setOnDetachHandler(ch, onDetachHandler, NULL);
    if (res != EPHIDGET_OK)
    {
        fprintf(stderr, "failed to assign on detach handler\n");
        return (res);
    }

    res = Phidget_setOnErrorHandler(ch, errorHandler, NULL);
    if (res != EPHIDGET_OK)
    {
        fprintf(stderr, "failed to assign on error handler\n");
        return (res);
    }

    /*
     * Please review the Phidget22 channel matching documentation for details on the device
     * and class architecture of Phidget22, and how channels are matched to device features.
     */

    /*
     * Specifies the serial number of the device to attach to.
     * For VINT devices, this is the hub serial number.
     *
     * The default is any device.
     */
    // Phidget_setDeviceSerialNumber(ch, <YOUR DEVICE SERIAL NUMBER>);

    /*
     * For VINT devices, this specifies the port the VINT device must be plugged into.
     *
     * The default is any port.
     */
    // Phidget_setHubPort(ch, 0);

    /*
     * Specifies that the channel should only match a VINT hub port.
     * The only valid channel id is 0.
     *
     * The default is 0 (false), meaning VINT hub ports will never match
     */
    // Phidget_setIsHubPortDevice(ch, 1);

    /*
     * Specifies which channel to attach to.  It is important that the channel of
     * the device is the same class as the channel that is being opened.
     *
     * The default is any channel.
     */
    // Phidget_setChannel(ch, 0);

    /*
     * In order to attach to a network Phidget, the program must connect to a Phidget22 Network Server.
     * In a normal environment this can be done automatically by enabling server discovery, which
     * will cause the client to discovery and connect to available servers.
     *
     * To force the channel to only match a network Phidget, set remote to 1.
     */
    // PhidgetNet_enableServerDiscovery(PHIDGETSERVER_DEVICE);
    // Phidget_setIsRemote(ch, 1);

    return (EPHIDGET_OK);
}


int initVoltageRatioInputH(PhidgetVoltageRatioInputHandle ch)
{
    PhidgetReturnCode res;
    const char* errs;
    Phidget_DeviceID deviceID;

    /*
     * Enable logging to stdout
     */
    PhidgetLog_enable(PHIDGET_LOG_INFO, NULL);

    res = PhidgetVoltageRatioInput_create(&ch);
    if (res != EPHIDGET_OK)
    {
        fprintf(stderr, "failed to create voltage ratio input channel\n");
        return 1;
    }

    res = initChannel((PhidgetHandle)ch);
    if (res != EPHIDGET_OK)
    {
        Phidget_getErrorDescription(res, &errs);
        fprintf(stderr, "failed to initialize channel:%s\n", errs);
        return 1;
    }

    res = PhidgetVoltageRatioInput_setOnVoltageRatioChangeHandler(ch,
                                                                  onVoltageRatioChangeHandler,
                                                                  NULL);
    if (res != EPHIDGET_OK)
    {
        Phidget_getErrorDescription(res, &errs);
        fprintf(stderr, "failed to set voltage ratio change handler: %s\n",
                errs);
        goto done;
    }

    /*
     * Open the channel synchronously: waiting a maximum of 5 seconds.
     */
    res = Phidget_openWaitForAttachment((PhidgetHandle)ch, 5000);
    if (res != EPHIDGET_OK)
    {
        if (res == EPHIDGET_TIMEOUT)
        {
            printf("Channel did not attach after 5 seconds: please check that the device is attached\n");
        }
        else
        {
            Phidget_getErrorDescription(res, &errs);
            fprintf(stderr, "failed to open channel:%s\n", errs);
        }
        goto done;
    }

    res = Phidget_getDeviceID((PhidgetHandle)ch, &deviceID);
    if (res != EPHIDGET_OK)
    {
        fprintf(stderr, "failed to get device ID\n");
        return 2;
    }

    if (deviceID == PHIDID_1046)
    {
        printf("setting bridge enabled\n");
        res = PhidgetVoltageRatioInput_setBridgeEnabled(ch, 1);
        if (res != EPHIDGET_OK)
        {
            fprintf(stderr, "failed to set bridge enabled\n");
            return 3;
        }
    }
done:
    Phidget_close((PhidgetHandle)ch);
    PhidgetVoltageRatioInput_delete(&ch);
}

int main(int argc, char** argv)
{
    memset(g_FaderRatios, 0, sizeof(g_FaderRatios));

    PhidgetVoltageRatioInputHandle faderHandles[FADER_COUNT];

    for (int i = 0; i < FADER_COUNT; ++i)
    {
        initVoltageRatioInputH(faderHandles[i]);
    }

    //PhidgetReturnCode res;
    //const char* errs;
    //Phidget_DeviceID deviceID;
    //
    ///*
    // * Enable logging to stdout
    // */
    //PhidgetLog_enable(PHIDGET_LOG_INFO, NULL);
    //
    //res = PhidgetVoltageRatioInput_create(&ch);
    //if (res != EPHIDGET_OK)
    //{
    //    fprintf(stderr, "failed to create voltage ratio input channel\n");
    //    exit(1);
    //}
    //
    //res = initChannel((PhidgetHandle)ch);
    //if (res != EPHIDGET_OK)
    //{
    //    Phidget_getErrorDescription(res, &errs);
    //    fprintf(stderr, "failed to initialize channel:%s\n", errs);
    //    exit(1);
    //}
    //
    //res = PhidgetVoltageRatioInput_setOnVoltageRatioChangeHandler(
    //        ch,
    //        onVoltageRatioChangeHandler,
    //        NULL);
    //if (res != EPHIDGET_OK)
    //{
    //    Phidget_getErrorDescription(res, &errs);
    //    fprintf(stderr, "failed to set voltage ratio change handler: %s\n",
    //            errs);
    //    goto done;
    //}
    //
    ///*
    // * Open the channel synchronously: waiting a maximum of 5 seconds.
    // */
    //res = Phidget_openWaitForAttachment((PhidgetHandle)ch, 5000);
    //if (res != EPHIDGET_OK)
    //{
    //    if (res == EPHIDGET_TIMEOUT)
    //    {
    //        printf("Channel did not attach after 5 seconds: please check that the device is attached\n");
    //    }
    //    else
    //    {
    //        Phidget_getErrorDescription(res, &errs);
    //        fprintf(stderr, "failed to open channel:%s\n", errs);
    //    }
    //    goto done;
    //}
    //
    //res = Phidget_getDeviceID((PhidgetHandle)ch, &deviceID);
    //if (res != EPHIDGET_OK)
    //{
    //    fprintf(stderr, "failed to get device ID\n");
    //    return 2;
    //}
    //
    //if (deviceID == PHIDID_1046)
    //{
    //    printf("setting bridge enabled\n");
    //    res = PhidgetVoltageRatioInput_setBridgeEnabled(ch, 1);
    //    if (res != EPHIDGET_OK)
    //    {
    //        fprintf(stderr, "failed to set bridge enabled\n");
    //        return 3;
    //    }
    //}

    printf("Gathering data for 10 seconds...\n");
    ssleep(10);

done:

    Phidget_close((PhidgetHandle)ch);
    PhidgetVoltageRatioInput_delete(&ch);

    exit(res);
}

static void CCONV ssleep(int tm)
{
    #ifdef _WIN32
    Sleep(tm * 1000);
    #else
    sleep(tm);
    #endif
}
