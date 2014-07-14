#ifndef wap32_common_h
#define wap32_common_h

#include <stddef.h>

#ifndef WAP32_API
#  ifdef _WIN32
#     if defined(WAP32_BUILD_SHARED) /* build dll */
#         define WAP32_API __declspec(dllexport)
#     elif !defined(WAP32_BUILD_STATIC) /* use dll */
#         define WAP32_API __declspec(dllimport)
#     else /* static library */
#         define WAP32_API
#     endif
#  else
#     if __GNUC__ >= 4
#         define WAP32_API __attribute__((visibility("default")))
#     else
#         define WAP32_API
#     endif
#  endif
#endif

#ifdef __cplusplus
#  define WAP32_BEGIN_DECLS extern "C" {
#  define WAP32_END_DECLS }
#else
#  define WAP32_BEGIN_DECLS
#  define WAP32_END_DECLS
#endif

#endif