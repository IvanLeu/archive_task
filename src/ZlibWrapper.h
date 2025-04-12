#pragma once

#include <zlib.h>
#include <stdexcept>
#include <string>

class Deflater {
public:
    explicit Deflater(int level = Z_DEFAULT_COMPRESSION) {
        stream_.zalloc = Z_NULL;
        stream_.zfree  = Z_NULL;
        stream_.opaque = Z_NULL;
        int ret = deflateInit(&stream_, level);
        if (ret != Z_OK) {
            throw std::runtime_error("deflateInit failed: " + std::to_string(ret));
        }
    }
    ~Deflater() {
        deflateEnd(&stream_);
    }
    z_stream* get() { return &stream_; }
private:
    z_stream stream_;
};

class Inflater {
public:
    Inflater() {
        stream_.zalloc = Z_NULL;
        stream_.zfree  = Z_NULL;
        stream_.opaque = Z_NULL;
        int ret = inflateInit(&stream_);
        if (ret != Z_OK) {
            throw std::runtime_error("inflateInit failed: " + std::to_string(ret));
        }
    }
    ~Inflater() {
        inflateEnd(&stream_);
    }
    z_stream* get() { return &stream_; }
private:
    z_stream stream_;
};
