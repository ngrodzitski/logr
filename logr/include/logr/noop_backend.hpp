/**
 * @brief A noop logger implementations used for testing and analysis
 *        with compiler explorer.
 *
 * Serves as a sample logger, please, don't use it in production.
 */

#pragma once

#include <benchmark/benchmark.h>

#include <logr/logr.hpp>

namespace logr
{

//
// basic_noop_logger_t
//

/**
 * @brief A basic noop logger for benchmark and generated code analysis.
 */
template < std::size_t Inline_Size,
           typename CharT     = char,
           typename Allocator = std::allocator< CharT > >
class basic_noop_logger_t
    : public basic_logger_t<
          basic_logger_traits_t< Inline_Size, CharT, Allocator > >
{
public:
    using base_type_t =
        basic_logger_t< basic_logger_traits_t< Inline_Size, CharT, Allocator > >;
    using string_view_t = typename base_type_t::string_view_t;

    explicit basic_noop_logger_t(
        log_message_level level = log_message_level::error,
        const Allocator & alloc = Allocator() )
        : base_type_t{ level, alloc }
    {
    }

private:
    void log_message_trace( string_view_t msg ) final
    {
        benchmark::DoNotOptimize( msg );
    }

    void log_message_trace( src_location_t src, string_view_t msg ) final
    {
        benchmark::DoNotOptimize( src );
        benchmark::DoNotOptimize( msg );
    }

    void log_message_debug( string_view_t msg ) final
    {
        benchmark::DoNotOptimize( msg );
    }

    void log_message_debug( src_location_t src, string_view_t msg ) final
    {
        benchmark::DoNotOptimize( src );
        benchmark::DoNotOptimize( msg );
    }

    void log_message_info( string_view_t msg ) final
    {
        benchmark::DoNotOptimize( msg );
    }

    void log_message_info( src_location_t src, string_view_t msg ) final
    {
        benchmark::DoNotOptimize( src );
        benchmark::DoNotOptimize( msg );
    }

    void log_message_warn( string_view_t msg ) final
    {
        benchmark::DoNotOptimize( msg );
    }

    void log_message_warn( src_location_t src, string_view_t msg ) final
    {
        benchmark::DoNotOptimize( src );
        benchmark::DoNotOptimize( msg );
    }

    void log_message_error( string_view_t msg ) final
    {
        benchmark::DoNotOptimize( msg );
    }

    void log_message_error( src_location_t src, string_view_t msg ) final
    {
        benchmark::DoNotOptimize( src );
        benchmark::DoNotOptimize( msg );
    }

    void log_message_critical( string_view_t msg ) final
    {
        benchmark::DoNotOptimize( msg );
    }

    void log_message_critical( src_location_t src, string_view_t msg ) final
    {
        benchmark::DoNotOptimize( src );
        benchmark::DoNotOptimize( msg );
    }

    void log_flush() final {}
};

template < std::size_t Inline_Size = 1024,
           typename Allocator      = std::allocator< char > >
using noop_logger_t = basic_noop_logger_t< Inline_Size, char, Allocator >;

}  // namespace logr
