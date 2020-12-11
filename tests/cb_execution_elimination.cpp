// Check that message producing callback are not executed
// for log levels less severe than currently configured one.

#include <gtest/gtest.h>

#include <logr/logr.hpp>
#include <logr/null_backend.hpp>

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
std::tuple< bool, bool, bool, bool, bool, bool > do_log_for_all_log_levels(
    Logger & logger )
{
    bool trace_cb_was_called{ false };
    bool debug_cb_was_called{ false };
    bool info_cb_was_called{ false };
    bool warn_cb_was_called{ false };
    bool error_cb_was_called{ false };
    bool critical_cb_was_called{ false };

    logger.trace( [&]() {
        trace_cb_was_called = true;
        return "test [cb]";
    } );
    logger.trace( LOGR_SRC_LOCATION, [&]() {
        trace_cb_was_called = true;
        return "test [cb]";
    } );

    logger.trace( [&]( auto & out ) {
        trace_cb_was_called = true;
        format_to( out, "{} [{}]", "test", "cb with explicit out" );
    } );

    logger.trace( LOGR_SRC_LOCATION, [&]( auto & out ) {
        trace_cb_was_called = true;
        format_to( out, "{} [{}]", "test", "cb with explicit out" );
    } );

    logger.debug( [&]() {
        debug_cb_was_called = true;
        return "test [cb]";
    } );
    logger.debug( LOGR_SRC_LOCATION, [&]() {
        debug_cb_was_called = true;
        return "test [cb]";
    } );

    logger.debug( [&]( auto & out ) {
        debug_cb_was_called = true;
        format_to( out, "{} [{}]", "test", "cb with explicit out" );
    } );

    logger.debug( LOGR_SRC_LOCATION, [&]( auto & out ) {
        debug_cb_was_called = true;
        format_to( out, "{} [{}]", "test", "cb with explicit out" );
    } );

    logger.info( [&]() {
        info_cb_was_called = true;
        return "test [cb]";
    } );
    logger.info( LOGR_SRC_LOCATION, [&]() {
        info_cb_was_called = true;
        return "test [cb]";
    } );

    logger.info( [&]( auto & out ) {
        info_cb_was_called = true;
        format_to( out, "{} [{}]", "test", "cb with explicit out" );
    } );

    logger.info( LOGR_SRC_LOCATION, [&]( auto & out ) {
        info_cb_was_called = true;
        format_to( out, "{} [{}]", "test", "cb with explicit out" );
    } );

    logger.warn( [&]() {
        warn_cb_was_called = true;
        return "test [cb]";
    } );
    logger.warn( LOGR_SRC_LOCATION, [&]() {
        warn_cb_was_called = true;
        return "test [cb]";
    } );

    logger.warn( [&]( auto & out ) {
        warn_cb_was_called = true;
        format_to( out, "{} [{}]", "test", "cb with explicit out" );
    } );

    logger.warn( LOGR_SRC_LOCATION, [&]( auto & out ) {
        warn_cb_was_called = true;
        format_to( out, "{} [{}]", "test", "cb with explicit out" );
    } );

    logger.error( [&]() {
        error_cb_was_called = true;
        return "test [cb]";
    } );
    logger.error( LOGR_SRC_LOCATION, [&]() {
        error_cb_was_called = true;
        return "test [cb]";
    } );

    logger.error( [&]( auto & out ) {
        error_cb_was_called = true;
        format_to( out, "{} [{}]", "test", "cb with explicit out" );
    } );

    logger.error( LOGR_SRC_LOCATION, [&]( auto & out ) {
        error_cb_was_called = true;
        format_to( out, "{} [{}]", "test", "cb with explicit out" );
    } );

    logger.critical( [&]() {
        critical_cb_was_called = true;
        return "test [cb]";
    } );
    logger.critical( LOGR_SRC_LOCATION, [&]() {
        critical_cb_was_called = true;
        return "test [cb]";
    } );

    logger.critical( [&]( auto & out ) {
        critical_cb_was_called = true;
        format_to( out, "{} [{}]", "test", "cb with explicit out" );
    } );

    logger.critical( LOGR_SRC_LOCATION, [&]( auto & out ) {
        critical_cb_was_called = true;
        format_to( out, "{} [{}]", "test", "cb with explicit out" );
    } );

    return std::tie( trace_cb_was_called,
                     debug_cb_was_called,
                     info_cb_was_called,
                     warn_cb_was_called,
                     error_cb_was_called,
                     critical_cb_was_called );
}

constexpr size_t static_buffer_size = 128UL;

//
// dummy_logger_t
//

class dummy_logger_t final
    : public logr::basic_logger_t<
          logr::basic_logger_traits_t< static_buffer_size > >
{
public:
    using base_type_t =
        logr::basic_logger_t< logr::basic_logger_traits_t< static_buffer_size > >;

    explicit dummy_logger_t( logr::log_message_level level )
        : base_type_t{ level }
    {
    }

private:
    // NOLINTNEXTLINE(readability-named-parameter)
    void log_message_trace( std::string_view ) override {}

    // NOLINTNEXTLINE(readability-named-parameter)
    void log_message_trace( logr::src_location_t, std::string_view ) override {}

    // NOLINTNEXTLINE(readability-named-parameter)
    void log_message_debug( std::string_view ) override {}

    // NOLINTNEXTLINE(readability-named-parameter)
    void log_message_debug( logr::src_location_t, std::string_view ) override {}

    // NOLINTNEXTLINE(readability-named-parameter)
    void log_message_info( std::string_view ) override {}

    // NOLINTNEXTLINE(readability-named-parameter)
    void log_message_info( logr::src_location_t, std::string_view ) override {}

    // NOLINTNEXTLINE(readability-named-parameter)
    void log_message_warn( std::string_view ) override {}

    // NOLINTNEXTLINE(readability-named-parameter)
    void log_message_warn( logr::src_location_t, std::string_view ) override {}

    // NOLINTNEXTLINE(readability-named-parameter)
    void log_message_error( std::string_view ) override {}

    // NOLINTNEXTLINE(readability-named-parameter)
    void log_message_error( logr::src_location_t, std::string_view ) override {}

    // NOLINTNEXTLINE(readability-named-parameter)
    void log_message_critical( std::string_view ) override {}

    // NOLINTNEXTLINE(readability-named-parameter)
    void log_message_critical( logr::src_location_t, std::string_view ) override {}

    void log_flush() override {}
};

TEST( LogrCbExecutionElimination, TestTraceLevel )  // NOLINT
{
    dummy_logger_t logger( logr::log_message_level::trace );

    const auto [ trace_cb_was_called,
                 debug_cb_was_called,
                 info_cb_was_called,
                 warn_cb_was_called,
                 error_cb_was_called,
                 critical_cb_was_called ] = do_log_for_all_log_levels( logger );

    EXPECT_TRUE( trace_cb_was_called );
    EXPECT_TRUE( debug_cb_was_called );
    EXPECT_TRUE( info_cb_was_called );
    EXPECT_TRUE( warn_cb_was_called );
    EXPECT_TRUE( error_cb_was_called );
    EXPECT_TRUE( critical_cb_was_called );
}

TEST( LogrCbExecutionElimination, TestDebugLevel )  // NOLINT
{
    dummy_logger_t logger( logr::log_message_level::debug );

    const auto [ trace_cb_was_called,
                 debug_cb_was_called,
                 info_cb_was_called,
                 warn_cb_was_called,
                 error_cb_was_called,
                 critical_cb_was_called ] = do_log_for_all_log_levels( logger );

    EXPECT_FALSE( trace_cb_was_called );
    EXPECT_TRUE( debug_cb_was_called );
    EXPECT_TRUE( info_cb_was_called );
    EXPECT_TRUE( warn_cb_was_called );
    EXPECT_TRUE( error_cb_was_called );
    EXPECT_TRUE( critical_cb_was_called );
}

TEST( LogrCbExecutionElimination, TestInfoLevel )  // NOLINT
{
    dummy_logger_t logger( logr::log_message_level::info );

    const auto [ trace_cb_was_called,
                 debug_cb_was_called,
                 info_cb_was_called,
                 warn_cb_was_called,
                 error_cb_was_called,
                 critical_cb_was_called ] = do_log_for_all_log_levels( logger );

    EXPECT_FALSE( trace_cb_was_called );
    EXPECT_FALSE( debug_cb_was_called );
    EXPECT_TRUE( info_cb_was_called );
    EXPECT_TRUE( warn_cb_was_called );
    EXPECT_TRUE( error_cb_was_called );
    EXPECT_TRUE( critical_cb_was_called );
}

TEST( LogrCbExecutionElimination, TestWarnLevel )  // NOLINT
{
    dummy_logger_t logger( logr::log_message_level::warn );

    const auto [ trace_cb_was_called,
                 debug_cb_was_called,
                 info_cb_was_called,
                 warn_cb_was_called,
                 error_cb_was_called,
                 critical_cb_was_called ] = do_log_for_all_log_levels( logger );

    EXPECT_FALSE( trace_cb_was_called );
    EXPECT_FALSE( debug_cb_was_called );
    EXPECT_FALSE( info_cb_was_called );
    EXPECT_TRUE( warn_cb_was_called );
    EXPECT_TRUE( error_cb_was_called );
    EXPECT_TRUE( critical_cb_was_called );
}

TEST( LogrCbExecutionElimination, TestErrorLevel )  // NOLINT
{
    dummy_logger_t logger( logr::log_message_level::error );

    const auto [ trace_cb_was_called,
                 debug_cb_was_called,
                 info_cb_was_called,
                 warn_cb_was_called,
                 error_cb_was_called,
                 critical_cb_was_called ] = do_log_for_all_log_levels( logger );

    EXPECT_FALSE( trace_cb_was_called );
    EXPECT_FALSE( debug_cb_was_called );
    EXPECT_FALSE( info_cb_was_called );
    EXPECT_FALSE( warn_cb_was_called );
    EXPECT_TRUE( error_cb_was_called );
    EXPECT_TRUE( critical_cb_was_called );
}

TEST( LogrCbExecutionElimination, TestCriticalLevel )  // NOLINT
{
    dummy_logger_t logger( logr::log_message_level::critical );

    const auto [ trace_cb_was_called,
                 debug_cb_was_called,
                 info_cb_was_called,
                 warn_cb_was_called,
                 error_cb_was_called,
                 critical_cb_was_called ] = do_log_for_all_log_levels( logger );

    EXPECT_FALSE( trace_cb_was_called );
    EXPECT_FALSE( debug_cb_was_called );
    EXPECT_FALSE( info_cb_was_called );
    EXPECT_FALSE( warn_cb_was_called );
    EXPECT_FALSE( error_cb_was_called );
    EXPECT_TRUE( critical_cb_was_called );
}

TEST( LogrCbExecutionElimination, TestNologLevel )  // NOLINT
{
    dummy_logger_t logger( logr::log_message_level::nolog );

    const auto [ trace_cb_was_called,
                 debug_cb_was_called,
                 info_cb_was_called,
                 warn_cb_was_called,
                 error_cb_was_called,
                 critical_cb_was_called ] = do_log_for_all_log_levels( logger );

    EXPECT_FALSE( trace_cb_was_called );
    EXPECT_FALSE( debug_cb_was_called );
    EXPECT_FALSE( info_cb_was_called );
    EXPECT_FALSE( warn_cb_was_called );
    EXPECT_FALSE( error_cb_was_called );
    EXPECT_FALSE( critical_cb_was_called );
}

TEST( LogrCbExecutionElimination, TestNullBackend )  // NOLINT
{
    logr::null_logger_t<> logger{};

    const auto [ trace_cb_was_called,
                 debug_cb_was_called,
                 info_cb_was_called,
                 warn_cb_was_called,
                 error_cb_was_called,
                 critical_cb_was_called ] = do_log_for_all_log_levels( logger );

    EXPECT_FALSE( trace_cb_was_called );
    EXPECT_FALSE( debug_cb_was_called );
    EXPECT_FALSE( info_cb_was_called );
    EXPECT_FALSE( warn_cb_was_called );
    EXPECT_FALSE( error_cb_was_called );
    EXPECT_FALSE( critical_cb_was_called );
}

} /* anonymous namespace */
