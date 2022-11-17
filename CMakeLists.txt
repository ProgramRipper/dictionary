cmake_minimum_required(VERSION 3.0.0)
project(dictionary VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(dictionary main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

find_package(OpenSSL REQUIRED)
if(OPENSSL_FOUND)
  include_directories(${OPENSSL_INCLUDE_DIRS})
  target_link_libraries(dictionary OpenSSL::Crypto)
  message(STATUS "OpenSSL Found!")
endif()
