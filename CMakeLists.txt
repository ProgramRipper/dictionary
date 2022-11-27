cmake_minimum_required(VERSION 3.0.0)
project(dictionary VERSION 0.1.0)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fexec-charset=GBK")

add_executable(dictionary main.c trie.c cipher.c)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIRS})
target_link_libraries(dictionary OpenSSL::Crypto)
