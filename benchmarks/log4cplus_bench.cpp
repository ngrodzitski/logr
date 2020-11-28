// Logger frontend library for C++.
//
// Copyright (c) 2020 - present,  Nicolai Grodzitski
// See LICENSE file in the root of the project.

#include <benchmark/benchmark.h>

#include <log4cplus/appender.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/spi/loggingevent.h>
#include <logr/log4cplus_backend.hpp>
#include <log4cplus/initializer.h>

#include "logger_usage.ipp"

namespace /* anonymous */
{

//
// noop_log4cplus_appender_t
//

class noop_log4cplus_appender_t final : public log4cplus::Appender
{
public:
    void close() final {}

    noop_log4cplus_appender_t() = default;

    noop_log4cplus_appender_t( const noop_log4cplus_appender_t & ) = delete;
    noop_log4cplus_appender_t( noop_log4cplus_appender_t && )      = delete;
    noop_log4cplus_appender_t & operator=( const noop_log4cplus_appender_t & ) =
        delete;
    noop_log4cplus_appender_t & operator=( noop_log4cplus_appender_t && ) = delete;

    ~noop_log4cplus_appender_t() final { destructorImpl(); }

protected:
    void append( const log4cplus::spi::InternalLoggingEvent & event ) final
    {
        benchmark::DoNotOptimize( event );
    }
};

//
// bench_log4cplus_native()
//

void bench_log4cplus_native( benchmark::State & state )
{
    log4cplus::SharedAppenderPtr appender( new noop_log4cplus_appender_t );

    appender->setLayout( std::make_unique< log4cplus::PatternLayout >(
        LOG4CPLUS_TEXT( "%-5p [%F,%L] %m%n" ) ) );

    log4cplus::Logger logger = log4cplus::Logger::getInstance( "bench" );
    logger.addAppender( std::move( appender ) );

    auto log_level = log4cplus::TRACE_LOG_LEVEL;
    benchmark::DoNotOptimize( log_level );
    logger.setLogLevel( log_level );

    int x = 0L;
    int y = 0x7FFFFFFFL;

    for( auto _ : state )
    {
        do_some_logging_log4cplus_native( logger, x++, y-- );
    }
}

//
// bench_log4cplus_logr()
//

template < typename Logger >
void bench_log4cplus_logr( benchmark::State & state )
{
    log4cplus::SharedAppenderPtr appender( new noop_log4cplus_appender_t );

    appender->setLayout( std::make_unique< log4cplus::PatternLayout >(
        LOG4CPLUS_TEXT( "%-5p [%F,%L] %m%n" ) ) );

    log4cplus::Logger native_logger = log4cplus::Logger::getInstance( "bench" );
    native_logger.addAppender( std::move( appender ) );

    auto log_level = logr::log_message_level::trace;
    benchmark::DoNotOptimize( log_level );

    Logger logger{ std::move( native_logger ), log_level };

    int x = 0L;
    int y = 0x7FFFFFFFL;

    for( auto _ : state )
    {
        do_some_logging_logr( logger, x++, y-- );
    }
}

using log4cplus_logr_t              = logr::log4cplus_logger_t;
using log4cplus_logr_devirt_fixup_t = logr::devirt_fixup_t< log4cplus_logr_t >;

}  // anonymous namespace

// Running logr after log4cplus native shows up very different result
// compared to a run with filtered logr-run only.
// So I moved it to be benched first so it would give the same results
// as in filtered benchmark.
// But anyway running a particular bench first makes other benches results
// noticably worse than in the case of running separately (using filters).

BENCHMARK_TEMPLATE( bench_log4cplus_logr, log4cplus_logr_t );
BENCHMARK_TEMPLATE( bench_log4cplus_logr, log4cplus_logr_devirt_fixup_t );
BENCHMARK( bench_log4cplus_native );

BENCHMARK_MAIN();
