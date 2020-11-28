// Logger frontend library for C++.
//
// Copyright (c) 2020 - present,  Nicolai Grodzitski
// See LICENSE file in the root of the project.

#include <benchmark/benchmark.h>

#include <glog/logging.h>
#include <logr/glog_backend.hpp>

#include "logger_usage.ipp"

namespace /* anonymous */
{

//
// noop_glog_sink_t
//

/**
 * @brief Noop sink compatible with glog.
 */
class noop_glog_sink_t final : public google::LogSink
{
public:
    void send( google::LogSeverity severity,
               const char * full_filename,
               const char * base_filename,
               int line,
               const struct ::tm * tm_time,
               const char * message,
               size_t message_len ) final
    {
        benchmark::DoNotOptimize( severity );
        benchmark::DoNotOptimize( full_filename );
        benchmark::DoNotOptimize( base_filename );
        benchmark::DoNotOptimize( line );
        benchmark::DoNotOptimize( tm_time );
        benchmark::DoNotOptimize( message );
        benchmark::DoNotOptimize( message_len );
    }
};

//
// bench_glog_native()
//

void bench_glog_native( benchmark::State & state )
{
    noop_glog_sink_t sink;

    int x = 0L;
    int y = 0x7FFFFFFFL;

    for( auto _ : state )
    {
        do_some_logging_glog_native( &sink, x++, y-- );
    }
}

//
// bench_glog_logr()
//

template < typename Logger >
void bench_glog_logr( benchmark::State & state )
{
    auto log_level = logr::log_message_level::trace;
    benchmark::DoNotOptimize( log_level );

    noop_glog_sink_t sink;
    Logger logger{ &sink, false, log_level };

    int x = 0L;
    int y = 0x7FFFFFFFL;

    for( auto _ : state )
    {
        do_some_logging_logr( logger, x++, y-- );
    }
}

using glog_logr_t              = logr::glog_logger_t;
using glog_logr_devirt_fixup_t = logr::devirt_fixup_t< glog_logr_t >;

}  // anonymous namespace

BENCHMARK( bench_glog_native );
BENCHMARK_TEMPLATE( bench_glog_logr, glog_logr_t );
BENCHMARK_TEMPLATE( bench_glog_logr, glog_logr_devirt_fixup_t );

BENCHMARK_MAIN();
