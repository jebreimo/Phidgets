#include <stddef.h>
#include "phidget22.h"

PhidgetReturnCode Phidget_close(PhidgetHandle phid)
{
    return EPHIDGET_OK;
}

PhidgetReturnCode Phidget_getChannel(PhidgetHandle phid, int *channel)
{
    return EPHIDGET_OK;
}

PhidgetReturnCode Phidget_getDeviceID(PhidgetHandle phid, Phidget_DeviceID *deviceID)
{
    return EPHIDGET_OK;
}

PhidgetReturnCode Phidget_getDeviceSerialNumber(PhidgetHandle phid, int32_t *deviceSerialNumber)
{
    return EPHIDGET_OK;
}

PhidgetReturnCode Phidget_getErrorDescription(PhidgetReturnCode errorCode, const char **errorString)
{
    *errorString = "Unknown error.";
    return EPHIDGET_OK;
}

PhidgetReturnCode Phidget_getHubPort(PhidgetHandle phid, int *hubPort)
{
    return EPHIDGET_OK;
}

PhidgetReturnCode Phidget_open(PhidgetHandle phid)
{
    return EPHIDGET_OK;
}

PhidgetReturnCode Phidget_openWaitForAttachment(PhidgetHandle phid, uint32_t timeoutMs)
{
    return EPHIDGET_OK;
}

PhidgetReturnCode Phidget_setChannel(PhidgetHandle phid, int channel)
{
    return EPHIDGET_OK;
}

PhidgetReturnCode Phidget_setDataInterval(PhidgetHandle phid, uint32_t di)
{
    return EPHIDGET_OK;
}

PhidgetReturnCode Phidget_setDeviceID(PhidgetHandle phid, Phidget_DeviceID deviceID)
{
    return EPHIDGET_OK;
}

PhidgetReturnCode Phidget_setDeviceSerialNumber(PhidgetHandle phid, int32_t deviceSerialNumber)
{
    return EPHIDGET_OK;
}

PhidgetReturnCode Phidget_setHubPort(PhidgetHandle phid, int hubPort)
{
    return EPHIDGET_OK;
}

PhidgetReturnCode Phidget_setIsHubPortDevice(PhidgetHandle phid, int isHubPortDevice)
{
    return EPHIDGET_OK;
}

PhidgetReturnCode Phidget_setOnAttachHandler(PhidgetHandle phid, Phidget_OnAttachCallback fptr, void *ctx)
{
    return EPHIDGET_OK;
}

PhidgetReturnCode Phidget_setOnDetachHandler(PhidgetHandle phid, Phidget_OnDetachCallback fptr, void *ctx)
{
    return EPHIDGET_OK;
}

PhidgetReturnCode Phidget_setOnErrorHandler(PhidgetHandle phid, Phidget_OnErrorCallback fptr, void *ctx)
{
    return EPHIDGET_OK;
}

PhidgetReturnCode PhidgetLog_enable(Phidget_LogLevel level, const char *dest)
{
    return EPHIDGET_OK;
}

PhidgetReturnCode PhidgetVoltageRatioInput_create(PhidgetVoltageRatioInputHandle *ch)
{
    return EPHIDGET_OK;
}

PhidgetReturnCode PhidgetVoltageRatioInput_delete(PhidgetVoltageRatioInputHandle *ch)
{
    return EPHIDGET_OK;
}

PhidgetReturnCode PhidgetVoltageRatioInput_getSensorValue(
    PhidgetVoltageRatioInputHandle ch,
    double *sensorValue)
{
    return EPHIDGET_OK;
}

PhidgetReturnCode PhidgetVoltageRatioInput_setDataInterval(
    PhidgetVoltageRatioInputHandle ch,
    uint32_t dataInterval)
{
    return EPHIDGET_OK;
}

PhidgetReturnCode PhidgetVoltageRatioInput_setOnVoltageRatioChangeHandler(
    PhidgetVoltageRatioInputHandle ch,
    PhidgetVoltageRatioInput_OnVoltageRatioChangeCallback fptr,
    void *ctx)
{
    return EPHIDGET_OK;
}

PhidgetReturnCode PhidgetVoltageRatioInput_setSensorValueChangeTrigger(
    PhidgetVoltageRatioInputHandle ch,
    double sensorValueChangeTrigger)
{
    return EPHIDGET_OK;
}

PhidgetReturnCode PhidgetVoltageRatioInput_setVoltageRatioChangeTrigger(
    PhidgetVoltageRatioInputHandle ch,
    double voltageRatioChangeTrigger)
{
    return EPHIDGET_OK;
}
