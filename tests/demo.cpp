#include <wap32.h>
#include "../src/wwd.h"
#include "../src/errors.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

using namespace std;

typedef unique_ptr<wap_wwd, void(*)(wap_wwd*)> unique_wwd_pointer;

int main(int argc, const char **argv)
{
    for(int i = 1; i < argc; ++i) {
        string filename = argv[i];
        cerr << filename << endl;
        
        wap_wwd *wwd = wap_wwd_create();
        unique_wwd_pointer wwd_ptr(wwd, wap_wwd_free);
        
        wap_buffer wwd_buffer_compressed, wwd_buffer_decompressed;
        wap_buffer_init(&wwd_buffer_decompressed);
        wap_buffer_init(&wwd_buffer_compressed);
        
        int error = 0;
        error = wap_wwd_open(wwd, filename.c_str());
        assert(!error);
        assert(wap_wwd_get_properties(wwd)->flags & WAP_WWD_FLAG_COMPRESS);
        error = wap_wwd_write(wwd, &wwd_buffer_compressed);
        wap_wwd_get_properties(wwd)->flags = 0;
        error = wap_wwd_save(wwd, "their_decompressed.WWD");
        assert(!error);
        error = wap_wwd_read(wwd, wap_buffer_data(&wwd_buffer_compressed), wap_buffer_size(&wwd_buffer_compressed));
        assert(!error);
        wap_wwd_get_properties(wwd)->flags = 0;
        error = wap_wwd_save(wwd, "our_decompressed.WWD");
        assert(!error);
        
        wap_buffer_destroy(&wwd_buffer_decompressed);
        wap_buffer_destroy(&wwd_buffer_compressed);
    }
}