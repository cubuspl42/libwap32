#include <wap32.h>
#include "../src/buffer.h"

#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, const char **argv) {
    assert(argc == 2);

    std::string filename = argv[1];
    std::ifstream file;
    file.open(filename.c_str());
    assert(file.good());

    std::vector<char> wwd_buffer_loaded((std::istreambuf_iterator<char>(file)),
                                        std::istreambuf_iterator<char>());

    wap_wwd *wwd = wap_wwd_create();
    assert(wwd);

    int error = 0;
    error =
        wap_wwd_read(wwd, wwd_buffer_loaded.data(), wwd_buffer_loaded.size());
    assert(!error);

    assert(
        !(wap_wwd_get_properties(wwd)->flags &
          WAP_WWD_FLAG_COMPRESS)); // It's best to test on non compressed files

    wap_buffer wwd_buffer_compressed;
    wap_buffer wwd_buffer_decompressed;
    wap_buffer_init(&wwd_buffer_compressed);
    wap_buffer_init(&wwd_buffer_decompressed);

    wap_wwd_get_properties(wwd)->flags |= WAP_WWD_FLAG_COMPRESS;

    error = wap_wwd_write(wwd, &wwd_buffer_compressed);
    assert(!error);

    error = wap_wwd_read(wwd, wap_buffer_data(&wwd_buffer_compressed),
                         wap_buffer_size(&wwd_buffer_compressed));
    assert(!error);

    wap_wwd_get_properties(wwd)->flags &= ~WAP_WWD_FLAG_COMPRESS;

    error = wap_wwd_write(wwd, &wwd_buffer_decompressed);
    assert(!error);

    // error = wap_wwd_save(wwd, filename.replace(filename.size()-4, 4,
    // "_libwap32.WWD").c_str());
    // assert(!error);

    assert(wwd_buffer_loaded ==
           *wap::cast_wap_buffer_to_vector(&wwd_buffer_decompressed));

    wap_buffer_destroy(&wwd_buffer_compressed);
    wap_buffer_destroy(&wwd_buffer_decompressed);

    return 0;
}