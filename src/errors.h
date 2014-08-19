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

#endif
