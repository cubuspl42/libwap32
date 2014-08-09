#ifndef wap_pid_h
#define wap_pid_h

#include "common.h"
#include "buffer.h"

#include <stdint.h>

WAP_BEGIN_DECLS

enum {
    WAP_PID_FLAG_TRANSPARENCY       = 1 << 0,
    WAP_PID_FLAG_VIDEO_MEMORY       = 1 << 1,
    WAP_PID_FLAG_SYSTEM_MEMORY      = 1 << 2,
    WAP_PID_FLAG_MIRROR             = 1 << 3,
    WAP_PID_FLAG_INVERT             = 1 << 4,
    WAP_PID_FLAG_COMPRESSION        = 1 << 5,
    WAP_PID_FLAG_LIGHTS             = 1 << 6,
    WAP_PID_FLAG_EMBEDDED_PALETTE   = 1 << 7,
};

typedef struct wap_pid wap_pid;

typedef struct {
    unsigned char r, g, b;
} wap_rgb;

typedef struct {
    uint32_t id;
    uint32_t flags; /* WAP_PID_FLAG_ flags */
    uint32_t width;
    uint32_t height;
    uint32_t offset_x;
    uint32_t offset_y;
} wap_pid_header;

WAP_API wap_pid *wap_pid_create();

WAP_API void wap_pid_free(wap_pid *pid);

WAP_API int wap_pid_read_header(wap_pid_header *header, const char *pid_buffer, size_t pid_buffer_size);

WAP_API int wap_pid_read(wap_pid *pid, const char *pid_buffer, size_t pid_buffer_size);

WAP_API int wap_pid_open(wap_pid *pid, const char *file_path);

WAP_API int wap_pid_write(const wap_pid *pid, wap_buffer *out_pid_buffer);

WAP_API int wap_pid_save(const wap_pid *pid, const char *file_path);

WAP_END_DECLS

#endif
