# Root CMake for logr project.

cmake_minimum_required(VERSION 3.14)

set(logr_root_prj logr_root)

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake-scripts/cpp_standard.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake-scripts/libcxx.cmake")

message(STATUS "ENABLE C++17")
cxx_17()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# ------------------------------------------------------------------------------
# Choose package manager and set propper cmake-variables for it.
if (NOT LOGR_PKG_PROVIDER)
    set(LOGR_PKG_PROVIDER "conan")
endif ()

string( TOLOWER "${LOGR_PKG_PROVIDER}" logr_pkg_provider_lowercase )

if( NOT ${logr_pkg_provider_lowercase} STREQUAL "conan" AND
    NOT ${logr_pkg_provider_lowercase} STREQUAL "vcpkg" )
    message( FATAL_ERROR "Unknown package manager: ${LOGR_PKG_PROVIDER}")
endif ()

message( STATUS "Use package manager: ${LOGR_PKG_PROVIDER}")


if( "${logr_pkg_provider_lowercase}" STREQUAL "conan" )
    set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR} ${CMAKE_MODULE_PATH})
    set(CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR} ${CMAKE_PREFIX_PATH})
elseif ( "${logr_pkg_provider_lowercase}" STREQUAL "vcpkg" )
    if( NOT CMAKE_TOOLCHAIN_FILE )
        message( FATAL_ERROR "CMAKE_TOOLCHAIN_FILE must be defined when using vcpkg")
    endif ()
else ()
    message( FATAL_ERROR "packge manager '${logr_pkg_provider_lowercase}' not supported")
endif ()
# ------------------------------------------------------------------------------


# ------------------------------------------------------------------------------
# Build options
option(LOGR_INSTALL "Generate install target." ON)
option(LOGR_BUILD_TESTS "Build tests." ON)
option(LOGR_BUILD_EXAMPLES "Build examples." ON)
option(LOGR_BUILD_BENCHMARKS "Build benchmarks." ON)

# It doesn't work well for all cmake generators, so it is disabled by default.
option(LOGR_PRJ_ROOT_LENGTH_HINT_ENABLED
       "Enable a hint to cut prj-root from sources path in logging."
       OFF)

option(LOGR_PRJ_COLLAPSE_SRC_LOCATION_ENABLED
       "Enable a hint to collapse all src location (make them unused)."
       OFF)

# Backends.
option(LOGR_WITH_SPDLOG_BACKEND "Provide logr with spdlog backend" ON)
option(LOGR_WITH_GLOG_BACKEND "Provide logr with glog backend" ON)
option(LOGR_WITH_LOG4CPLUS_BACKEND "Provide logr with log4cplus backend" ON)
# ------------------------------------------------------------------------------

project(${logr_root_prj} CXX)
handle_explicit_libcxx_if_necessary()

# ------------------------------------------------------------------------------
# Various helpers:
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake-scripts/common_options.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake-scripts/compiler_flags.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake-scripts/find_program_required.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake-scripts/append_src_files.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake-scripts/link_threads_if_necessary.cmake")
include( "${CMAKE_CURRENT_SOURCE_DIR}/cmake-scripts/static_analysis.cmake" )
# ------------------------------------------------------------------------------

# ------------------------------------------------------------------------------
# A piece to define hint for pretty source files paths.
if (LOGR_PRJ_COLLAPSE_SRC_LOCATION_ENABLED)
    add_compile_definitions("LOGR_PRJ_COLLAPSE_SRC_LOCATION")
else ()
    if (LOGR_PRJ_ROOT_LENGTH_HINT_ENABLED)
        if( NOT ${CMAKE_GENERATOR} STREQUAL "Ninja" )
            # Ninja uses relative path for sources.
            string(LENGTH "${CMAKE_SOURCE_DIR}/" project_root_path_len)
            add_compile_definitions("LOGR_PRJ_ROOT_LENGTH_HINT=${project_root_path_len}")
        else ()
            # Ninja uses a relative path, so LOGR_PRJ_ROOT_LENGTH_HINT must be ignored.
            # For a target project a corresponding macro definition
            # can be adjusted with knowing the "head" to cut from sources.
            message(WARNING "LOGR_PRJ_ROOT_LENGTH_HINT is ignored for Ninja build")
        endif()
    endif ()
endif ()
# ------------------------------------------------------------------------------

find_package(fmt        REQUIRED)

if (LOGR_WITH_SPDLOG_BACKEND)
    find_package(spdlog     REQUIRED)
endif ()

if (LOGR_WITH_GLOG_BACKEND)
    find_package(glog       REQUIRED)
endif ()

if (LOGR_WITH_LOG4CPLUS_BACKEND)
    find_package(log4cplus  REQUIRED)
endif ()

if (LOGR_BUILD_BENCHMARKS)
    find_package(benchmark  REQUIRED)
endif ()

# ===================================
# Logr iself:
add_subdirectory( logr )
# ===================================

if (LOGR_BUILD_TESTS)
    enable_testing()
    message(STATUS "Tests are enabled")

    set(CMAKE_FIND_PACKAGE_PREFER_CONFIG ON)
    find_package(GTest 1.10.0  REQUIRED)

    add_subdirectory(tests)
endif ()

if (LOGR_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif ()

if (LOGR_BUILD_BENCHMARKS)
    add_subdirectory(benchmarks)
endif ()




