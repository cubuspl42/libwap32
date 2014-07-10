#include "wwd.h"
#include "errors.h"
#include <cassert>
#include <fstream>
#include <memory>

Wap32Wwd::Wap32Wwd()
{
    memset(this, 0, offsetof(Wap32Wwd, planes));
}

Wap32Wwd *wap32_wwd_create()
{
    return new (std::nothrow) Wap32Wwd;
}

void wap32_wwd_free(Wap32Wwd *wwd)
{
	delete wwd;
}

int wap32_wwd_open(Wap32Wwd **out, const char *file_path)
{
    try {
        Wap32ErrorContext errctx("opening file '%s'", file_path);
        *out = nullptr;
        
        std::ifstream file(file_path, std::ios::binary);
        if(!file.good())
            return WAP32_EFILE;
        std::vector<char> wwd_buffer((std::istreambuf_iterator<char>(file)),
                                     std::istreambuf_iterator<char>());
        std::unique_ptr<Wap32Wwd, void(*)(Wap32Wwd*)> wwd {wap32_wwd_create(), wap32_wwd_free};
        
        int error = wap32_wwd__read(wwd.get(), wwd_buffer);
        if(error < 0)
            return error;
        
        *out = wwd.release();
        return 0;
    } catch(std::bad_alloc&) {
        return WAP32_ENOMEMORY;
    }
}