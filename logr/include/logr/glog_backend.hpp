// Logger frontend library for C++.
//
// Copyright (c) 2020 - present,  Nicolai Grodzitski
// See LICENSE file in the root of the project.

#pragma once

#include <glog/logging.h>

#include <logr/logr.hpp>

namespace logr
{

//
// glog_logger_traits_t
//

template < std::size_t Inline_Size, typename Allocator = std::allocator< char > >
struct glog_logger_traits_t
    : public basic_logger_traits_t< Inline_Size, char, Allocator >
{
};

//
// basic_glog_logger_t
//

/**
 * @brief A logger using glog (https://github.com/google/glog)
 * as a backend.
 *
 * Implements logr::basic_logger_t for writing log-messages to glog.
 */
template < std::size_t Inline_Size, typename Allocator = std::allocator< char > >
class basic_glog_logger_t
    : public basic_logger_t< glog_logger_traits_t< Inline_Size, Allocator > >
{
public:
    using base_type_t =
        basic_logger_t< glog_logger_traits_t< Inline_Size, Allocator > >;
    using message_container_t = typename base_type_t::message_container_t;
    using string_view_t       = typename base_type_t::string_view_t;

    explicit basic_glog_logger_t(
        log_message_level level = log_message_level::error,
        const Allocator & alloc = Allocator() )
        : base_type_t{ level, alloc }
    {
    }

    explicit basic_glog_logger_t(
        google::LogSink * log_sink,
        log_message_level level = log_message_level::error,
        const Allocator & alloc = Allocator() )
        : base_type_t{ level, alloc }
        , m_log_sink{ log_sink }
    {
    }

    explicit basic_glog_logger_t(
        google::LogSink * log_sink,
        bool also_send_to_log,
        log_message_level level = log_message_level::error,
        const Allocator & alloc = Allocator() )
        : base_type_t{ level, alloc }
        , m_log_sink{ log_sink }
        , m_also_send_to_log{ also_send_to_log }
    {
    }

private:
    void log_message_trace( string_view_t message ) final
    {
        if( VLOG_IS_ON( 1 ) )
        {
            google::LogMessage(
                "", 0, google::GLOG_INFO, m_log_sink, m_also_send_to_log )
                    .stream()
                << message;
        }
    }

    void log_message_trace( src_location_t src_location,
                            string_view_t message ) final
    {
        if( VLOG_IS_ON( 1 ) )
        {
            google::LogMessage( src_location.file,
                                src_location.line,
                                google::GLOG_INFO,
                                m_log_sink,
                                m_also_send_to_log )
                    .stream()
                << message;
        }
    }

    void log_message_debug( string_view_t message ) final
    {
        if( VLOG_IS_ON( 0 ) )
        {
            google::LogMessage(
                "", 0, google::GLOG_INFO, m_log_sink, m_also_send_to_log )
                    .stream()
                << message;
        }
    }

    void log_message_debug( src_location_t src_location,
                            string_view_t message ) final
    {
        if( VLOG_IS_ON( 0 ) )
        {
            google::LogMessage( src_location.file,
                                src_location.line,
                                google::GLOG_INFO,
                                m_log_sink,
                                m_also_send_to_log )
                    .stream()
                << message;
        }
    }

    void log_message_info( string_view_t message ) final
    {
        // LOG( INFO ) << message;
        google::LogMessage(
            "", 0, google::GLOG_INFO, m_log_sink, m_also_send_to_log )
                .stream()
            << message;
    }

    void log_message_info( src_location_t src_location,
                           string_view_t message ) final
    {
        google::LogMessage( src_location.file,
                            src_location.line,
                            google::GLOG_INFO,
                            m_log_sink,
                            m_also_send_to_log )
                .stream()
            << message;
    }

    void log_message_warn( string_view_t message ) final
    {
        google::LogMessage(
            "", 0, google::GLOG_WARNING, m_log_sink, m_also_send_to_log )
                .stream()
            << message;
    }

    void log_message_warn( src_location_t src_location,
                           string_view_t message ) final
    {
        google::LogMessage( src_location.file,
                            src_location.line,
                            google::GLOG_WARNING,
                            m_log_sink,
                            m_also_send_to_log )
                .stream()
            << message;
    }

    void log_message_error( string_view_t message ) final
    {
        google::LogMessage(
            "", 0, google::GLOG_ERROR, m_log_sink, m_also_send_to_log )
                .stream()
            << message;
    }

    void log_message_error( src_location_t src_location,
                            string_view_t message ) final
    {
        google::LogMessage( src_location.file,
                            src_location.line,
                            google::GLOG_ERROR,
                            m_log_sink,
                            m_also_send_to_log )
                .stream()
            << message;
    }

    void log_message_critical( string_view_t message ) final
    {
        google::LogMessage(
            "", 0, google::GLOG_FATAL, m_log_sink, m_also_send_to_log )
                .stream()
            << message;
    }

    void log_message_critical( src_location_t src_location,
                               string_view_t message ) final
    {
        google::LogMessage( src_location.file,
                            src_location.line,
                            google::GLOG_FATAL,
                            m_log_sink,
                            m_also_send_to_log )
                .stream()
            << message;
    }

    void log_flush() final {}

    google::LogSink * const m_log_sink{ nullptr };
    const bool m_also_send_to_log{ true };
};

using glog_logger_t = basic_glog_logger_t< 1024 >;

} /* namespace logr */
