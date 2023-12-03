set(logr_benchmark_prj logr_benchmark)

project(${logr_benchmark_prj})

if (WIN32)
    set(logr_perf_flags)
else ()
    set(logr_perf_flags -fno-omit-frame-pointer -g)
endif ()

# ===============================================
# spdlog
if (LOGR_WITH_SPDLOG_BACKEND)
    add_executable(_bench.spdlog spdlog_bench.cpp)
    target_link_libraries(_bench.spdlog
                          PRIVATE logr::logr_spdlog benchmark::benchmark)

    target_compile_options(_bench.spdlog PRIVATE ${logr_perf_flags})
    target_compile_definitions(_bench.spdlog PRIVATE LOGR_WITH_SPDLOG_BACKEND )
endif ()
# ===============================================

# ===============================================
# glog
if (LOGR_WITH_GLOG_BACKEND)
    add_executable(_bench.glog glog_bench.cpp)
    target_link_libraries(_bench.glog
                          PRIVATE logr::logr_glog benchmark::benchmark)

    target_compile_options(_bench.glog PRIVATE ${logr_perf_flags})
    target_compile_definitions(_bench.glog PRIVATE LOGR_WITH_GLOG_BACKEND )
endif ()
# ===============================================

# ===============================================
# log4cplus
if (LOGR_WITH_LOG4CPLUS_BACKEND)
    add_executable(_bench.log4cplus log4cplus_bench.cpp)
    target_link_libraries(_bench.log4cplus
                          PRIVATE logr::logr_log4cplus benchmark::benchmark)

    target_compile_options(_bench.log4cplus PRIVATE ${logr_perf_flags})
    target_compile_definitions(_bench.log4cplus PRIVATE LOGR_WITH_LOG4CPLUS_BACKEND )
endif ()
# ===============================================

# ===============================================
# devirt_fixup
add_executable(_bench.devirt_fixup devirt_fixup.bench.cpp)
target_link_libraries(_bench.devirt_fixup
                      PRIVATE logr::logr_base benchmark::benchmark)

target_compile_options(_bench.devirt_fixup PRIVATE ${logr_perf_flags})
# ===============================================
