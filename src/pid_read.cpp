#include "pid.h"
#include "io.h"

void read_header(wap::InputStream &stream, wap_pid_header &header) {
    stream.read(header.id, header.flags, header.width, header.offset_x,
                header.offset_y);
}

void pid_read_header(wap_pid_header &header, const char *pid_buffer,
                     size_t pid_buffer_size) {
    wap::InputStream stream(pid_buffer, pid_buffer_size);
    read_header(stream, header);
}

void pid_read(wap_pid &pid, const char *pid_buffer, size_t pid_buffer_size) {
    wap::InputStream stream(pid_buffer, pid_buffer_size);
    read_header(stream, pid.header);
}

void pid_open(wap_pid &pid, const char *file_path) {
    std::vector<char> pid_buffer = wap::read_whole_file(file_path);
    pid_read(pid, pid_buffer.data(), pid_buffer.size());
}

int wap_pid_read_header(wap_pid_header *header, const char *pid_buffer,
                        size_t pid_buffer_size) {
    return wap::handle_exceptions(pid_read_header, *header, pid_buffer,
                                  pid_buffer_size);
}

int wap_pid_read(wap_pid *pid, const char *pid_buffer, size_t pid_buffer_size) {
    return wap::handle_exceptions(pid_read, *pid, pid_buffer, pid_buffer_size);
}

int wap_pid_open(wap_pid *pid, const char *file_path) {
    return wap::handle_exceptions(pid_open, *pid, file_path);
}
