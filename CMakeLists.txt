cmake_minimum_required(VERSION 3.0.0)
project(dictionary VERSION 0.1.0)

add_executable(dictionary main.c trie.c cipher.c)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fexec-charset=GBK")
include(CPack)

find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIRS})
target_link_libraries(dictionary OpenSSL::Crypto)
