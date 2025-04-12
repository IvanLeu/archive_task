#include "Compressor.h"
#include "SignalHandler.h"

#include <filesystem>
#include <iostream>
#include <string>
#include <format>

int main(int argc, char* argv[]) {
    SignalHandler::setupSignalHandler();

    if (argc != 4) {
        std::cout << "Usage:\n"
                  << argv[0] << " a <input file> <output file>\n"
                  << argv[0] << " e <input file> <output file>\n";
        return EXIT_FAILURE;
    }

    std::string mode    = argv[1];
    std::string srcFile = argv[2];
    std::string destFile= argv[3];

    if (!std::filesystem::exists(srcFile)) {
        std::cerr << "Source file does not exist: " << srcFile << std::endl;
        return EXIT_FAILURE;
    }

    bool result = false;
    if (mode == "a") {
        std::cout << std::format("Compressing '{}', to '{}'...", srcFile, destFile) << std::endl;
        result = compressFile(srcFile, destFile);
    } else if (mode == "e") {
        std::cout << std::format("Decompressing '{}', to '{}'...", srcFile, destFile) << std::endl;
        result = decompressFile(srcFile, destFile);
    } else {
        std::cerr << "Invalid mode: " << mode << std::endl;
        std::cout << "Usage:\n"
                  << argv[0] << " a <input file> <output file>\n"
                  << argv[0] << " e <input file> <output file>\n";
        return EXIT_FAILURE;
    }

    if (!result) {
        std::cerr << "Operation failed." << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Operation completed successfully." << std::endl;

    return 0;
}