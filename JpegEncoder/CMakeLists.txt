project (pract_jpeg)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_STANDARD 14)
find_package(Threads)

add_executable(pract_jpeg src/jpeg_encoder.cpp src/main.cpp)

target_link_libraries(pract_jpeg Threads::Threads)
