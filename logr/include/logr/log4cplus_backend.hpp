// Logger frontend library for C++.
//
// Copyright (c) 2020 - present,  Nicolai Grodzitski
// See LICENSE file in the root of the project.

#pragma once

#include <log4cplus/logger.h>
#include <log4cplus/loglevel.h>

#include <logr/logr.hpp>

namespace logr
{

//
// log4cplus_logger_traits_t
//

template < std::size_t Inline_Size,
           typename Allocator = std::allocator< log4cplus::tchar > >
struct log4cplus_logger_traits_t
    : public basic_logger_traits_t< Inline_Size, log4cplus::tchar, Allocator >
{
};

//
// basic_log4cplus_logger_t
//

/**
 * @brief A logger using log4cplus (https://github.com/google/log4cplus)
 * as a backend.
 *
 * Implements logr::basic_logger_t for writing log-messages to log4cplus.
 * Depending on log4cplus unicode-ness it is a char-based or wshac-based.
 */
template < std::size_t Inline_Size,
           typename Allocator = std::allocator< log4cplus::tchar > >
class basic_log4cplus_logger_t
    : public basic_logger_t< log4cplus_logger_traits_t< Inline_Size, Allocator > >
{
public:
    using string_view_t = std::basic_string_view< log4cplus::tchar >;
    using base_type_t =
        basic_logger_t< log4cplus_logger_traits_t< Inline_Size, Allocator > >;
    using message_container_t = typename base_type_t::message_container_t;

    explicit basic_log4cplus_logger_t(
        log4cplus::Logger logger,
        log_message_level level = log_message_level::error,
        const Allocator & alloc = Allocator() )
        : base_type_t{ level, alloc }
        , m_logger{ std::move( logger ) }
    {
    }

private:
    void dispatch_message( log4cplus::LogLevel log_level,
                           string_view_t message,
                           const char * file,
                           int line = -1 )
    {
        // @ngrodzitski:
        // Current version available in Conan expects `const std::string &`
        // as a message parameter, so we have to do the following.
        // This is changed to `string_view` in master, though it
        // still requires a reference.
        const std::basic_string< log4cplus::tchar > msg{ message.data(),
                                                         message.size() };
        m_logger.forcedLog( log_level, msg, file, line, "" );
    }

    void log_message_trace( string_view_t message ) final
    {
        dispatch_message( log4cplus::TRACE_LOG_LEVEL, message, "" );
    }

    void log_message_trace( src_location_t src_location,
                            string_view_t message ) final
    {
        dispatch_message( log4cplus::TRACE_LOG_LEVEL,
                          message,
                          src_location.file,
                          src_location.line );
    }

    void log_message_debug( string_view_t message ) final
    {
        dispatch_message( log4cplus::DEBUG_LOG_LEVEL, message, nullptr );
    }

    void log_message_debug( src_location_t src_location,
                            string_view_t message ) final
    {
        dispatch_message( log4cplus::DEBUG_LOG_LEVEL,
                          message,
                          src_location.file,
                          src_location.line );
    }

    void log_message_info( string_view_t message ) final
    {
        dispatch_message( log4cplus::INFO_LOG_LEVEL, message, nullptr );
    }

    void log_message_info( src_location_t src_location,
                           string_view_t message ) final
    {
        dispatch_message( log4cplus::INFO_LOG_LEVEL,
                          message,
                          src_location.file,
                          src_location.line );
    }

    void log_message_warn( string_view_t message ) final
    {
        dispatch_message( log4cplus::WARN_LOG_LEVEL, message, nullptr );
    }

    void log_message_warn( src_location_t src_location,
                           string_view_t message ) final
    {
        dispatch_message( log4cplus::WARN_LOG_LEVEL,
                          message,
                          src_location.file,
                          src_location.line );
    }

    void log_message_error( string_view_t message ) final
    {
        dispatch_message( log4cplus::ERROR_LOG_LEVEL, message, nullptr );
    }

    void log_message_error( src_location_t src_location,
                            string_view_t message ) final
    {
        dispatch_message( log4cplus::ERROR_LOG_LEVEL,
                          message,
                          src_location.file,
                          src_location.line );
    }

    void log_message_critical( string_view_t message ) final
    {
        dispatch_message( log4cplus::FATAL_LOG_LEVEL, message, nullptr );
    }

    void log_message_critical( src_location_t src_location,
                               string_view_t message ) final
    {
        dispatch_message( log4cplus::FATAL_LOG_LEVEL,
                          message,
                          src_location.file,
                          src_location.line );
    }

    void log_flush() final {}

    log4cplus::Logger m_logger;
};

using log4cplus_logger_t = basic_log4cplus_logger_t< 1024 >;

} /* namespace logr */
