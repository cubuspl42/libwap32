#include "wwd.h"
#include "wwd_read.h"
#include "errors.h"
#include <cassert>
#include <fstream>
#include <memory>

Wap32Wwd::Wap32Wwd()
{
    memset(&this->data, 0, sizeof(this->data));
}

Wap32Wwd *wap32_wwd_create()
{
    return new Wap32Wwd;
}

void wap32_wwd_free(Wap32Wwd *wwd)
{
	delete wwd;
}

int wap32_wwd_open(Wap32Wwd **out, const char *file_path)
{
	*out = nullptr;
    std::ifstream file(file_path, std::ios::binary);
    std::vector<char> wwd_buffer((std::istreambuf_iterator<char>(file)),
                                   std::istreambuf_iterator<char>());
    std::unique_ptr<Wap32Wwd, void(*)(Wap32Wwd*)> wwd {wap32_wwd_create(), wap32_wwd_free};
    try {
        wap32_wwd__read(wwd.get(), wwd_buffer);
    } catch (Wap32Exception &ex) {
        return ex.error;
    }
    
	*out = wwd.release();
	return WAP32_OK;
}