cmake_minimum_required(VERSION 3.14)

include("${CMAKE_CURRENT_SOURCE_DIR}/../cmake-scripts/cpp_standard.cmake")

message(STATUS "ENABLE C++17")
cxx_17()

project(PackageTest CXX)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(example example.cpp)
target_link_libraries(example ${CONAN_LIBS})

# CTest is a testing tool that can be used to test your project.
# enable_testing()
# add_test(NAME example
#          WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
#          COMMAND example)
