#include "Compressor.h"
#include "ZlibWrapper.h"
#include "SignalHandler.h"

#include <fstream>
#include <iostream>
#include <vector>
#include <filesystem>
#include <string_view>

constexpr std::size_t CHUNK_SIZE = 64 * 1024;

bool compressFile(const std::string &srcFile, const std::string &destFile) {
    std::ifstream in(srcFile, std::ios::binary);
    if (!in) {
        std::cerr << "Error opening source file: " << srcFile << std::endl;
        return false;
    }

    std::ofstream out(destFile, std::ios::binary);
    if (!out) {
        std::cerr << "Error opening destination file: " << destFile << std::endl;
        return false;
    }

    try {
        Deflater deflater;
        auto zstrm = deflater.get();

        std::vector<char> inBuffer(CHUNK_SIZE);
        std::vector<char> outBuffer(CHUNK_SIZE);
        int flush;

        do {
            in.read(inBuffer.data(), inBuffer.size());
            std::streamsize bytesRead = in.gcount();

            if (in.bad()) {
                std::cerr << "Error reading from source file." << std::endl;
                return false;
            }

            flush = in.eof() ? Z_FINISH : Z_NO_FLUSH;

            zstrm->avail_in = static_cast<uInt>(bytesRead);
            zstrm->next_in = reinterpret_cast<Bytef*>(inBuffer.data());

            do {
                zstrm->avail_out = static_cast<uInt>(outBuffer.size());
                zstrm->next_out = reinterpret_cast<Bytef*>(outBuffer.data());

                int ret = deflate(zstrm, flush);
                if (ret == Z_STREAM_ERROR) {
                    std::cerr << "Compression error: " << ret << std::endl;
                    return false;
                }

                std::size_t have = outBuffer.size() - zstrm->avail_out;
                out.write(outBuffer.data(), have);

                if (!out) {
                    std::cerr << "Error writing to destination file." << std::endl;
                    return false;
                }

                if (SignalHandler::interrupted) {
                    std::cerr << "Compression interrupted by user." << std::endl;
                    return false;
                }

            } while (zstrm->avail_out == 0);
        } while (flush != Z_FINISH);

    } 
    catch (const std::exception &ex) {
        std::cerr << "Exception during compression: " << ex.what() << std::endl;
        return false;
    }

    return true;
}

bool decompressFile(const std::string &srcFile, const std::string &destFile) {
    std::ifstream in(srcFile, std::ios::binary);
    if (!in) {
        std::cerr << "Error opening source file: " << srcFile << std::endl;
        return false;
    }
    std::ofstream out(destFile, std::ios::binary);
    if (!out) {
        std::cerr << "Error opening destination file: " << destFile << std::endl;
        return false;
    }

    try {
        Inflater inflater;
        auto zstrm = inflater.get();
        std::vector<char> inBuffer(CHUNK_SIZE);
        std::vector<char> outBuffer(CHUNK_SIZE);
        int ret;

        do {
            in.read(inBuffer.data(), inBuffer.size());
            std::streamsize bytesRead = in.gcount();

            if (in.bad()) {
                std::cerr << "Error reading from source file." << std::endl;
                return false;
            }

            if (bytesRead == 0)
                break;

            zstrm->avail_in = static_cast<uInt>(bytesRead);
            zstrm->next_in = reinterpret_cast<Bytef*>(inBuffer.data());

            do {
                zstrm->avail_out = static_cast<uInt>(outBuffer.size());
                zstrm->next_out = reinterpret_cast<Bytef*>(outBuffer.data());

                ret = inflate(zstrm, Z_NO_FLUSH);
                if (ret == Z_STREAM_ERROR) {
                    std::cerr << "Decompression error: " << ret << std::endl;
                    return false;
                }

                switch (ret) {
                    case Z_NEED_DICT:
                    case Z_DATA_ERROR:
                    case Z_MEM_ERROR:
                        std::cerr << "Decompression error: " << ret << std::endl;
                        return false;
                }

                std::size_t have = outBuffer.size() - zstrm->avail_out;
                out.write(outBuffer.data(), have);

                if (!out) {
                    std::cerr << "Error writing to destination file." << std::endl;
                    return false;
                }

                if (SignalHandler::interrupted) {
                    std::cerr << "Decompression interrupted by user." << std::endl;
                    return false;
                }

            } while (zstrm->avail_out == 0);
        } while (ret != Z_STREAM_END);

        if (ret != Z_STREAM_END) {
            std::cerr << "Incomplete decompression." << std::endl;
            return false;
        }
    } 
    catch (const std::exception &ex) {
        std::cerr << "Exception during decompression: " << ex.what() << std::endl;
        return false;
    }

    return true;
}