# Root CMake for logr project.
cmake_minimum_required(VERSION 3.15)

set(LOGR_LIBRARY_NAME logr)

project(${LOGR_LIBRARY_NAME}_root CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# ------------------------------------------------------------------------------
# Various helpers:
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake-scripts/common_options.cmake")

set(COMPILER_ENABLE_UNUSED_X_AS_ERROR OFF)
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake-scripts/compiler_flags.cmake")

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake-scripts/find_program_required.cmake")
include( "${CMAKE_CURRENT_SOURCE_DIR}/cmake-scripts/static_analysis.cmake" )
include( "${CMAKE_CURRENT_SOURCE_DIR}/cmake-scripts/code_coverage.cmake" )

# ------------------------------------------------------------------------------
# Build options
option(LOGR_INSTALL           "Generate install target"  ON)
option(LOGR_BUILD_TESTS       "Build tests"              ON)
option(LOGR_BUILD_EXAMPLES    "Build examples"           ON)
option(LOGR_BUILD_BENCHMARK   "Build benchmarks"         ON)
option(LOGR_GCC_CODE_COVERAGE "Build with code coverage" OFF)

# It doesn't work well for all cmake generators, so it is disabled by default.
option(LOGR_PRJ_ROOT_LENGTH_HINT_ENABLED
       "Enable a hint to cut prj-root from sources path in logging."
       OFF)

option(LOGR_PRJ_COLLAPSE_SRC_LOCATION_ENABLED
       "Enable a hint to collapse all src location (make them unused)."
       OFF)

# Backends.
option(LOGR_WITH_SPDLOG_BACKEND    "Provide logr with spdlog backend"    ON)
option(LOGR_WITH_GLOG_BACKEND      "Provide logr with glog backend"      ON)
option(LOGR_WITH_LOG4CPLUS_BACKEND "Provide logr with log4cplus backend" ON)
option(LOGR_WITH_BOOSTLOG_BACKEND  "Provide logr with Boost.Log backend" ON)


message(STATUS "LOGR_INSTAL:                 ${LOGR_INSTALL}")
message(STATUS "LOGR_BUILD_TEST:             ${LOGR_BUILD_TESTS}")
message(STATUS "LOGR_BUILD_EXAMPLES:         ${LOGR_BUILD_EXAMPLES}")
message(STATUS "LOGR_BUILD_BENCHMARK:        ${LOGR_BUILD_BENCHMARK}")
message(STATUS "LOGR_GCC_CODE_COVERAGE:      ${LOGR_GCC_CODE_COVERAGE}")
message(STATUS "LOGR_WITH_SPDLOG_BACKEND:    ${LOGR_GCC_CODE_COVERAGE}")
message(STATUS "LOGR_WITH_GLOG_BACKEND:      ${LOGR_GCC_CODE_COVERAGE}")
message(STATUS "LOGR_WITH_LOG4CPLUS_BACKEND: ${LOGR_GCC_CODE_COVERAGE}")
message(STATUS "LOGR_WITH_BOOSTLOG_BACKEND:  ${LOGR_GCC_CODE_COVERAGE}")

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

if (LOGR_WITH_BOOSTLOG_BACKEND)
    find_package(Boost  REQUIRED COMPONENTS log)
endif ()

if (LOGR_BUILD_BENCHMARK)
    find_package(benchmark  REQUIRED)
endif ()

if (LOGR_GCC_CODE_COVERAGE)
    make_code_coverage_targets(
        NAME CodeCoverage
        HTML_TITLE "Code coverage report for logr library"
        FILTERS
            '${CMAKE_SOURCE_DIR}/${LOGR_LIBRARY_NAME}/src/.*'
            '${CMAKE_SOURCE_DIR}/${LOGR_LIBRARY_NAME}/include/.*'
    )
endif()

# ===================================
# Logr iself:
add_subdirectory(${LOGR_LIBRARY_NAME})
# ===================================

if (LOGR_BUILD_TESTS)
    include(CTest)
    enable_testing()
    message(STATUS "Tests are enabled")

    set(CMAKE_FIND_PACKAGE_PREFER_CONFIG ON)
    find_package(GTest REQUIRED)

    add_subdirectory(${LOGR_LIBRARY_NAME}/test)
endif ()

if (LOGR_BUILD_EXAMPLES)
    add_subdirectory(${LOGR_LIBRARY_NAME}/examples)
endif ()

if (LOGR_BUILD_BENCHMARK)
    add_subdirectory(${LOGR_LIBRARY_NAME}/benchmark)
endif ()
