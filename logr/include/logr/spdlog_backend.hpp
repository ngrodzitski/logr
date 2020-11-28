// Logger frontend library for C++.
//
// Copyright (c) 2020 - present,  Nicolai Grodzitski
// See LICENSE file in the root of the project.

#pragma once

#include <spdlog/sinks/sink.h>
#include <spdlog/spdlog.h>

#include <logr/logr.hpp>

namespace logr
{

//
// spdlog_logger_traits_t
//

template < std::size_t Inline_Size, typename Allocator = std::allocator< char > >
struct spdlog_logger_traits_t
    : public basic_logger_traits_t< Inline_Size, char, Allocator >
{
};

//
// basic_spdlog_logger_t
//

/**
 * @brief A logger using spdlog logger (https://github.com/gabime/spdlog)
 * as a backend.
 *
 * Implements logr::basic_logger_t for writing log-messages to spdlogs.
 * It can be treated as a substitute for original `spdlog::logger`
 * It connot be used as a drop-in replacement as it doesn't mimiques its
 * interface.
 * @c basic_spdlog_logger_t can be constructed the same way
 * as `spdlog::logger` does.
 */
template < std::size_t Inline_Size, typename Allocator = std::allocator< char > >
class basic_spdlog_logger_t
    : public basic_logger_t< spdlog_logger_traits_t< Inline_Size, Allocator > >
{
public:
    using base_type_t =
        basic_logger_t< spdlog_logger_traits_t< Inline_Size, Allocator > >;
    using message_container_t = typename base_type_t::message_container_t;
    using string_view_t       = typename base_type_t::string_view_t;

    explicit basic_spdlog_logger_t(
        std::string logger_name,
        log_message_level level = log_message_level::error,
        const Allocator & alloc = Allocator() )
        : base_type_t{ level, alloc }
        , m_logger_name{ std::move( logger_name ) }
    {
    }

    template < typename Sink_Iterator >
    basic_spdlog_logger_t( std::string logger_name,
                           Sink_Iterator begin,
                           Sink_Iterator end,
                           log_message_level level = log_message_level::error,
                           const Allocator & alloc = Allocator() )
        : base_type_t{ level, alloc }
        , m_logger_name{ std::move( logger_name ) }
        , m_sinks{ begin, end }
    {
    }

    basic_spdlog_logger_t( std::string logger_name,
                           spdlog::sinks_init_list sinks,
                           log_message_level level = log_message_level::error,
                           const Allocator & alloc = Allocator() )
        : basic_spdlog_logger_t{ std::move( logger_name ),
                                 begin( sinks ),
                                 end( sinks ),
                                 level,
                                 alloc }
    {
    }

    basic_spdlog_logger_t( std::string logger_name,
                           spdlog::sink_ptr single_sink,
                           log_message_level level = log_message_level::error,
                           const Allocator & alloc = Allocator() )
        : basic_spdlog_logger_t{ std::move( logger_name ),
                                 { std::move( single_sink ) },
                                 level,
                                 alloc }
    {
    }

    const auto & sinks() const noexcept { return m_sinks; }

    auto & sinks() noexcept { return m_sinks; }

private:
    /**
     * @brief Distribute message to all sinks.
     * @param log_msg  Spdlog biassed log message.
     */
    void dispatch_message( const spdlog::details::log_msg & log_msg )
    {
        for( auto & s : m_sinks )
        {
            s->log( log_msg );
        }
    }

    template < log_message_level Level >
    void dispatch_message( string_view_t message )
    {
        dispatch_message( spdlog::details::log_msg{
            m_logger_name,
            static_cast< spdlog::level::level_enum >( Level ),
            message } );
    }

    template < log_message_level Level >
    void dispatch_message( src_location_t src_location, string_view_t message )
    {
        dispatch_message( spdlog::details::log_msg{
            spdlog::source_loc{ src_location.file, src_location.line, nullptr },
            m_logger_name,
            static_cast< spdlog::level::level_enum >( Level ),
            message } );
    }

    void log_message_trace( string_view_t message ) final
    {
        dispatch_message< log_message_level::trace >( message );
    }

    void log_message_trace( src_location_t src_location,
                            string_view_t message ) final
    {
        dispatch_message< log_message_level::trace >( src_location, message );
    }

    void log_message_debug( string_view_t message ) final
    {
        dispatch_message< log_message_level::debug >( message );
    }

    void log_message_debug( src_location_t src_location,
                            string_view_t message ) final
    {
        dispatch_message< log_message_level::debug >( src_location, message );
    }

    void log_message_info( string_view_t message ) final
    {
        dispatch_message< log_message_level::info >( message );
    }

    void log_message_info( src_location_t src_location,
                           string_view_t message ) final
    {
        dispatch_message< log_message_level::info >( src_location, message );
    }

    void log_message_warn( string_view_t message ) final
    {
        dispatch_message< log_message_level::warn >( message );
    }

    void log_message_warn( src_location_t src_location,
                           string_view_t message ) final
    {
        dispatch_message< log_message_level::warn >( src_location, message );
    }

    void log_message_error( string_view_t message ) final
    {
        dispatch_message< log_message_level::error >( message );
    }

    void log_message_error( src_location_t src_location,
                            string_view_t message ) final
    {
        dispatch_message< log_message_level::error >( src_location, message );
    }

    void log_message_critical( string_view_t message ) final
    {
        dispatch_message< log_message_level::critical >( message );
    }

    void log_message_critical( src_location_t src_location,
                               string_view_t message ) final
    {
        dispatch_message< log_message_level::critical >( src_location, message );
    }

    void log_flush() final
    {
        for( auto & s : m_sinks )
        {
            s->flush();
        }
    }

    const std::string m_logger_name;
    std::vector< spdlog::sink_ptr > m_sinks;
};

template < std::size_t Inline_Size = 1024,
           typename Allocator      = std::allocator< char > >
using spdlog_logger_t = basic_spdlog_logger_t< Inline_Size, Allocator >;

} /* namespace logr */
