#ifndef errors_h
#define errors_h

#include "wap32/errors.h"
#include <exception>

struct Wap32Exception : public std::exception
{
    int error = WAP32_OK;
    inline Wap32Exception(int error_code)
    : error(error_code) {}
};

void wap32_err__set_description(const char *desc);
void wap32_err__throw(int error_code, const char *description);
void wap32_err__warning(const char *description);

#endif