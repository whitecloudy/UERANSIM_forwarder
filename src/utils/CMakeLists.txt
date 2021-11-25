cmake_minimum_required(VERSION 3.10)

file(GLOB_RECURSE HDR_FILES *.hpp)
file(GLOB_RECURSE SRC_FILES *.cpp)

add_library(utils ${HDR_FILES} ${SRC_FILES})

target_compile_options(utils PRIVATE -g -fPIC -Wall -Wextra -pedantic -Wno-unused-parameter)
target_compile_options(utils PUBLIC -g -fPIC -Wno-format-security)

target_link_libraries(utils ext)
