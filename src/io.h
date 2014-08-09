#ifndef io_h
#define io_h

#include "common.h"
#include "errors.h"

namespace wap {
    class InputStream
    {
    public:
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
                throw wap::Error(WAP_EINVALIDDATA);
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
    
    class OutputStream
    {
    public:
        inline OutputStream(char *buffer, size_t buffer_size, unsigned offset = 0)
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
        void write(T&& val, Tail&&... tail) {
            write_impl(std::forward<T>(val));
            write(std::forward<Tail>(tail)...);
        }
        template<typename T>
        void write_buffer(T *buffer, size_t buffer_size) {
            for(size_t i = 0; i < buffer_size; ++i)
                write_impl(buffer[i]);
        }
    private:
        inline void write() {}
        template<typename T>
        void write_impl(T val) {
            if(m_offset > m_buffer_size - sizeof(T)) {
                throw wap::Error(WAP_EINVALIDDATA);
            }
            memcpy(m_buffer + m_offset, &val, sizeof(T));
            m_offset += sizeof(T);
        }
        template<typename T, size_t size>
        void write_impl(T (&buffer)[size]) {
            write_buffer(buffer, size);
        }
        
        char *m_buffer;
        size_t m_buffer_size;
        unsigned m_offset = 0;
    };

}

#endif
