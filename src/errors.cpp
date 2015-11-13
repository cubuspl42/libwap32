#include "errors.h"

#include <zlib.h>

#include <cassert>
#include <iostream>

namespace wap {
Error Error::from_zlib_error(int zlib_error_code) {
    int error_code;
    switch (zlib_error_code) {
    case Z_BUF_ERROR:
    case Z_DATA_ERROR:
        error_code = WAP_EINVALIDDATA;
        break;
    case Z_MEM_ERROR:
        error_code = WAP_ENOMEMORY;
        break;
    default:
        error_code = WAP_ERROR;
    }
    return Error(error_code);
}
}
