#include "errors.h"
#include <cassert>
#include <iostream>

std::vector<std::array<char, 128>> Wap32ErrorContext::error_context_stack;
//char s_error_context_description[2048];
//char *s_error_description;

Wap32ErrorContext::~Wap32ErrorContext()
{
    error_context_stack.pop_back();
}

void wap32_err__print_full_context(const char *message)
{
    auto &stack = Wap32ErrorContext::error_context_stack;
    size_t n = stack.size();
    for(size_t i = 0; i <= n; ++i) {
        for(int j=0; j<i; ++j)
            std::cerr << "    ";
        if(i < n)
            std::cerr << "when " << stack[i].data() << std::endl;
        else std::cerr << message << std::endl;
    }
}