#ifndef wap32_common_h
#define wap32_common_h

#ifdef LIBWAP32_EXPORTS
#define WAP32_API __declspec(dllexport)
#else
#define WAP32_API __declspec(dllimport)
#endif

struct wap32_Rect {
	unsigned left;
	unsigned top;
	unsigned right;
	unsigned bottom;
};

#endif