// Logger frontend library for C++.
//
// Copyright (c) 2020 - present,  Nicolai Grodzitski
// See LICENSE file in the root of the project.

#include <benchmark/benchmark.h>

#include <logr/noop_backend.hpp>

#include "logger_usage.ipp"

namespace /* anonymous */
{

//
// bench_spdlog_logr()
//

template < typename Logger, log_srcloc_flag src_loc_flag >
void bench_logger( benchmark::State & state )
{
    auto log_level_1 = logr::log_message_level::trace;
    auto log_level_2 = logr::log_message_level::error;

    benchmark::DoNotOptimize( log_level_1 );
    benchmark::DoNotOptimize( log_level_2 );
    Logger logger{ log_level_1 };

    int x = 0L;
    int y = 0x7FFFFFFFL;

    for( auto _ : state )
    {
        logger.log_level( log_level_1 );
        do_some_logging_logr< decltype( logger ), src_loc_flag >(
            logger, x++, y-- );

        logger.log_level( log_level_2 );
        do_some_logging_logr< decltype( logger ), src_loc_flag >(
            logger, x++, y-- );
    }
}

using logger_no_fixup_t = logr::basic_noop_logger_t< 1024 >;

using logger_with_fixup_t = logr::devirt_fixup_t< logger_no_fixup_t >;

}  // anonymous namespace

BENCHMARK_TEMPLATE( bench_logger,
                    logger_no_fixup_t,
                    log_srcloc_flag::with_src_loc );
BENCHMARK_TEMPLATE( bench_logger,
                    logger_with_fixup_t,
                    log_srcloc_flag::with_src_loc );

BENCHMARK_TEMPLATE( bench_logger, logger_no_fixup_t, log_srcloc_flag::no_src_loc );
BENCHMARK_TEMPLATE( bench_logger,
                    logger_with_fixup_t,
                    log_srcloc_flag::no_src_loc );

BENCHMARK_MAIN();
