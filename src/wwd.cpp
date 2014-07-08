#include "wwd.h"
#include <cassert>
#include <fstream>

enum {
	WWD_SIGNATURE = 0x05F4
};

int wap32_wwd_open(Wap32Wwd **out, const char *file_path)
{
	*out = nullptr;
	auto wwd = new Wap32Wwd;

	*out = wwd;
	return 0;
}

void wap32_wwd_free(Wap32Wwd *wwd)
{
	delete wwd;
}
