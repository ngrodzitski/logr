set(logr_test_prj _utest.logr)

project(${logr_test_prj})

list(APPEND  unittests_srcfiles
     cb_execution_elimination.cpp
     include_is_fine.cpp
     level_filtering.cpp
     levels_routing.cpp
     root_logger_type.cpp
     writeto_msg_builder_out_usages.cpp
)

add_executable(${logr_test_prj} ${unittests_srcfiles})

if (WIN32)
    target_compile_definitions(${logr_test_prj} PRIVATE _CRT_SECURE_NO_WARNINGS)
endif ()

target_link_libraries(${logr_test_prj} PRIVATE GTest::gmock_main
                                               logr::logr_base)

include(GoogleTest)

gtest_discover_tests(${logr_test_prj})
