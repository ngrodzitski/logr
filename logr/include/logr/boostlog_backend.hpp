// Logger frontend library for C++.
//
// Copyright (c) 2021 - present,  Nicolai Grodzitski
// See LICENSE file in the root of the project.

#pragma once

#include <boost/log/trivial.hpp>

#include <logr/logr.hpp>

namespace logr
{

//
// boostlog_logger_traits_t
//

template < std::size_t Inline_Size >
struct boostlog_logger_traits_t
    : public basic_logger_traits_t< Inline_Size, char, std::allocator< char > >
{
};

/**
 * @brief A logger using Boost.Log logger
 * (https://www.boost.org/doc/libs/1_77_0/libs/log/doc/html/index.html)
 * as a backend.
 *
 * Implements logr::basic_logger_t for writing log-messages using
 * boost trivial logging
 * https://www.boost.org/doc/libs/1_77_0/libs/log/doc/html/log/tutorial.html#log.tutorial.trivial.
 */
template < std::size_t Inline_Size >
class basic_boostlog_logger_t final
    : public basic_logger_t< basic_logger_traits_t< Inline_Size > >
{
public:
    using base_type_t   = basic_logger_t< basic_logger_traits_t< Inline_Size > >;
    using string_view_t = typename base_type_t::string_view_t;

    explicit basic_boostlog_logger_t(
        log_message_level level              = log_message_level::error,
        const std::allocator< char > & alloc = {} )
        : base_type_t{ level, alloc }
    {
    }

private:
    void log_message_trace( string_view_t message ) final
    {
        BOOST_LOG_TRIVIAL( trace ) << message;
    }

    void log_message_trace( src_location_t src_location,
                            string_view_t message ) final
    {
        BOOST_LOG_TRIVIAL( trace ) << message << " [" << src_location.file << ":"
                                   << src_location.line << "]";
    }

    void log_message_debug( string_view_t message ) final
    {
        BOOST_LOG_TRIVIAL( debug ) << message;
    }

    void log_message_debug( src_location_t src_location,
                            string_view_t message ) final
    {
        BOOST_LOG_TRIVIAL( debug ) << message << " [" << src_location.file << ":"
                                   << src_location.line << "]";
    }

    void log_message_info( string_view_t message ) final
    {
        BOOST_LOG_TRIVIAL( info ) << message;
    }

    void log_message_info( src_location_t src_location,
                           string_view_t message ) final
    {
        BOOST_LOG_TRIVIAL( info ) << message << " [" << src_location.file << ":"
                                  << src_location.line << "]";
    }

    void log_message_warn( string_view_t message ) final
    {
        BOOST_LOG_TRIVIAL( warning ) << message;
    }

    void log_message_warn( src_location_t src_location,
                           string_view_t message ) final
    {
        BOOST_LOG_TRIVIAL( warning ) << message << " [" << src_location.file << ":"
                                     << src_location.line << "]";
    }

    void log_message_error( string_view_t message ) final
    {
        BOOST_LOG_TRIVIAL( error ) << message;
    }

    void log_message_error( src_location_t src_location,
                            string_view_t message ) final
    {
        BOOST_LOG_TRIVIAL( error ) << message << " [" << src_location.file << ":"
                                   << src_location.line << "]";
    }

    void log_message_critical( string_view_t message ) final
    {
        BOOST_LOG_TRIVIAL( fatal ) << message;
    }

    void log_message_critical( src_location_t src_location,
                               string_view_t message ) final
    {
        BOOST_LOG_TRIVIAL( fatal ) << message << " [" << src_location.file << ":"
                                   << src_location.line << "]";
    }

    void log_flush() final { boost::log::core::get()->flush(); }
};

template < std::size_t Inline_Size = 1024 >
using boostlog_logger_t = basic_boostlog_logger_t< Inline_Size >;

} /* namespace logr */
