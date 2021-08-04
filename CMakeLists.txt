cmake_minimum_required(VERSION 3.17)
project(ueransim)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

include_directories(src)

#################### SUB DIRECTORIES ####################

add_subdirectory(src/ext)
add_subdirectory(src/utils)
add_subdirectory(src/asn/asn1c)
add_subdirectory(src/asn/rrc)
add_subdirectory(src/asn/ngap)
add_subdirectory(src/lib)
add_subdirectory(src/tester)


#################### GNB EXECUTABLE ####################

add_executable(main src/main.cpp)

target_compile_options(main PRIVATE -ggdb -Wall -Wextra -pedantic)

target_link_libraries(main pthread)
target_link_libraries(main asn-rrc)
target_link_libraries(main asn-ngap)
target_link_libraries(main common-lib)
target_link_libraries(main tester)
