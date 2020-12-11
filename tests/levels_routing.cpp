// Checks that message log level is routed correctly,

#include <gtest/gtest.h>

#include <logr/logr.hpp>

#include "logger_mock.hpp"

namespace /* anonymous */
{

using namespace ::testing;  // NOLINT

TEST( LogrLevelsRouting, TraceMessages )  // NOLINT
{
    logr_test::logger_mock_t<> logger( logr::log_message_level::trace );

    InSequence seq;
    EXPECT_CALL( logger, log_message_trace( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_trace( _, An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_trace( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_trace( _, An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_trace( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_trace( _, An< std::string_view >() ) );

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
}

TEST( LogrLevelsRouting, DebugMessages )  // NOLINT
{
    logr_test::logger_mock_t<> logger( logr::log_message_level::debug );

    InSequence seq;
    EXPECT_CALL( logger, log_message_debug( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_debug( _, An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_debug( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_debug( _, An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_debug( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_debug( _, An< std::string_view >() ) );

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
}

TEST( LogrLevelsRouting, InfoMessages )  // NOLINT
{
    logr_test::logger_mock_t<> logger( logr::log_message_level::info );

    InSequence seq;
    EXPECT_CALL( logger, log_message_info( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_info( _, An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_info( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_info( _, An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_info( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_info( _, An< std::string_view >() ) );

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
}

TEST( LogrLevelsRouting, WarnMessages )  // NOLINT
{
    logr_test::logger_mock_t<> logger( logr::log_message_level::warn );

    InSequence seq;
    EXPECT_CALL( logger, log_message_warn( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_warn( _, An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_warn( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_warn( _, An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_warn( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_warn( _, An< std::string_view >() ) );

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
}

TEST( LogrLevelsRouting, ErrorMessages )  // NOLINT
{
    logr_test::logger_mock_t<> logger( logr::log_message_level::error );

    InSequence seq;
    EXPECT_CALL( logger, log_message_error( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_error( _, An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_error( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_error( _, An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_error( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_error( _, An< std::string_view >() ) );

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
}

TEST( LogrLevelsRouting, CriticalMessages )  // NOLINT
{
    logr_test::logger_mock_t<> logger( logr::log_message_level::error );

    InSequence seq;
    EXPECT_CALL( logger, log_message_critical( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_critical( _, An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_critical( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_critical( _, An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_critical( An< std::string_view >() ) );
    EXPECT_CALL( logger, log_message_critical( _, An< std::string_view >() ) );

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

} /* anonymous namespace */
