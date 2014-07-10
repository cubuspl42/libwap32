#ifndef io_h
#define io_h

#include "common.h"
#include <cassert>
#include <exception>
#include <string.h>
#include <utility>

class InputStream
{
public:
    struct EndOfBuffer : std::exception {};
    
    inline InputStream(const char *buffer, size_t buffer_size)
    : m_buffer(buffer)
    , m_size(buffer_size)
    , m_offset(0) {}
    template<class Containter> InputStream(const Containter &c)
    : m_buffer(c.data())
    , m_size(c.size())
    , m_offset(0) {}
    inline void set_offset(unsigned offset) {
        m_offset = offset;
    }
    template<typename T, typename... Tail>
    void read(T&& val, Tail&&... tail) {
        read_impl(std::forward<T>(val));
        read(std::forward<Tail>(tail)...);
    }
private:
    inline void read() {}
    template<typename T>
    void read_impl(T &val) {
        if(m_size - m_offset < sizeof(T))
            throw EndOfBuffer();
        memcpy(&val, m_buffer + m_offset, sizeof(T));
        m_offset += sizeof(T);
    }
    template<typename T>
    void read_impl(const T &constant) {
        T tmp {};
        read_impl(tmp);
    }
    template<typename T, size_t size>
    void read_impl(T (&buf)[size]) {
        for(int i=0; i<size; ++i)
            read_impl(buf[i]);
    }
    
    const char *m_buffer;
    size_t m_size;
    unsigned m_offset;
};

#endif
