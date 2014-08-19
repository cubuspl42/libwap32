#ifndef wap_buffer_h
#define wap_buffer_h

#include "common.h"

WAP_BEGIN_DECLS

typedef struct {
    void *_private[4]; // opaque struct
} wap_buffer;

WAP_API void wap_buffer_init(wap_buffer *buffer);

WAP_API char *wap_buffer_data(wap_buffer *buffer);

WAP_API size_t wap_buffer_size(wap_buffer *buffer);

WAP_API void wap_buffer_destroy(wap_buffer *buffer);

WAP_END_DECLS

#endif