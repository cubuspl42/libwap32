#include "errors.h"
#include <iostream>

static const char *s_wap32_err_description;

const char *wap32_err_description()
{
    return s_wap32_err_description;
}

void wap32_err__set_description(const char *description)
{
    s_wap32_err_description = description;
}

void wap32_err__throw(int error_code, const char *description)
{
    wap32_err__set_description(description);
    throw Wap32Exception(error_code);
}

void wap32_err__warning(const char *description)
{
    std::cerr << description << std::endl;
}