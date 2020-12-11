// Logger frontend library for C++.
//
// Copyright (c) 2020 - present,  Nicolai Grodzitski
// See LICENSE file in the root of the project.

/**
 * @file
 *
 * Gather all logger usage routines used in benchmarks in a single file
 * to coordinate changes in them in one place.
 */

//
// log_srcloc_flag
//

/**
 * @brief An enum use as a template parameter for log usage functions.
 *
 * Some logers allow to omit source-location info,
 * so it introduces another demention for varying conditions.
 */
enum class log_srcloc_flag
{
    //! Omit source location info.
    no_src_loc,

    //! Use logger with source location.
    with_src_loc,
};

//
// do_some_logging_logr()
//

/**
 * @brief Generic logging routing for logr-base loggers.
 *
 * @tparam Logger       A logger object type.
 * @tparam Log_Src_Loc  A flag for using src location or not.
 *
 * @param logger  Logger object.
 * @param x       Sample parameter x.
 * @param y       Sample parameter y.
 */
template < typename Logger,
           log_srcloc_flag Log_Src_Loc = log_srcloc_flag::with_src_loc >
void do_some_logging_logr( Logger & logger, int x, int y )
{
    if constexpr( log_srcloc_flag::no_src_loc == Log_Src_Loc )
    {
        logger.info( "Hello World! [raw message]" );

        logger.info( []() { return "Hello World! [cb]"; } );

        logger.info( [&]( auto & out ) {
            format_to( out, "X is {}! and Y is {}", x, y );
        } );

        logger.info( [&]( auto & out ) {
            format_to( out, "Y is {}! and X is {}", y, x );
        } );

        logger.info( [&x]( auto & out ) {
            format_to( out, "X is {}! and Y is skipped", x );
        } );

        logger.info( [&y]( auto & out ) {
            format_to( out, "Y is {}! and X is skipped", y );
        } );
    }
    else
    {
        // ===========
        // Some
        // space
        // is
        // skipped
        // here
        // to
        // ensure
        // all
        // source
        // code
        // locations
        // would
        // refer
        // to
        // 3-digits
        // lines
        // Which
        // would
        // equalize
        // it
        // with
        // loggers
        // native
        // usage
        // happening
        // on
        // 3-digits
        // lines.
        // ===========

        logger.info( LOGR_SRC_LOCATION, "Hello World! [raw message]" );

        logger.info( LOGR_SRC_LOCATION, []() { return "Hello World! [cb]"; } );

        logger.info( LOGR_SRC_LOCATION, [&]( auto & out ) {
            format_to( out, "X is {}! and Y is {}", x, y );
        } );

        logger.info( LOGR_SRC_LOCATION, [&]( auto & out ) {
            format_to( out, "Y is {}! and X is {}", y, x );
        } );

        logger.info( LOGR_SRC_LOCATION, [&x]( auto & out ) {
            format_to( out, "X is {}! and Y is skipped", x );
        } );

        logger.info( LOGR_SRC_LOCATION, [&y]( auto & out ) {
            format_to( out, "Y is {}! and X is skipped", y );
        } );
    }
}

#if defined( LOGR_WITH_SPDLOG_BACKEND )

//
// do_some_logging_spdlog_native()
//

/**
 * @brief Log a series of mesages with spdlog standard approach.
 */
template < log_srcloc_flag log_srcloc >
void do_some_logging_spdlog_native( spdlog::logger & logger, int x, int y )
{
    if constexpr( log_srcloc_flag::with_src_loc == log_srcloc )
    {
        SPDLOG_LOGGER_INFO( &logger, "Hello World! [raw message]" );
        SPDLOG_LOGGER_INFO( &logger, "Hello World! [cb]" );

        SPDLOG_LOGGER_INFO( &logger, "X is {}! and Y is {}", x, y );
        SPDLOG_LOGGER_INFO( &logger, "Y is {}! and X is {}", y, x );

        SPDLOG_LOGGER_INFO( &logger, "X is {}! and Y is skipped", x );
        SPDLOG_LOGGER_INFO( &logger, "Y is {}! and X is skipped", y );
    }
    else
    {
        logger.info( "Hello World! [raw message]" );
        logger.info( "Hello World! [cb]" );

        logger.info( "X is {}! and Y is {}", x, y );
        logger.info( "Y is {}! and X is {}", y, x );

        logger.info( "X is {}! and Y is skipped", x );
        logger.info( "Y is {}! and X is skipped", y );
    }
}
#endif  // defined(LOGR_WITH_SPDLOG_BACKEND)

#if defined( LOGR_WITH_GLOG_BACKEND )

//
// do_some_logging_glog_native()
//

/**
 * @brief Log a series of mesages with glog standard approach.
 */
void do_some_logging_glog_native( google::LogSink * sink, int x, int y )
{
    LOG_TO_SINK_BUT_NOT_TO_LOGFILE( sink, INFO ) << "Hello World! [raw message]";
    LOG_TO_SINK_BUT_NOT_TO_LOGFILE( sink, INFO ) << "Hello World! [cb]";

    LOG_TO_SINK_BUT_NOT_TO_LOGFILE( sink, INFO )
        << "X is " << x << "! and Y is " << y;
    LOG_TO_SINK_BUT_NOT_TO_LOGFILE( sink, INFO )
        << "Y is " << y << "! and X is " << x;

    LOG_TO_SINK_BUT_NOT_TO_LOGFILE( sink, INFO )
        << "X is " << x << "! and Y is skipped";
    LOG_TO_SINK_BUT_NOT_TO_LOGFILE( sink, INFO )
        << "Y is " << y << "! and X is skipped";
}

#endif  // defined(LOGR_WITH_GLOG_BACKEND)

#if defined( LOGR_WITH_LOG4CPLUS_BACKEND )

//
// do_some_logging_log4cplus_native()
//

/**
 * @brief Log a series of mesages with log4cplus standard approach.
 */
void do_some_logging_log4cplus_native( log4cplus::Logger & logger, int x, int y )
{
    LOG4CPLUS_INFO( logger, "Hello World! [raw message]" );
    LOG4CPLUS_INFO( logger, "Hello World! [cb]" );

    LOG4CPLUS_INFO( logger, "X is " << x << "! and Y is " << y );
    LOG4CPLUS_INFO( logger, "Y is " << y << "! and X is " << x );

    LOG4CPLUS_INFO( logger, "X is " << x << "! and Y is skipped" );
    LOG4CPLUS_INFO( logger, "Y is " << y << "! and X is skipped" );
}

#endif  // defined(LOGR_WITH_LOG4CPLUS_BACKEND)
