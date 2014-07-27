#ifndef wap_errors_h
#define wap_errors_h

#include "common.h"

WAP_BEGIN_DECLS

typedef enum {
    WAP_OK = 0,
    WAP_ERROR = -1, // Generic error
    WAP_ENOMEMORY = -2, // Out of memory
    WAP_EFILE = -3, // File access failed
    WAP_EINVALIDDATA = -4, // Input data is invalid
} Wap32Error;

WAP_END_DECLS

#endif
