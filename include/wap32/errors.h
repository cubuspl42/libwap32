#ifndef wap32_errors_h
#define wap32_errors_h

#include "common.h"

WAP32_BEGIN_DECLS

typedef enum {
    WAP32_OK = 0,
    WAP32_ERROR = -1, // Generic error
    WAP32_ENOMEMORY = -2, // Out of memory
    WAP32_EFILE = -3, // File access failed
    WAP32_EINVALIDDATA = -4, // Input data is invalid
} Wap32Error;

WAP32_END_DECLS

#endif