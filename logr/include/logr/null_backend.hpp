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
 * @brief A null logger.
 *
 * Represents a stub for cases no log is needed.
 *
 */
template < std::size_t Inline_Size,
           typename CharT     = char,
           typename Allocator = std::allocator< CharT > >
class basic_null_logger_t
    : public basic_logger_t<
          basic_logger_traits_t< Inline_Size, CharT, Allocator > >
{
public:
    using base_type_t =
        basic_logger_t< basic_logger_traits_t< Inline_Size, CharT, Allocator > >;
    using ostream_t     = std::basic_ostream< CharT >;
    using string_view_t = typename base_type_t::string_view_t;

    explicit basic_null_logger_t()
        : base_type_t{ log_message_level::nolog, Allocator() }
    {
    }

private:
    constexpr void log_message_trace( string_view_t ) final {}

    constexpr void log_message_trace( src_location_t, string_view_t ) final {}

    constexpr void log_message_debug( string_view_t ) final {}

    constexpr void log_message_debug( src_location_t, string_view_t ) final {}

    constexpr void log_message_info( string_view_t ) final {}

    constexpr void log_message_info( src_location_t, string_view_t ) final {}

    constexpr void log_message_warn( string_view_t ) final {}

    constexpr void log_message_warn( src_location_t, string_view_t ) final {}

    constexpr void log_message_error( string_view_t ) final {}

    constexpr void log_message_error( src_location_t, string_view_t ) final {}

    constexpr void log_message_critical( string_view_t ) final {}

    constexpr void log_message_critical( src_location_t, string_view_t ) final {}

    constexpr void log_flush() final {}
};

template < std::size_t Inline_Size = 1,
           typename Allocator      = std::allocator< char > >
using null_logger_t = basic_null_logger_t< Inline_Size, char, Allocator >;

template < std::size_t Inline_Size = 1,
           typename Allocator      = std::allocator< wchar_t > >
using null_wlogger_t = basic_null_logger_t< Inline_Size, wchar_t, Allocator >;

} /* namespace logr */
