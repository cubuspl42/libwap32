#ifndef io_h
#define io_h

#include "common.h"
#include "errors.h"
#include "utils.h"

#include <algorithm>
#include <fstream>
#include <type_traits>

namespace wap {
    namespace detail {
        template<bool Output>
        struct BasicStreamHelper {
            static inline void memcpy(char *to, const char *from, size_t size) {
                ::memcpy(to, from, size);
            }
        };
        template<>
        struct BasicStreamHelper<true> {
            static void memcpy(const char *from, char *to, size_t size) {
                ::memcpy(to, from, size);
            }
        };
        template<bool Output>
        class BasicStream {
            typedef typename std::conditional<Output, char*, const char*>::type buffer_ptr_t;
        protected:
            inline BasicStream(buffer_ptr_t buffer, size_t buffer_size, unsigned offset = 0)
            : m_buffer(buffer)
            , m_buffer_size(buffer_size)
            , m_offset(offset) {}
            template<typename T, typename... Tail>
            void read_write(T&& val, Tail&&... tail) {
                read_write_impl(std::forward<T>(val));
                read_write(std::forward<Tail>(tail)...);
            }
            template<typename T>
            void read_write_buffer(T *buffer, size_t buffer_size) {
                for(size_t i = 0; i < buffer_size; ++i)
                    read_write_impl(buffer[i]);
            }
        public:
            inline void seek(unsigned offset) {
                m_offset = offset;
            }
            inline unsigned tell() {
                return m_offset;
            }
        private:
            inline void read_write() {}
            template<typename T>
            void read_write_impl(const T &val) {
                T tmp = val;
                read_write_impl(tmp);
                if(!Output && tmp != val)
                    throw Error(WAP_EINVALIDDATA);
            }
            template<typename T>
            void read_write_impl(T &val) {
                if(m_offset > m_buffer_size - sizeof(T)) {
                    throw wap::Error(WAP_EINVALIDDATA);
                }
                char *val_ptr = reinterpret_cast<char*>(&val);
                BasicStreamHelper<Output>::memcpy(val_ptr, m_buffer + m_offset, sizeof(T));
                if(wap::system_is_big_endian())
                    std::reverse(val_ptr, val_ptr + sizeof(T));
                m_offset += sizeof(T);
            }
            template<typename T, size_t size>
            void read_write_impl(T (&buffer)[size]) {
                read_write_buffer(buffer, size);
            }
            
            buffer_ptr_t m_buffer;
            size_t m_buffer_size;
            unsigned m_offset = 0;
            
        };
    }
    
    class InputStream : public detail::BasicStream<false>
    {
    public:
        inline InputStream(const char *buffer, size_t buffer_size, unsigned offset = 0)
        : BasicStream(buffer, buffer_size, offset) { }
        template<typename... Args>
        void read(Args&&... args) {
            read_write(std::forward<Args>(args)...);
        }
        template<typename T>
        void read_buffer(T *buffer, size_t buffer_size) {
            read_write_buffer(buffer, buffer_size);
        }
    };
    
    class OutputStream : public detail::BasicStream<true>
    {
    public:
        inline OutputStream(char *buffer, size_t buffer_size, unsigned offset = 0)
        : BasicStream(buffer, buffer_size, offset) {}
        template<typename... Args>
        void write(Args&&... args) {
            read_write(std::forward<Args>(args)...);
        }
        template<typename T>
        void write_buffer(T *buffer, size_t buffer_size) {
            read_write_buffer(buffer, buffer_size);
        }
    };
    
    std::vector<char> read_whole_file(const char *file_path);
}

#endif
