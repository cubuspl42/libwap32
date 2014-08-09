#include "io.h"

namespace wap {
    std::vector<char> read_whole_file(const char *file_path) {
        wap_error_context errctx("opening file '%s'", file_path);
        std::ifstream file(file_path, std::ios::binary);
        std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        if(!file.good())
            throw wap::Error(WAP_EFILE);
        return buffer;
    }
}
