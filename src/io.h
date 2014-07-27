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
    
    inline InputStream(const char *buffer, size_t buffer_size, unsigned offset = 0)
    : m_buffer(buffer)
    , m_buffer_size(buffer_size)
    , m_offset(offset) {}
    inline void seek(unsigned offset) {
        m_offset = offset;
    }
    inline unsigned tell() {
        return m_offset;
    }
    template<typename T, typename... Tail>
    void read(T&& val, Tail&&... tail) {
        read_impl(std::forward<T>(val));
        read(std::forward<Tail>(tail)...);
    }
    template<typename T>
    void read_buffer(T *buffer, size_t buffer_size) {
        for(int i = 0; i < buffer_size; ++i)
            read_impl(buffer[i]);
    }
private:
    inline void read() {}
    template<typename T>
    void read_impl(T &val) {
        if(m_offset > m_buffer_size - sizeof(T)) {
            wap_err__critical("unexpected end of input buffer");
            throw EndOfBuffer();
        }
        memcpy(&val, m_buffer + m_offset, sizeof(T));
        m_offset += sizeof(T);
    }
    template<typename T>
    void read_impl(const T &constant) {
        T tmp {};
        read_impl(tmp);
    }
    template<typename T, size_t size>
    void read_impl(T (&buffer)[size]) {
        read_buffer(buffer, size);
    }
    
    const char *m_buffer;
    size_t m_buffer_size;
    unsigned m_offset = 0;
};

#endif
