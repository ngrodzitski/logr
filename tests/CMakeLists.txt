set(logr_test_prj _utest.logr)

project(${logr_test_prj})

append_src_files(
    TARGET unittests_srcfiles
    GLOB_PATTERN
        *.cpp )

add_executable(${logr_test_prj} ${unittests_srcfiles})

if (WIN32)
    target_compile_definitions(${logr_test_prj} PRIVATE _CRT_SECURE_NO_WARNINGS)
endif ()

target_link_libraries(${logr_test_prj} PRIVATE GTest::gmock_main
                                               logr::logr)

include(GoogleTest)

gtest_discover_tests(${logr_test_prj})
