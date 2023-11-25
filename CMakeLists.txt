cmake_minimum_required(VERSION 3.0.0)
project(autoclosing_stream_tag VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)

include(FetchContent)


FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.4.0 # or a later release
)
FetchContent_MakeAvailable(Catch2)
list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
include(Catch)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

add_library(autoclosing_stream_tag INTERFACE)
target_include_directories(autoclosing_stream_tag INTERFACE "src")

add_executable(autoclosing_stream_tag_tests "test/autoclosing_tag_test.cpp")
target_link_libraries(autoclosing_stream_tag_tests PRIVATE Catch2::Catch2WithMain autoclosing_stream_tag)

catch_discover_tests(autoclosing_stream_tag_tests)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
