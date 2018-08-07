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

PhidgetReturnCode Phidget_getDeviceSerialNumber(PhidgetHandle phid, int32_t *deviceSerialNumber)
{
    return EPHIDGET_OK;
}

PhidgetReturnCode Phidget_getErrorDescription(PhidgetReturnCode errorCode, const char **errorString)
{
    return EPHIDGET_OK;
}

PhidgetReturnCode Phidget_getHubPort(PhidgetHandle phid, int *hubPort)
{
    return EPHIDGET_OK;
}

PhidgetReturnCode Phidget_setOnDetachHandler(PhidgetHandle phid, Phidget_OnDetachCallback fptr, void *ctx)
{
    return EPHIDGET_OK;
}

PhidgetReturnCode Phidget_setOnAttachHandler(PhidgetHandle phid, Phidget_OnAttachCallback fptr, void *ctx)
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
