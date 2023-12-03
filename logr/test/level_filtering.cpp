// Check that messages severity filters work.

#include <gtest/gtest.h>

#include <logr/logr.hpp>

#include "logger_mock.hpp"

namespace /* anonymous */
{

using namespace ::testing;  // NOLINT

//
// do_log_for_all_log_levels()
//

/**
 * @brief Logs a message for all log levels and all variants of log producer.
 */
template < typename Logger >
void do_log_for_all_log_levels( Logger & logger )
{
    logger.trace( "test [raw message]" );
    logger.trace( LOGR_SRC_LOCATION, "test [raw message]" );

    logger.trace( []() { return "test [cb]"; } );
    logger.trace( LOGR_SRC_LOCATION, []() { return "test [cb]"; } );

    logger.trace( []( auto & out ) {
        format_to( out, "{} [{}]", "test", "cb with explicit out" );
    } );

    logger.trace( LOGR_SRC_LOCATION, []( auto & out ) {
        format_to( out, "{} [{}]", "test", "cb with explicit out" );
    } );

    logger.debug( "test [raw message]" );
    logger.debug( LOGR_SRC_LOCATION, "test [raw message]" );

    logger.debug( []() { return "test [cb]"; } );
    logger.debug( LOGR_SRC_LOCATION, []() { return "test [cb]"; } );

    logger.debug( []( auto & out ) {
        format_to( out, "{} [{}]", "test", "cb with explicit out" );
    } );

    logger.debug( LOGR_SRC_LOCATION, []( auto & out ) {
        format_to( out, "{} [{}]", "test", "cb with explicit out" );
    } );

    logger.info( "test [raw message]" );
    logger.info( LOGR_SRC_LOCATION, "test [raw message]" );

    logger.info( []() { return "test [cb]"; } );
    logger.info( LOGR_SRC_LOCATION, []() { return "test [cb]"; } );

    logger.info( []( auto & out ) {
        format_to( out, "{} [{}]", "test", "cb with explicit out" );
    } );

    logger.info( LOGR_SRC_LOCATION, []( auto & out ) {
        format_to( out, "{} [{}]", "test", "cb with explicit out" );
    } );

    logger.warn( "test [raw message]" );
    logger.warn( LOGR_SRC_LOCATION, "test [raw message]" );

    logger.warn( []() { return "test [cb]"; } );
    logger.warn( LOGR_SRC_LOCATION, []() { return "test [cb]"; } );

    logger.warn( []( auto & out ) {
        format_to( out, "{} [{}]", "test", "cb with explicit out" );
    } );

    logger.warn( LOGR_SRC_LOCATION, []( auto & out ) {
        format_to( out, "{} [{}]", "test", "cb with explicit out" );
    } );

    logger.error( "test [raw message]" );
    logger.error( LOGR_SRC_LOCATION, "test [raw message]" );

    logger.error( []() { return "test [cb]"; } );
    logger.error( LOGR_SRC_LOCATION, []() { return "test [cb]"; } );

    logger.error( []( auto & out ) {
        format_to( out, "{} [{}]", "test", "cb with explicit out" );
    } );

    logger.error( LOGR_SRC_LOCATION, []( auto & out ) {
        format_to( out, "{} [{}]", "test", "cb with explicit out" );
    } );

    logger.critical( "test [raw message]" );
    logger.critical( LOGR_SRC_LOCATION, "test [raw message]" );

    logger.critical( []() { return "test [cb]"; } );
    logger.critical( LOGR_SRC_LOCATION, []() { return "test [cb]"; } );

    logger.critical( []( auto & out ) {
        format_to( out, "{} [{}]", "test", "cb with explicit out" );
    } );

    logger.critical( LOGR_SRC_LOCATION, []( auto & out ) {
        format_to( out, "{} [{}]", "test", "cb with explicit out" );
    } );
}

using mock_logger_t = StrictMock< logr_test::logger_mock_t<> >;

TEST( LogrLevelsFiltering, TestTraceLevel )  // NOLINT
{
    mock_logger_t logger( logr::log_message_level::trace );

    InSequence seq;

    EXPECT_CALL( logger, log_message_trace( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_trace( _, An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_trace( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_trace( _, An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_trace( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_trace( _, An< std::string_view >() ) );

    EXPECT_CALL( logger, log_message_debug( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_debug( _, An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_debug( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_debug( _, An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_debug( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_debug( _, An< std::string_view >() ) );

    EXPECT_CALL( logger, log_message_info( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_info( _, An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_info( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_info( _, An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_info( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_info( _, An< std::string_view >() ) );

    EXPECT_CALL( logger, log_message_warn( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_warn( _, An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_warn( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_warn( _, An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_warn( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_warn( _, An< std::string_view >() ) );

    EXPECT_CALL( logger, log_message_error( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_error( _, An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_error( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_error( _, An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_error( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_error( _, An< std::string_view >() ) );

    EXPECT_CALL( logger, log_message_critical( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_critical( _, An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_critical( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_critical( _, An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_critical( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_critical( _, An< std::string_view >() ) );

    do_log_for_all_log_levels( logger );
}

TEST( LogrLevelsFiltering, TestDebugLevel )  // NOLINT
{
    mock_logger_t logger( logr::log_message_level::debug );

    InSequence seq;

    EXPECT_CALL( logger, log_message_debug( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_debug( _, An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_debug( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_debug( _, An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_debug( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_debug( _, An< std::string_view >() ) );

    EXPECT_CALL( logger, log_message_info( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_info( _, An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_info( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_info( _, An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_info( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_info( _, An< std::string_view >() ) );

    EXPECT_CALL( logger, log_message_warn( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_warn( _, An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_warn( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_warn( _, An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_warn( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_warn( _, An< std::string_view >() ) );

    EXPECT_CALL( logger, log_message_error( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_error( _, An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_error( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_error( _, An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_error( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_error( _, An< std::string_view >() ) );

    EXPECT_CALL( logger, log_message_critical( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_critical( _, An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_critical( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_critical( _, An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_critical( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_critical( _, An< std::string_view >() ) );

    do_log_for_all_log_levels( logger );
}

TEST( LogrLevelsFiltering, TestInfoLevel )  // NOLINT
{
    mock_logger_t logger( logr::log_message_level::info );

    InSequence seq;

    EXPECT_CALL( logger, log_message_info( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_info( _, An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_info( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_info( _, An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_info( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_info( _, An< std::string_view >() ) );

    EXPECT_CALL( logger, log_message_warn( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_warn( _, An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_warn( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_warn( _, An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_warn( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_warn( _, An< std::string_view >() ) );

    EXPECT_CALL( logger, log_message_error( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_error( _, An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_error( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_error( _, An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_error( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_error( _, An< std::string_view >() ) );

    EXPECT_CALL( logger, log_message_critical( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_critical( _, An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_critical( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_critical( _, An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_critical( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_critical( _, An< std::string_view >() ) );

    do_log_for_all_log_levels( logger );
}

TEST( LogrLevelsFiltering, TestWarnLevel )  // NOLINT
{
    mock_logger_t logger( logr::log_message_level::warn );

    InSequence seq;

    EXPECT_CALL( logger, log_message_warn( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_warn( _, An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_warn( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_warn( _, An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_warn( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_warn( _, An< std::string_view >() ) );

    EXPECT_CALL( logger, log_message_error( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_error( _, An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_error( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_error( _, An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_error( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_error( _, An< std::string_view >() ) );

    EXPECT_CALL( logger, log_message_critical( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_critical( _, An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_critical( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_critical( _, An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_critical( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_critical( _, An< std::string_view >() ) );

    do_log_for_all_log_levels( logger );
}

TEST( LogrLevelsFiltering, TestErrorLevel )  // NOLINT
{
    mock_logger_t logger( logr::log_message_level::error );

    InSequence seq;

    EXPECT_CALL( logger, log_message_error( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_error( _, An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_error( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_error( _, An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_error( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_error( _, An< std::string_view >() ) );

    EXPECT_CALL( logger, log_message_critical( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_critical( _, An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_critical( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_critical( _, An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_critical( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_critical( _, An< std::string_view >() ) );

    do_log_for_all_log_levels( logger );
}

TEST( LogrLevelsFiltering, TestCriticalLevel )  // NOLINT
{
    mock_logger_t logger( logr::log_message_level::critical );

    InSequence seq;

    EXPECT_CALL( logger, log_message_critical( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_critical( _, An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_critical( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_critical( _, An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_critical( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_critical( _, An< std::string_view >() ) );

    do_log_for_all_log_levels( logger );
}

TEST( LogrLevelsFiltering, TestNologLevel )  // NOLINT
{
    mock_logger_t logger( logr::log_message_level::nolog );

    InSequence seq;

    do_log_for_all_log_levels( logger );
}

} /* anonymous namespace */
