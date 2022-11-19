cmake_minimum_required(VERSION 3.0.0)
project(dictionary VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(dictionary.exe main.c trie.c)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

find_package(OpenSSL)
if(NOT OPENSSL_FOUND)
  set(OPENSSL_ROOT_DIR "/mnt/d/Program Files/OpenSSL-Win64")
  set(OPENSSL_INCLUDE_DIR "/mnt/d/Program Files/OpenSSL-Win64/include")
  set(OPENSSL_SSL_LIBRARY "/mnt/d/Program Files/OpenSSL-Win64/lib/libssl.lib")
  set(OPENSSL_CRYPTO_LIBRARY "/mnt/d/Program Files/OpenSSL-Win64/lib/libcrypto.lib")
  find_package(OpenSSL REQUIRED)
endif()
include_directories(${OPENSSL_INCLUDE_DIRS})
target_link_libraries(dictionary.exe OpenSSL::Crypto)