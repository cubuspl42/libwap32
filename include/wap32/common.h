#ifndef wap_common_h
#define wap_common_h

#include <stddef.h>

#ifndef WAP_API
#  ifdef _WIN32
#     if defined(WAP_BUILD_SHARED) /* build dll */
#         define WAP_API __declspec(dllexport)
#     elif !defined(WAP_BUILD_STATIC) /* use dll */
#         define WAP_API __declspec(dllimport)
#     else /* static library */
#         define WAP_API
#     endif
#  else
#     if __GNUC__ >= 4
#         define WAP_API __attribute__((visibility("default")))
#     else
#         define WAP_API
#     endif
#  endif
#endif

#ifdef __cplusplus
#  define WAP_BEGIN_DECLS extern "C" {
#  define WAP_END_DECLS }
#else
#  define WAP_BEGIN_DECLS
#  define WAP_END_DECLS
#endif

#endif
