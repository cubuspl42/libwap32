#ifndef errors_h
#define errors_h

#include "wap32/errors.h"
#include "utils.h"
#include <array>
#include <exception>
#include <iostream>
#include <vector>

#ifdef _WIN32
#define BREAKPOINT DebugBreak();
#else
#define BREAKPOINT __asm__("int $3")
#endif

struct wap_error_context
{
    static std::vector<std::array<char, 128>> error_context_stack;
    template<typename... Args>
    wap_error_context(const char *format, Args... args)
    {
        error_context_stack.emplace_back();
        char *buffer = error_context_stack.back().data();
        constexpr size_t error_context_buffer_size = sizeof(decltype(error_context_stack)::value_type);
        wap_util_buffer_printf(buffer, error_context_buffer_size, format, args...);
    }
    ~wap_error_context();
};

void wap_err__print_full_context(const char *message);
template<typename... Args>
void wap_err__critical(const char *format, Args... args)
{
    std::cerr << "critical error:" << std::endl;
    char msg[128];
    wap_util_buffer_printf(msg, format, args...);
    wap_err__print_full_context(msg);
    BREAKPOINT;
}

#endif
