cmake_minimum_required(VERSION 3.17)

file(GLOB_RECURSE HDR_FILES *.h)
file(GLOB_RECURSE SRC_FILES *.c)

add_library(common-lib-5gs ${HDR_FILES} ${SRC_FILES})

target_compile_options(common-lib-5gs PRIVATE -Wall -Wextra -pedantic -Wno-unused-parameter)

target_link_libraries(common-lib-5gs tun)
target_link_libraries(common-lib-5gs sctp)
target_link_libraries(common-lib-5gs sbi)
target_link_libraries(common-lib-5gs s1ap)
target_link_libraries(common-lib-5gs pfcp)
target_link_libraries(common-lib-5gs ngap)
target_link_libraries(common-lib-5gs nas)
target_link_libraries(common-lib-5gs ipfw)
target_link_libraries(common-lib-5gs gtp)
target_link_libraries(common-lib-5gs diameter)
target_link_libraries(common-lib-5gs dbi)
target_link_libraries(common-lib-5gs crypt)
target_link_libraries(common-lib-5gs core)
target_link_libraries(common-lib-5gs asn1c)
target_link_libraries(common-lib-5gs app)

