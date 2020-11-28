// Logger frontend library for C++.
//
// Copyright (c) 2020 - present,  Nicolai Grodzitski
// See LICENSE file in the root of the project.

#pragma once

#include <ostream>

#include <logr/logr.hpp>

namespace logr
{

/**
 * @brief A simple logger to std::ostream.
 *
 * Implements logr::basic_logger_t for writing log-messages to ostream.
 *
 * Serves as a demo logger, please, don't use it in production.
 */
template < std::size_t Inline_Size,
           typename CharT     = char,
           typename Allocator = std::allocator< CharT > >
class basic_ostream_logger_t final
    : public basic_logger_t<
          basic_logger_traits_t< Inline_Size, CharT, Allocator > >
{
public:
    using base_type_t =
        basic_logger_t< basic_logger_traits_t< Inline_Size, CharT, Allocator > >;
    using ostream_t     = std::basic_ostream< CharT >;
    using string_view_t = typename base_type_t::string_view_t;

    explicit basic_ostream_logger_t(
        ostream_t & output,
        log_message_level level = log_message_level::error,
        const Allocator & alloc = Allocator() )
        : base_type_t{ level, alloc }
        , m_output{ output }
    {
    }

private:
    void log_message_trace( string_view_t message ) override
    {
        m_output << "TRACE: " << message << '\n';
    }

    void log_message_trace( src_location_t src_location,
                            string_view_t message ) override
    {
        m_output << "TRACE: " << message << " @ " << src_location.file << "("
                 << src_location.line << ")\n";
    }

    void log_message_debug( string_view_t message ) override
    {
        m_output << "TRACE: " << message << '\n';
    }

    void log_message_debug( src_location_t src_location,
                            string_view_t message ) override
    {
        m_output << "TRACE: " << message << " @ " << src_location.file << "("
                 << src_location.line << ")\n";
    }

    void log_message_info( string_view_t message ) override
    {
        m_output << "INFO : " << message << '\n';
    }

    void log_message_info( src_location_t src_location,
                           string_view_t message ) override
    {
        m_output << "INFO : " << message << " @ " << src_location.file << "("
                 << src_location.line << ")\n";
    }

    void log_message_warn( string_view_t message ) override
    {
        m_output << "WARN : " << message << '\n';
    }

    void log_message_warn( src_location_t src_location,
                           string_view_t message ) override
    {
        m_output << "WARN : " << message << " @ " << src_location.file << "("
                 << src_location.line << ")\n";
    }

    void log_message_error( string_view_t message ) override
    {
        m_output << "ERR  : " << message << '\n';
    }

    void log_message_error( src_location_t src_location,
                            string_view_t message ) override
    {
        m_output << "ERR  : " << message << " @ " << src_location.file << "("
                 << src_location.line << ")\n";
    }

    void log_message_critical( string_view_t message ) override
    {
        m_output << "CRIT : " << message << '\n';
    }

    void log_message_critical( src_location_t src_location,
                               string_view_t message ) override
    {
        m_output << "CRIT : " << message << " @ " << src_location.file << "("
                 << src_location.line << ")\n";
    }

    void log_flush() override { m_output.flush(); }

    ostream_t & m_output;
};

template < std::size_t Inline_Size = 1024,
           typename Allocator      = std::allocator< char > >
using ostream_logger_t = basic_ostream_logger_t< Inline_Size, char, Allocator >;

template < std::size_t Inline_Size = 1024,
           typename Allocator      = std::allocator< wchar_t > >
using ostream_wlogger_t =
    basic_ostream_logger_t< Inline_Size, wchar_t, Allocator >;

} /* namespace logr */
