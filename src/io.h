#ifndef io_h
#define io_h

#include "common.h"
#include <cassert>
#include <string.h>
#include <utility>

class InputStream
{
public:
    inline InputStream(const char *buffer, size_t buffer_size)
    : m_buffer(buffer)
    , m_offset(0)
    , m_size(buffer_size) {}
    inline void set_offset(unsigned offset) {
        m_offset = offset;
    }
    template<typename T, typename... Tail>
    void read(T&& val, Tail&&... tail) {
        read_impl(std::forward<T>(val));
        return read(std::forward<Tail>(tail)...);
    }
private:
    inline void read() {}
    template<typename T>
    void read_impl(T &val) {
        if(m_size - m_offset < sizeof(T))
            wap32_err__throw(WAP32_EINVALIDDATA, "Unexpected end of buffer");
        memcpy(&val, m_buffer + m_offset, sizeof(T));
        m_offset += sizeof(T);
    }
    template<typename T>
    void read_impl(const T &constant) {
        T tmp {};
        if(tmp != constant)
            wap32_err__warning("Value doesn't equal expected constant");
        read_impl(tmp);
    }
    template<typename T, size_t size>
    void read_impl(T (&buf)[size]) {
        for(int i=0; i<size; ++i)
            read_impl(buf[i]);
    }
    
    const char *m_buffer;
    unsigned m_offset;
    size_t m_size;
};

#endif
