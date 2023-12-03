cmake_minimum_required(VERSION 3.14)

if (NOT LOGR_LIBRARY_NAME)
    set(LOGR_LIBRARY_NAME logr)
endif ()

# Define where cmake-scripts dir is located.
# in case of using this lib as a submodule it might be located somewhere else
# and not next to a root `CMakeLists.txt` file of the project.
if (NOT LOGR_CMAKE_SCRIPTS_DIR)
    # If not explicitly set use a default one:
    set(LOGR_CMAKE_SCRIPTS_DIR "${CMAKE_SOURCE_DIR}/cmake-scripts")
endif ()

# ====================================================================
# Version
# ====================================================================
include(${LOGR_CMAKE_SCRIPTS_DIR}/extract_version.cmake)
extract_version(
    "include/${LOGR_LIBRARY_NAME}/version.hpp"
    # Define extracted version as following:
    PRJ_VERSION_MAJOR
    PRJ_VERSION_MINOR
    PRJ_VERSION_PATCH
)

set(PRJ_VERSION ${PRJ_VERSION_MAJOR}.${PRJ_VERSION_MINOR}.${PRJ_VERSION_PATCH})
# ABI compatibility version.
# A safer approach is to declare compatibility on the level of minor version.
# If a major version compatibility is chosen then consider changing
# a COMPATIBILITY type in install section (SameMinorVersion => SameMajorVersion).
set(PRJ_SOVERSION ${PRJ_VERSION_MAJOR}.${PRJ_VERSION_MINOR})

message(STATUS "${LOGR_LIBRARY_NAME} VERSION  : " ${PRJ_VERSION})
message(STATUS "${LOGR_LIBRARY_NAME} SOVERSION: " ${PRJ_SOVERSION})

# ====================================================================
# Declare project
# ====================================================================

set(TARGET_PROJECT ${LOGR_LIBRARY_NAME})

project(${TARGET_PROJECT} LANGUAGES CXX VERSION ${PRJ_VERSION})

# ====================================================================
# Set sources
# ====================================================================

list(APPEND TARGET_PUBLIC_HEADERS
    include/${LOGR_LIBRARY_NAME}/logr.hpp
    include/${LOGR_LIBRARY_NAME}/null_backend.hpp
    include/${LOGR_LIBRARY_NAME}/ostream_backend.hpp

    # Since v0.7.0
    include/${LOGR_LIBRARY_NAME}/version.hpp
)

if (LOGR_WITH_SPDLOG_BACKEND)
    list(APPEND TARGET_PUBLIC_HEADERS include/${LOGR_LIBRARY_NAME}/spdlog_backend.hpp )
endif ()

if (LOGR_WITH_GLOG_BACKEND)
    list(APPEND TARGET_PUBLIC_HEADERS include/${LOGR_LIBRARY_NAME}/glog_backend.hpp )
endif ()

if (LOGR_WITH_LOG4CPLUS_BACKEND)
    list(APPEND TARGET_PUBLIC_HEADERS include/${LOGR_LIBRARY_NAME}/log4cplus_backend.hpp )
endif ()

if (LOGR_WITH_BOOSTLOG_BACKEND)
    list(APPEND TARGET_PUBLIC_HEADERS include/${LOGR_LIBRARY_NAME}/boostlog_backend.hpp )
endif ()

# ====================================================================
# TARGET
# ====================================================================
add_library(${TARGET_PROJECT}_base INTERFACE ${TARGET_PUBLIC_HEADERS})
add_library(${TARGET_PROJECT}::${TARGET_PROJECT}_base ALIAS ${TARGET_PROJECT}_base)

set(PROJECT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
message(STATUS "${TARGET_PROJECT} include_dir: ${PROJECT_INCLUDE_DIR}")

target_include_directories(${TARGET_PROJECT}_base
                           INTERFACE
                           $<BUILD_INTERFACE:${PROJECT_INCLUDE_DIR}>
                           $<INSTALL_INTERFACE:include>
)

target_link_libraries(${TARGET_PROJECT}_base INTERFACE fmt::fmt)

# Targets for install
list(APPEND TARGETS_LIST ${TARGET_PROJECT}_base)

if (LOGR_WITH_SPDLOG_BACKEND)
    add_library(${TARGET_PROJECT}_spdlog INTERFACE)
    add_library(${TARGET_PROJECT}::${TARGET_PROJECT}_spdlog ALIAS ${TARGET_PROJECT}_spdlog)
    target_link_libraries(${TARGET_PROJECT}_spdlog
                          INTERFACE
                          ${TARGET_PROJECT}::${TARGET_PROJECT}_base
                          spdlog::spdlog_header_only
    )
    list(APPEND TARGETS_LIST ${TARGET_PROJECT}_spdlog)
endif ()

if (LOGR_WITH_GLOG_BACKEND)
    add_library(${TARGET_PROJECT}_glog INTERFACE)
    add_library(${TARGET_PROJECT}::${TARGET_PROJECT}_glog ALIAS ${TARGET_PROJECT}_glog)
    target_link_libraries(${TARGET_PROJECT}_glog
                          INTERFACE
                          ${TARGET_PROJECT}::${TARGET_PROJECT}_base
                          glog::glog
    )
    list(APPEND TARGETS_LIST ${TARGET_PROJECT}_glog)
endif ()

if (LOGR_WITH_LOG4CPLUS_BACKEND)
    add_library(${TARGET_PROJECT}_log4cplus INTERFACE)
    add_library(${TARGET_PROJECT}::${TARGET_PROJECT}_log4cplus ALIAS ${TARGET_PROJECT}_log4cplus)
    target_link_libraries(${TARGET_PROJECT}_log4cplus
                          INTERFACE
                          ${TARGET_PROJECT}::${TARGET_PROJECT}_base
                          log4cplus::log4cplus
    )
    list(APPEND TARGETS_LIST ${TARGET_PROJECT}_log4cplus)
endif ()

if (LOGR_WITH_BOOSTLOG_BACKEND)
    add_library(${TARGET_PROJECT}_boostlog INTERFACE)
    add_library(${TARGET_PROJECT}::${TARGET_PROJECT}_boostlog ALIAS ${TARGET_PROJECT}_boostlog)
    target_link_libraries(${TARGET_PROJECT}_boostlog
                          INTERFACE
                          ${TARGET_PROJECT}::${TARGET_PROJECT}_base
                          Boost::log
    )
    list(APPEND TARGETS_LIST ${TARGET_PROJECT}_boostlog)
endif ()
# ====================================================================

# ====================================================================
# Install
# ====================================================================
if (LOGR_INSTALL)
    include(${LOGR_CMAKE_SCRIPTS_DIR}/lib_install.cmake)

    lib_install(
        TARGET_PROJECT ${TARGET_PROJECT}
        VERSION "${PRJ_VERSION}"
        LIB_CONFIG cmake/lib-config.cmake.in
        TARGETS_LIST ${TARGETS_LIST}
    )

    lib_install_headers(
        REMOVE_INCLUDE_DIR_PREFIX include
        HEADERS ${TARGET_PUBLIC_HEADERS}
    )
endif ()
