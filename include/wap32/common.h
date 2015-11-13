#ifndef wap_common_h
#define wap_common_h

#include <limits.h>
#include <stddef.h>
#include <stdint.h>

#define WAP_INTEGER_ENCODING (-1 & 3)
#define WAP_SIGN_AND_MAGNITUDE 1
#define WAP_ONES_COMPLEMENT 2
#define WAP_TWOS_COMPLEMENT 3

#if WAP_INTEGER_ENCODING != WAP_TWOS_COMPLEMENT
#error Support for integer encoding other than two’s complement is not implemented
#endif

#if CHAR_BIT != 8
#error There is no support for non-8bit chars
#endif

#ifndef WAP_API
#ifdef _WIN32
#if defined(WAP_BUILD_SHARED) /* build dll */
#define WAP_API __declspec(dllexport)
#elif !defined(WAP_BUILD_STATIC) /* use dll */
#define WAP_API __declspec(dllimport)
#else /* static library */
#define WAP_API
#endif
#else
#if __GNUC__ >= 4
#define WAP_API __attribute__((visibility("default")))
#else
#define WAP_API
#endif
#endif
#endif

#ifdef __cplusplus
#define WAP_BEGIN_DECLS extern "C" {
#define WAP_END_DECLS }
#else
#define WAP_BEGIN_DECLS
#define WAP_END_DECLS
#endif

#endif
