// Check that all write-to message builders styles work the same.

#include <gtest/gtest.h>

#include <logr/logr.hpp>

#include "logger_mock.hpp"

namespace /* anonymous */
{

using namespace ::testing;  // NOLINT

using mock_logger_t = StrictMock< logr_test::logger_mock_t<> >;

TEST( LogrWriteToMsgBuilderStyles, OutByValue )  // NOLINT
{
    mock_logger_t logger( logr::log_message_level::trace );

    InSequence seq;

    int x         = 42;
    std::string s = "QWERTY";

    EXPECT_CALL(
        logger,
        log_message_trace( std::string_view{ "Test message 42 \"QWERTY\"" } ) );
    EXPECT_CALL(
        logger,
        log_message_trace( std::string_view{ "Test message 42 \"QWERTY\"" } ) );
    EXPECT_CALL(
        logger,
        log_message_trace( std::string_view{ "Test message 42 \"QWERTY\"" } ) );
    EXPECT_CALL(
        logger,
        log_message_trace( _, std::string_view{ "Test message 42 \"QWERTY\"" } ) );
    EXPECT_CALL(
        logger,
        log_message_trace( _, std::string_view{ "Test message 42 \"QWERTY\"" } ) );
    EXPECT_CALL(
        logger,
        log_message_trace( _, std::string_view{ "Test message 42 \"QWERTY\"" } ) );

    logger.trace( [x, &s]( auto out ) {
        format_to( out, "Test message {} \"{}\"", x, s );
    } );

    logger.trace(
        [x, &s]( auto out ) { out.format_to( "Test message {} \"{}\"", x, s ); } );

    logger.trace( [x, &s]( auto out ) {
        ::fmt::format_to( out.buf(), "Test message {} \"{}\"", x, s );
    } );

    logger.trace( LOGR_SRC_LOCATION, [x, &s]( auto out ) {
        format_to( out, "Test message {} \"{}\"", x, s );
    } );

    logger.trace( LOGR_SRC_LOCATION, [x, &s]( auto out ) {
        out.format_to( "Test message {} \"{}\"", x, s );
    } );

    logger.trace( LOGR_SRC_LOCATION, [x, &s]( auto out ) {
        ::fmt::format_to( out.buf(), "Test message {} \"{}\"", x, s );
    } );
}

TEST( LogrWriteToMsgBuilderStyles, OutByReference )  // NOLINT
{
    mock_logger_t logger( logr::log_message_level::trace );

    InSequence seq;

    int x         = 42;
    std::string s = "QWERTY";

    EXPECT_CALL(
        logger,
        log_message_trace( std::string_view{ "Test message 42 \"QWERTY\"" } ) );
    EXPECT_CALL(
        logger,
        log_message_trace( std::string_view{ "Test message 42 \"QWERTY\"" } ) );
    EXPECT_CALL(
        logger,
        log_message_trace( std::string_view{ "Test message 42 \"QWERTY\"" } ) );
    EXPECT_CALL(
        logger,
        log_message_trace( _, std::string_view{ "Test message 42 \"QWERTY\"" } ) );
    EXPECT_CALL(
        logger,
        log_message_trace( _, std::string_view{ "Test message 42 \"QWERTY\"" } ) );
    EXPECT_CALL(
        logger,
        log_message_trace( _, std::string_view{ "Test message 42 \"QWERTY\"" } ) );

    logger.trace( [x, &s]( auto & out ) {
        format_to( out, "Test message {} \"{}\"", x, s );
    } );

    logger.trace( [x, &s]( auto & out ) {
        out.format_to( "Test message {} \"{}\"", x, s );
    } );

    logger.trace( [x, &s]( auto & out ) {
        ::fmt::format_to( out.buf(), "Test message {} \"{}\"", x, s );
    } );

    logger.trace( LOGR_SRC_LOCATION, [x, &s]( auto & out ) {
        format_to( out, "Test message {} \"{}\"", x, s );
    } );

    logger.trace( LOGR_SRC_LOCATION, [x, &s]( auto & out ) {
        out.format_to( "Test message {} \"{}\"", x, s );
    } );

    logger.trace( LOGR_SRC_LOCATION, [x, &s]( auto & out ) {
        ::fmt::format_to( out.buf(), "Test message {} \"{}\"", x, s );
    } );
}

}  // anonymous namespace
