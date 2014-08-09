#ifndef errors_h
#define errors_h

#include "wap32/errors.h"
#include "utils.h"
#include <array>
#include <exception>
#include <iostream>
#include <utility>
#include <vector>

#ifdef _WIN32
#define BREAKPOINT DebugBreak();
#else
#define BREAKPOINT __asm__("int $3")
#endif

namespace wap {
    class Error {
    public:
        inline Error(int error_code) : error_code_(error_code) {}
        inline int error_code() { return error_code_; }
        static Error from_zlib_error(int zlib_error_code);
    private:
        int error_code_;
    };
    
    template<class F, typename... Args>
    int handle_exceptions(F f, Args&&... args) {
        try {
            f(std::forward<Args>(args)...);
        } catch(wap::Error &error) {
            return error.error_code();
        } catch(std::bad_alloc&) {
            return WAP_ENOMEMORY;
        }
        return WAP_OK;
    }
}

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
