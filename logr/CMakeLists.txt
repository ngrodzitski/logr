cmake_minimum_required(VERSION 3.14)

set(logr_prj logr)

project(${logr_prj})

set(logr_public_headers
    include/logr/logr.hpp
    include/logr/null_backend.hpp
    include/logr/ostream_backend.hpp )

if (LOGR_WITH_SPDLOG_BACKEND)
    list(APPEND logr_public_headers include/logr/spdlog_backend.hpp )
endif ()

if (LOGR_WITH_GLOG_BACKEND)
    list(APPEND logr_public_headers include/logr/glog_backend.hpp )
endif ()

if (LOGR_WITH_LOG4CPLUS_BACKEND)
    list(APPEND logr_public_headers include/logr/log4cplus_backend.hpp )
endif ()

if (LOGR_WITH_BOOSTLOG_BACKEND)
    list(APPEND logr_public_headers include/logr/boostlog_backend.hpp )
endif ()

set(logr_headers ${logr_public_headers})

list(APPEND logr_headers include/logr/config.hpp.in)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${logr_headers})

# ====================================================================
# Version & Config
# ====================================================================
include(version.cmake)

set(TARGET_LIB_VERSION
    ${TARGET_LIB_VERSION_MAJOR}.${TARGET_LIB_VERSION_MINOR}.${TARGET_LIB_VERSION_PATCH})

# ABI compatibility version.
# A safer approach is to declare compatibility on the level of minor version.
# If a major version compatibility is chosen then consider changing
# a COMPATIBILITY type in install section (SameMinorVersion => SameMajorVersion).
set(TARGET_LIB_SOVERSION ${TARGET_LIB_VERSION_MAJOR}.${TARGET_LIB_VERSION_MINOR})

message(STATUS "LOGR_LIB_VERSION  : " ${TARGET_LIB_VERSION})
message(STATUS "LOGR_LIB_SOVERSION: " ${TARGET_LIB_SOVERSION})

set(CONFIG_HEADER_INCLUDE_PATH "${PROJECT_BINARY_DIR}/include")
set(CONFIG_HEADER_PATH "${CONFIG_HEADER_INCLUDE_PATH}/logr/config.hpp")

configure_file(include/logr/config.hpp.in ${CONFIG_HEADER_PATH} )
# ====================================================================

# ====================================================================
# TARGET
# ====================================================================
add_library(${logr_prj} INTERFACE)
add_library(${logr_prj}::${logr_prj} ALIAS ${logr_prj})

# To have the same "Component target as in Conan:"
add_library(${logr_prj}_base INTERFACE)
add_library(${logr_prj}::${logr_prj}_base ALIAS ${logr_prj}_base)
set_property(TARGET ${logr_prj}_base APPEND PROPERTY
             INTERFACE_LINK_LIBRARIES "${logr_prj}")

target_compile_features(${logr_prj} INTERFACE cxx_std_17)

set(TARGET_BUILD_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include)

message("${logr_prj}_build_include_dir: ${TARGET_BUILD_INCLUDE_DIR}")

target_include_directories(${logr_prj} INTERFACE
        $<BUILD_INTERFACE:${TARGET_BUILD_INCLUDE_DIR}>
        $<BUILD_INTERFACE:${CONFIG_HEADER_INCLUDE_PATH}>
        $<INSTALL_INTERFACE:include>)

target_link_libraries(${logr_prj} INTERFACE fmt::fmt)

list(APPEND all_logr_targets ${logr_prj} ${logr_prj}_base)

if (LOGR_WITH_SPDLOG_BACKEND)
    add_library(${logr_prj}_spdlog INTERFACE)
    add_library(${logr_prj}::${logr_prj}_spdlog ALIAS ${logr_prj}_spdlog)
    target_link_libraries(${logr_prj}_spdlog INTERFACE ${logr_prj}::${logr_prj}
                                                       spdlog::spdlog)
    list(APPEND all_logr_targets ${logr_prj}_spdlog)
endif ()

if (LOGR_WITH_GLOG_BACKEND)
    add_library(${logr_prj}_glog INTERFACE)
    add_library(${logr_prj}::${logr_prj}_glog ALIAS ${logr_prj}_glog)
    target_link_libraries(${logr_prj}_glog INTERFACE ${logr_prj}::${logr_prj}
                                                       glog::glog)
    list(APPEND all_logr_targets ${logr_prj}_glog)
endif ()

if (LOGR_WITH_LOG4CPLUS_BACKEND)
    add_library(${logr_prj}_log4cplus INTERFACE)
    add_library(${logr_prj}::${logr_prj}_log4cplus ALIAS ${logr_prj}_log4cplus)
    target_link_libraries(${logr_prj}_log4cplus INTERFACE ${logr_prj}::${logr_prj}
                                                       log4cplus::log4cplus)
    list(APPEND all_logr_targets ${logr_prj}_log4cplus)
endif ()

if (LOGR_WITH_BOOSTLOG_BACKEND)
    add_library(${logr_prj}_boostlog INTERFACE)
    add_library(${logr_prj}::${logr_prj}_boostlog ALIAS ${logr_prj}_boostlog)
    target_link_libraries(${logr_prj}_boostlog INTERFACE ${logr_prj}::${logr_prj}
                                                       Boost::log)
    list(APPEND all_logr_targets ${logr_prj}_boostlog)
endif ()
# ====================================================================

# ====================================================================
# Install
# ====================================================================
if (LOGR_INSTALL)

    include(GNUInstallDirs)
    include(CMakePackageConfigHelpers)

    set(cmake_destination ${CMAKE_INSTALL_LIBDIR}/cmake/${logr_prj})

    install(TARGETS ${all_logr_targets}
        EXPORT ${logr_prj}_project_targets
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )

    install(EXPORT ${logr_prj}_project_targets
      FILE ${logr_prj}_targets.cmake
      NAMESPACE ${logr_prj}::
          DESTINATION ${cmake_destination}
    )

    foreach (header ${logr_public_headers} )
        file(RELATIVE_PATH header_path
            ${TARGET_BUILD_INCLUDE_DIR}
            ${CMAKE_CURRENT_LIST_DIR}/${header})
        get_filename_component(relative_to_include_dir ${header_path} PATH )

        install(FILES ${header}
                DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${relative_to_include_dir} )
    endforeach ()

    # Config file if necessary:
    if (CONFIG_HEADER_PATH)
      file(RELATIVE_PATH
           config_header_path
           ${CONFIG_HEADER_INCLUDE_PATH}
           ${CONFIG_HEADER_PATH})

      get_filename_component(config_relative_to_include_dir ${config_header_path} PATH )
      install(FILES ${CONFIG_HEADER_PATH}
              DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${config_relative_to_include_dir} )
    endif ()

    set(version_config ${PROJECT_BINARY_DIR}/${logr_prj}-config-version.cmake)
    set(project_config ${PROJECT_BINARY_DIR}/${logr_prj}-config.cmake)

    configure_package_config_file(lib-config.cmake.in
                                  ${project_config}
                                  INSTALL_DESTINATION ${cmake_destination}
                                  PATH_VARS TARGET_LIB_SOVERSION)

    write_basic_package_version_file(
        ${version_config}
        VERSION ${TARGET_LIB_VERSION}
        COMPATIBILITY SameMinorVersion
        )

    # Install version, config and target files.
    install(FILES ${version_config} ${project_config}
            DESTINATION ${cmake_destination})

endif ()
# ====================================================================
