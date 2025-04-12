cmake_minimum_required(VERSION 3.12)
project(Compressor VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

find_package(ZLIB REQUIRED)

set(sources
    src/main.cpp
    src/Compressor.h
    src/Compressor.cpp
    src/ZlibWrapper.h
    src/SignalHandler.h
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${sources})

add_executable(Compressor
    ${sources}
)

target_link_libraries(Compressor PRIVATE ZLIB::ZLIB)
