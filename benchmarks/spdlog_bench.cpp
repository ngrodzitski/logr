// Logger frontend library for C++.
//
// Copyright (c) 2020 - present,  Nicolai Grodzitski
// See LICENSE file in the root of the project.

#include <benchmark/benchmark.h>

#include <spdlog/sinks/base_sink.h>
#include <logr/spdlog_backend.hpp>

// Spdlog uses fmt either, and leverages its mem-buffer
// So for fair compitition the same buffer size should be used.
// https://github.com/gabime/spdlog/blob/4a9ccf7e38e257feecce0c579a782741254eaeef/include/spdlog/common.h#L103
constexpr std::size_t spd_log_fmt_membuffer_size = 250UL;

// Anoother point for fair comparison is using spdlog without exceptions.
// That is provided by `conanfile`.

#include "logger_usage.ipp"

namespace /* anonymous */
{

template < typename Mutex >
class noop_spdlog_sink_t final : public spdlog::sinks::base_sink< Mutex >
{
protected:
    void sink_it_( const spdlog::details::log_msg & msg ) final
    {
        benchmark::DoNotOptimize( msg );
    }

    void flush_() final {}
};

//
// bench_spdlog_native()
//

template < typename Mutex, log_srcloc_flag src_loc_flag >
void bench_spdlog_native( benchmark::State & state )
{
    spdlog::logger logger{ "bench",
                           std::make_shared< noop_spdlog_sink_t< Mutex > >() };
    auto log_level = spdlog::level::trace;
    benchmark::DoNotOptimize( log_level );
    logger.set_level( log_level );

    int x = 0L;
    int y = 0x7FFFFFFFL;

    for( auto _ : state )
    {
        do_some_logging_spdlog_native< src_loc_flag >( logger, x++, y-- );
    }
}

//
// bench_spdlog_logr()
//

template < typename Logger, typename Mutex, log_srcloc_flag src_loc_flag >
void bench_spdlog_logr( benchmark::State & state )
{
    auto log_level = logr::log_message_level::trace;
    benchmark::DoNotOptimize( log_level );

    Logger logger{ "bench",
                   std::make_shared< noop_spdlog_sink_t< Mutex > >(),
                   log_level };

    int x = 0L;
    int y = 0x7FFFFFFFL;

    for( auto _ : state )
    {
        do_some_logging_logr< decltype( logger ), src_loc_flag >(
            logger, x++, y-- );
    }
}

using spdlog_logr_t = logr::spdlog_logger_t< spd_log_fmt_membuffer_size >;

using spdlog_logr_devirt_fixup_t = logr::devirt_fixup_t< spdlog_logr_t >;

}  // anonymous namespace

BENCHMARK_TEMPLATE( bench_spdlog_native,
                    std::mutex,
                    log_srcloc_flag::with_src_loc );
BENCHMARK_TEMPLATE( bench_spdlog_logr,
                    spdlog_logr_t,
                    std::mutex,
                    log_srcloc_flag::with_src_loc );
BENCHMARK_TEMPLATE( bench_spdlog_logr,
                    spdlog_logr_devirt_fixup_t,
                    std::mutex,
                    log_srcloc_flag::with_src_loc );

BENCHMARK_TEMPLATE( bench_spdlog_native, std::mutex, log_srcloc_flag::no_src_loc );
BENCHMARK_TEMPLATE( bench_spdlog_logr,
                    spdlog_logr_t,
                    std::mutex,
                    log_srcloc_flag::no_src_loc );
BENCHMARK_TEMPLATE( bench_spdlog_logr,
                    spdlog_logr_devirt_fixup_t,
                    std::mutex,
                    log_srcloc_flag::no_src_loc );

BENCHMARK_TEMPLATE( bench_spdlog_native,
                    spdlog::details::null_mutex,
                    log_srcloc_flag::with_src_loc );
BENCHMARK_TEMPLATE( bench_spdlog_logr,
                    spdlog_logr_t,
                    spdlog::details::null_mutex,
                    log_srcloc_flag::with_src_loc );
BENCHMARK_TEMPLATE( bench_spdlog_logr,
                    spdlog_logr_devirt_fixup_t,
                    spdlog::details::null_mutex,
                    log_srcloc_flag::with_src_loc );

BENCHMARK_TEMPLATE( bench_spdlog_native,
                    spdlog::details::null_mutex,
                    log_srcloc_flag::no_src_loc );
BENCHMARK_TEMPLATE( bench_spdlog_logr,
                    spdlog_logr_t,
                    spdlog::details::null_mutex,
                    log_srcloc_flag::no_src_loc );
BENCHMARK_TEMPLATE( bench_spdlog_logr,
                    spdlog_logr_devirt_fixup_t,
                    spdlog::details::null_mutex,
                    log_srcloc_flag::no_src_loc );

BENCHMARK_MAIN();
