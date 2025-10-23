cmake_minimum_required(VERSION 3.10)
project(image_processor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(
    image_processor
    src/applier.cpp
    src/args.cpp
    src/bmp.cpp
    src/check_bmp.cpp
    src/filters.cpp
    src/launcher.cpp
    image_processor.cpp
)

target_include_directories(image_processor PRIVATE include)
