cmake_minimum_required(VERSION 3.10)

file(GLOB_RECURSE HDR_FILES *.hpp)
file(GLOB_RECURSE SRC_FILES *.cpp)

message(STATUS ${SRC_FILES})

add_library(tester ${HDR_FILES} ${SRC_FILES})

target_compile_options(tester PRIVATE -fPIC -Wall -Wextra -pedantic -Wno-unused-parameter)

target_link_libraries(tester utils)
target_link_libraries(tester asn-ngap)
target_link_libraries(tester asn-rrc)
target_link_libraries(tester common-lib)

target_link_libraries(tester sctp) # Linux SCTP
