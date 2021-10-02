set(logr_example_prj logr_examples)

project(${logr_example_prj})

add_executable(ostream_logger ostream_logger.cpp)
target_link_libraries(ostream_logger
                      PRIVATE logr::logr_base)

if (LOGR_WITH_SPDLOG_BACKEND)
    add_executable(spdlog_logger spdlog_logger.cpp)
    target_link_libraries(spdlog_logger
                          PRIVATE logr::logr_spdlog)
endif ()

if (LOGR_WITH_GLOG_BACKEND)
    add_executable(glog_logger glog_logger.cpp)
    target_link_libraries(glog_logger
                          PRIVATE logr::logr_glog)
endif ()

if (LOGR_WITH_LOG4CPLUS_BACKEND)
    add_executable(log4cplus_logger log4cplus_logger.cpp)
    target_link_libraries(log4cplus_logger
                          PRIVATE logr::logr_log4cplus)
endif ()

if (LOGR_WITH_BOOSTLOG_BACKEND)
    add_executable(boostlog_logger boostlog_logger.cpp)
    target_link_libraries(boostlog_logger
                          PRIVATE logr::logr_boostlog)
endif ()
