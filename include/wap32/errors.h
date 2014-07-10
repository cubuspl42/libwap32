#ifndef wap32_errors_h
#define wap32_errors_h

#include "common.h"

enum {
    WAP32_OK = 0,
    WAP32_ERROR = -1, // Generic error
    WAP32_ENOMEMORY = -2, // Out of memory
    WAP32_ENOTFOUND = -3, // Object could not be found
    WAP32_EINVALIDDATA = -4, // Input data is invalid
};

// Get last error description if available, NULL otherwise.
WAP32_API const char *wap32_err_description();

#endif