#ifndef ERRORTYPE_H
#define ERRORTYPE_H

#include <Arduino.h>

enum class ErrorType
{
    OK,
    FS_INIT,
    FS_WRITE_FILE,
    FS_READ_FILE,
    CONFIG_MISSING,
    CONFIG_INVALID,
    CONFIG_NO_WIFI_CRED,
    WIFI_FAILED,
    MDNS_FAILED,
    EMPTY,
    OUT_OF_RANGE,
    TEMP_OFFSET_FAILED,
    START_MEASUREMENT_FAILED,
    UNKNOWN,
    COUNT,
};

ErrorType operator||(ErrorType lhs, ErrorType rhs);
ErrorType& operator|=(ErrorType& lhs, ErrorType rhs);
void printError(ErrorType error);

#endif