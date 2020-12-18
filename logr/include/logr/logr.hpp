// Logger frontend library for C++.
//
// Copyright (c) 2020 - present,  Nicolai Grodzitski
// See LICENSE file in the root of the project.

#pragma once

#include <string_view>
#include <type_traits>
#include <atomic>

#include <fmt/format.h>

#include <logr/config.hpp>

namespace logr
{

namespace details
{

//
// basic_small_message_container_t
//

/**
 * @brief A small wrapper for receiving buffer for log-message.
 *
 * Uses `fmt::basic_memory_buffer<*>` for receiving log data
 * which is a SmallVector flavored CharT buffer that doesn't allocate
 * if the data fits in a given static buffer.
 */
template < std::size_t Inline_Size,
           typename CharT     = char,
           typename Allocator = std::allocator< CharT > >
class basic_small_message_container_t final
{
public:
    //! Alias for string view for a given char type.
    using string_view_t = std::basic_string_view< CharT >;

    /**
     * @brief Create a a view to a message.
     *
     * @return String view on message data.
     */
    string_view_t make_view() const noexcept
    {
        return string_view_t{ m_message.data(), m_message.size() };
    }

    using message_buffer_t =
        fmt::basic_memory_buffer< CharT, Inline_Size, Allocator >;

    /**
     * @brief Get an output buffer.
     * @return A reference to a lower level buffer.
     */
    message_buffer_t & msg_buffer() noexcept { return m_message; };

private:
    message_buffer_t m_message;
};

} /* namespace details */

//
// write_to_ouput_wrapper_t
//

/**
 * @brief A wrapper for output parameter passed to `write-to` callbacks.
 *
 * The purpose of the wrapper is to allow callbacks receiving
 * `out` parameter either by value or by reference.
 *
 * @code{.cpp}
 * logger.info( [&]( auto out ){
 *   // By value...
 * } );
 * logger.info( [&]( auto & out ){
 *   // By reference...
 * } );
 * @endcode
 */
template < typename Buffer >
class write_to_ouput_wrapper_t
{
public:
    /**
     * @brief Init with a given buffer.
     */
    write_to_ouput_wrapper_t( Buffer & buf ) noexcept
        : m_buffer{ buf }
    {
        using self_type_t = write_to_ouput_wrapper_t< Buffer >;
        static_assert( std::is_trivially_copyable_v< self_type_t > );
    }

    /**
     * @brief Get the fmt-acceptable buffer.
     *
     * @return A reference to fmt basic memory buffer.
     */
    Buffer & buf() noexcept { return m_buffer; }

    /**
     * @brief a shortcut function to perform message formating to buffer.
     */
    template < typename... Args >
    auto format_to( Args &&... args )
    {
        return ::fmt::format_to( buf(), std::forward< Args >( args )... );
    }

private:
    Buffer & m_buffer;
};

/**
 * @brief A helper free-function to start formatting message to buffer.
 *
 * ADL based shortcut function to allow write-to message builder
 * to write in the the following style:
 * @code{.cpp}
 * logger.info( [&]( auto out ){
 *   format_to( out, "fmt str", x, y, z );
 * } );
 * @endcode
 */
template < typename Buffer, typename... Args >
auto format_to( write_to_ouput_wrapper_t< Buffer > out, Args &&... args )
{
    return ::fmt::format_to( out.buf(), std::forward< Args >( args )... );
}

//
// log_level
//

/**
 * @brief Log messages levels.
 *
 * Primarly logr uses spdlog, so the list of log levels
 * is a copy of spdlog log levels.
 */
enum class log_message_level : int
{
    trace,
    debug,
    info,
    warn,
    error,
    critical,
    nolog
};

//
// src_location_t
//

/**
 * @brief A aggregate struct for src location.
 *
 * @see LOGR_SRC_LOCATION
 */

struct src_location_t
{
    constexpr src_location_t() = default;

    constexpr src_location_t( const char * f, int l )
        : file{ f }
        , line{ l }
    {
    }

    const char * file{ nullptr };
    int line{ 0 };
};

//
// no_src_location_t
//

/**
 * @brief A tag class for collapsing src location usage.
 *
 * This class helps to remove src-location attribute of a message
 * without modifying existing logging code.
 * If `LOGR_CLIENT_COLLAPSE_SRC_LOCATION` macro is defined then
 * `LOGR_SRC_LOCATION` is set to @c no_src_location_t.
 *
 * @see LOGR_CLIENT_COLLAPSE_SRC_LOCATION
 * @see LOGR_SRC_LOCATION
 */
struct no_src_location_t
{
};

//
// st_log_level_driver_t
//

/**
 * @brief A single thread log level driver.
 *
 * Handles log level access without any synchronization.
 *
 * @note Class doesn't have a virtual destructor
 *       as the intended usage scenario is to
 *       use it as a private base class explicitly
 *       moving access functions to public part of a base class.
 */
class st_log_level_driver_t final
{
public:
    explicit st_log_level_driver_t( log_message_level level ) noexcept
        : m_log_level{ level }
    {
    }

    // No virtual destructor as the data is trivial.

    /**
     * @brief Get currently enabled log level.
     *
     * @return Log level set for logger.
     */
    log_message_level log_level() const noexcept { return m_log_level; }

    /**
     * @brief Set log level for logger.
     *
     * @param new_log_level  New log level for logger.
     */
    void log_level( log_message_level new_log_level ) noexcept
    {
        m_log_level = new_log_level;
    }

private:
    log_message_level m_log_level;
};

//
// mt_log_level_driver_t
//

/**
 * @brief A multi thread log level driver.
 *
 * Handles log level access with atomic.
 *
 * @note Class doesn't have a virtual destructor
 *       as the intended usage scenario is to
 *       use it as a private base class explicitly
 *       moving access functions to public part of a base class.
 *
 * @tparam Load_MO  Memory order for loading value.
 * @tparam Store_MO  Memory order for storing new value.
 */
template < std::memory_order Load_MO  = std::memory_order_relaxed,
           std::memory_order Store_MO = std::memory_order_relaxed >
class mt_log_level_driver_t final
{
public:
    explicit mt_log_level_driver_t( log_message_level level ) noexcept
        : m_log_level{ level }
    {
    }

    // No virtual destructor as the data is trivial.

    /**
     * @brief Get currently enabled log level.
     *
     * @return Log level set for logger.
     */
    log_message_level log_level() const noexcept
    {
        return m_log_level.load( Load_MO );
    }

    /**
     * @brief Set log level for logger.
     *
     * @param new_log_level  New log level for logger.
     */
    void log_level( log_message_level new_log_level ) noexcept
    {
        m_log_level.store( new_log_level, Store_MO );
    }

private:
    std::atomic< log_message_level > m_log_level;
};

//
// basic_logger_traits_t
//

/**
 * @brief Logger traits.
 *
 * @tparam Inline_Size       The size of stack buffer used for messages.
 * @tparam CharT             Cheracter type.
 * @tparam Allocator         Allocator used for buffer
 *                           (in case stack buffer is not enough).
 * @tparam Log_Level_Driver  Log level control block.
 */
template < std::size_t Inline_Size,
           typename CharT            = char,
           typename Allocator        = std::allocator< CharT >,
           typename Log_Level_Driver = st_log_level_driver_t >
struct basic_logger_traits_t
{
    //! Inline message size.
    static constexpr std::size_t inline_size = Inline_Size;

    //! Charecter type.
    using char_t = CharT;

    //! Allocator.
    using allocator_t = Allocator;

    //! Log level handling routine.
    using log_level_driver_t = Log_Level_Driver;
};

//
// basic_logger_t
//

/**
 * @brief A logger front-end base class.
 *
 * Introduces `.xlevel( message )` interface. It
 * contains a propper mechanics for rendering log-messages (if needed)
 * and routing them to a set of overaridable functions with string view
 * interface (a more complex logic can be achieved using
 * overriding with message_container_t parameters).
 * So once a specific log level implementation (ovverriden function)
 * is called it means that a given level is active.
 *
 * @note Looking at implementation it may seem odd to have
 *       a transition from `xlevel(...)` messaging function
 *       stating level in its name to `message()` function having
 *       level specified as template parameter than back to
 *       to-be-overriden function `log_message_xlevel()` function
 *       stating its level in its name (via @c log_message_level_x).
 *       But that is a reuse mechanics not to manualy check for a
 *       particular log level and propagate copies of
 *       handle message builders biolerplates.
 *
 * @tparam Inline_Size       The size of stack buffer used for messages.
 * @tparam CharT             Cheracter type.
 * @tparam Allocator         Allocator used for buffer
 *                           (in case stack buffer is not enough).
 * @tparam Log_Level_Driver  Log level control block.
 */
template < std::size_t Inline_Size,
           typename CharT            = char,
           typename Allocator        = std::allocator< CharT >,
           typename Log_Level_Driver = st_log_level_driver_t >
class basic_logger_type_t
{
public:
    static constexpr auto inline_size = Inline_Size;
    using char_t                      = CharT;
    using allocator_t                 = Allocator;

    using message_container_t = details::
        basic_small_message_container_t< inline_size, char_t, allocator_t >;

    using string_view_t    = typename message_container_t::string_view_t;
    using message_buffer_t = typename message_container_t::message_buffer_t;

    template < typename Message_Builder >
    static inline constexpr bool is_by_return_msg_producer_v =
        std::is_invocable_r< string_view_t, Message_Builder >::value;

    template < typename Message_Builder >
    static inline constexpr bool is_by_writeto_msg_producer_v =
        std::is_invocable_v<
            Message_Builder,
            write_to_ouput_wrapper_t<
                message_buffer_t > > || std::is_invocable_v< Message_Builder, write_to_ouput_wrapper_t< message_buffer_t > & >;

    template < typename Message_Builder >
    static inline constexpr bool valid_message_producer_v =
        is_by_return_msg_producer_v<
            Message_Builder > || is_by_writeto_msg_producer_v< Message_Builder >;

    using log_level_driver_t = Log_Level_Driver;

    /**
     * @brief A very base class of logger.
     */
    using root_logger_type_t = basic_logger_type_t< inline_size,
                                                    char_t,
                                                    allocator_t,
                                                    log_level_driver_t >;

public:
    /**
     * @brief Constract logger with a given log level.
     *
     * @param  level  Logging messages level.
     * @param  alloc  Allocator used for message buffer.
     */
    explicit basic_logger_type_t(
        log_message_level level,
        const allocator_t & alloc = allocator_t() ) noexcept
        : m_log_level{ level }
        , m_alloc{ alloc }
    {
    }

    /**
     * @brief Constract logger with a default log level.
     *
     * @param  alloc  Allocator used for message buffer.
     */
    explicit basic_logger_type_t(
        const allocator_t & alloc = allocator_t() ) noexcept
        : basic_logger_type_t{ log_message_level::info, alloc }
    {
    }

    /**
     * @brief Get currently enabled log level.
     *
     * @return Log level set for logger.
     */
    auto log_level() const noexcept { return m_log_level.log_level(); }

    /**
     * @brief Set log level for logger.
     *
     * @param new_log_level  New log level for logger.
     */
    void log_level( log_message_level new_log_level ) noexcept
    {
        m_log_level.log_level( new_log_level );
    }

    virtual ~basic_logger_type_t() = default;

#if !defined( LOGR_CLIENT_ZERO_LOGGER )

    //
    // Basic message logging mechanics for all levels.
    //

    template < log_message_level Level >
    void message( string_view_t raw_message )
    {
        if( needs_log( Level ) )
        {
            log_message_level_x< Level >( raw_message );
        }
    }

    template < log_message_level Level >
    void message( src_location_t src_location, string_view_t raw_message )
    {
        if( needs_log( Level ) )
        {
            log_message_level_x< Level >( src_location, raw_message );
        }
    }

    template < log_message_level Level,
               typename Message_Builder,
               typename = std::enable_if_t<
                   valid_message_producer_v< Message_Builder > > >
    void message( Message_Builder msg_builder )
    {
        if( needs_log( Level ) )
        {
            if constexpr( is_by_return_msg_producer_v<
                              Message_Builder > )  // NOLINT
            {
                log_message_level_x< Level >( msg_builder() );
            }
            else
            {
                static_assert( is_by_writeto_msg_producer_v< Message_Builder >,
                               "Bad producer, that assert must never be failed, "
                               "unless a new message producer types are added" );

                message_container_t msg;

                {
                    write_to_ouput_wrapper_t out{ msg.msg_buffer() };
                    msg_builder( out );
                }

                // Dispatch message as a string view.
                log_message_level_x< Level >( msg.make_view() );
            }
        }
    }

    template < log_message_level Level,
               typename Message_Builder,
               typename = std::enable_if_t<
                   valid_message_producer_v< Message_Builder > > >
    void message( src_location_t src_location, Message_Builder msg_builder )
    {
        if( needs_log( Level ) )
        {
            if constexpr( is_by_return_msg_producer_v< Message_Builder > )
            {
                log_message_level_x< Level >( src_location, msg_builder() );
            }
            else
            {
                static_assert( is_by_writeto_msg_producer_v< Message_Builder >,
                               "Bad producer, that assert must never be failed, "
                               "unless a new message producer types are added" );

                message_container_t msg;

                {
                    write_to_ouput_wrapper_t out{ msg.msg_buffer() };
                    msg_builder( out );
                }
                // msg_builder( msg.msg_buffer() );

                // Dispatch message as a string view.
                log_message_level_x< Level >( src_location, msg.make_view() );
            }
        }
    }

    //
    // Trace messaging.
    //

    void trace( string_view_t raw_message )
    {
        message< log_message_level::trace >( raw_message );
    }

    template < size_t N >
    void trace( const char_t raw_message[ N ] )
    {
        message< log_message_level::trace >( string_view_t( raw_message, N - 1 ) );
    }

    void trace( no_src_location_t, string_view_t raw_message )
    {
        message< log_message_level::trace >( raw_message );
    }

    void trace( src_location_t src_location, string_view_t raw_message )
    {
        message< log_message_level::trace >( src_location, raw_message );
    }

    template < typename Message_Builder,
               typename = std::enable_if_t<
                   valid_message_producer_v< Message_Builder > > >
    void trace( Message_Builder msg_builder )
    {
        message< log_message_level::trace >( std::move( msg_builder ) );
    }

    template < typename Message_Builder,
               typename = std::enable_if_t<
                   valid_message_producer_v< Message_Builder > > >
    void trace( no_src_location_t, Message_Builder msg_builder )
    {
        trace( std::move( msg_builder ) );
    }

    template < typename Message_Builder,
               typename = std::enable_if_t<
                   valid_message_producer_v< Message_Builder > > >
    void trace( src_location_t src_location, Message_Builder msg_builder )
    {
        message< log_message_level::trace >( src_location,
                                             std::move( msg_builder ) );
    }

    //
    // Debug messaging.
    //

    void debug( string_view_t raw_message )
    {
        message< log_message_level::debug >( raw_message );
    }

    void debug( no_src_location_t, string_view_t raw_message )
    {
        message< log_message_level::debug >( raw_message );
    }

    void debug( src_location_t src_location, string_view_t raw_message )
    {
        message< log_message_level::debug >( src_location, raw_message );
    }

    template < typename Message_Builder,
               typename = std::enable_if_t<
                   valid_message_producer_v< Message_Builder > > >
    void debug( Message_Builder msg_builder )
    {
        message< log_message_level::debug >( std::move( msg_builder ) );
    }

    template < typename Message_Builder,
               typename = std::enable_if_t<
                   valid_message_producer_v< Message_Builder > > >
    void debug( no_src_location_t, Message_Builder msg_builder )
    {
        debug( std::move( msg_builder ) );
    }

    template < typename Message_Builder,
               typename = std::enable_if_t<
                   valid_message_producer_v< Message_Builder > > >
    void debug( src_location_t src_location, Message_Builder msg_builder )
    {
        message< log_message_level::debug >( src_location,
                                             std::move( msg_builder ) );
    }

    //
    // Info messaging.
    //

    void info( string_view_t raw_message )
    {
        message< log_message_level::info >( raw_message );
    }

    void info( no_src_location_t, string_view_t raw_message )
    {
        message< log_message_level::info >( raw_message );
    }

    void info( src_location_t src_location, string_view_t raw_message )
    {
        message< log_message_level::info >( src_location, raw_message );
    }

    template < typename Message_Builder,
               typename = std::enable_if_t<
                   valid_message_producer_v< Message_Builder > > >
    void info( Message_Builder msg_builder )
    {
        message< log_message_level::info >( std::move( msg_builder ) );
    }

    template < typename Message_Builder,
               typename = std::enable_if_t<
                   valid_message_producer_v< Message_Builder > > >
    void info( no_src_location_t, Message_Builder msg_builder )
    {
        info( std::move( msg_builder ) );
    }

    template < typename Message_Builder,
               typename = std::enable_if_t<
                   valid_message_producer_v< Message_Builder > > >
    void info( src_location_t src_location, Message_Builder msg_builder )
    {
        message< log_message_level::info >( src_location,
                                            std::move( msg_builder ) );
    }

    //
    // Warn messaging.
    //

    void warn( string_view_t raw_message )
    {
        message< log_message_level::warn >( raw_message );
    }

    void warn( no_src_location_t, string_view_t raw_message )
    {
        message< log_message_level::warn >( raw_message );
    }

    void warn( src_location_t src_location, string_view_t raw_message )
    {
        message< log_message_level::warn >( src_location, raw_message );
    }

    template < typename Message_Builder,
               typename = std::enable_if_t<
                   valid_message_producer_v< Message_Builder > > >
    void warn( Message_Builder msg_builder )
    {
        message< log_message_level::warn >( std::move( msg_builder ) );
    }

    template < typename Message_Builder,
               typename = std::enable_if_t<
                   valid_message_producer_v< Message_Builder > > >
    void warn( no_src_location_t, Message_Builder msg_builder )
    {
        warn( std::move( msg_builder ) );
    }

    template < typename Message_Builder,
               typename = std::enable_if_t<
                   valid_message_producer_v< Message_Builder > > >
    void warn( src_location_t src_location, Message_Builder msg_builder )
    {
        message< log_message_level::warn >( src_location,
                                            std::move( msg_builder ) );
    }

    //
    // Error messaging.
    //

    void error( string_view_t raw_message )
    {
        message< log_message_level::error >( raw_message );
    }

    void error( no_src_location_t, string_view_t raw_message )
    {
        message< log_message_level::error >( raw_message );
    }

    void error( src_location_t src_location, string_view_t raw_message )
    {
        message< log_message_level::error >( src_location, raw_message );
    }

    template < typename Message_Builder,
               typename = std::enable_if_t<
                   valid_message_producer_v< Message_Builder > > >
    void error( Message_Builder msg_builder )
    {
        message< log_message_level::error >( std::move( msg_builder ) );
    }

    template < typename Message_Builder,
               typename = std::enable_if_t<
                   valid_message_producer_v< Message_Builder > > >
    void error( no_src_location_t, Message_Builder msg_builder )
    {
        error( std::move( msg_builder ) );
    }

    template < typename Message_Builder,
               typename = std::enable_if_t<
                   valid_message_producer_v< Message_Builder > > >
    void error( src_location_t src_location, Message_Builder msg_builder )
    {
        message< log_message_level::error >( src_location,
                                             std::move( msg_builder ) );
    }

    //
    // Critical messaging.
    //

    void critical( string_view_t raw_message )
    {
        message< log_message_level::critical >( raw_message );
    }

    void critical( no_src_location_t, string_view_t raw_message )
    {
        message< log_message_level::critical >( raw_message );
    }

    void critical( src_location_t src_location, string_view_t raw_message )
    {
        message< log_message_level::critical >( src_location, raw_message );
    }

    template < typename Message_Builder,
               typename = std::enable_if_t<
                   valid_message_producer_v< Message_Builder > > >
    void critical( Message_Builder msg_builder )
    {
        message< log_message_level::critical >( std::move( msg_builder ) );
    }

    template < typename Message_Builder,
               typename = std::enable_if_t<
                   valid_message_producer_v< Message_Builder > > >
    void critical( no_src_location_t, Message_Builder msg_builder )
    {
        critical( std::move( msg_builder ) );
    }

    template < typename Message_Builder,
               typename = std::enable_if_t<
                   valid_message_producer_v< Message_Builder > > >
    void critical( src_location_t src_location, Message_Builder msg_builder )
    {
        message< log_message_level::critical >( src_location,
                                                std::move( msg_builder ) );
    }

    void flush() { log_flush(); }

#else  // defined(LOGR_CLIENT_ZERO_LOGGER) case

    // Eliminate any logging.
    template < typename... Args >
    constexpr void trace( Args &&... ) const noexcept
    {
    }

    template < typename... Args >
    constexpr void debug( Args &&... ) const noexcept
    {
    }

    template < typename... Args >
    constexpr void info( Args &&... ) const noexcept
    {
    }

    template < typename... Args >
    constexpr void warn( Args &&... ) const noexcept
    {
    }

    template < typename... Args >
    constexpr void error( Args &&... ) const noexcept
    {
    }

    template < typename... Args >
    constexpr void critical( Args &&... ) const noexcept
    {
    }

    constexpr void flush() const noexcept {}
#endif

private:
    /**
     * @brief A routing function to an implementation routine for a given
     *        log level.
     *
     * @param args  A wildcard params to be directed to an implementation routine.
     */
    template < log_message_level Level, typename... Args >
    void log_message_level_x( Args &&... args )
    {
        if constexpr( Level == log_message_level::trace )
        {
            log_message_trace( std::forward< Args >( args )... );
        }
        else if constexpr( Level == log_message_level::debug )
        {
            log_message_debug( std::forward< Args >( args )... );
        }
        else if constexpr( Level == log_message_level::info )
        {
            log_message_info( std::forward< Args >( args )... );
        }
        else if constexpr( Level == log_message_level::warn )
        {
            log_message_warn( std::forward< Args >( args )... );
        }
        else if constexpr( Level == log_message_level::error )
        {
            log_message_error( std::forward< Args >( args )... );
        }
        else if constexpr( Level == log_message_level::critical )
        {
            log_message_critical( std::forward< Args >( args )... );
        }
        else
        {
            // Unfortunately static_assert( false, "Message" )
            // would fire always, so here is a workaround
            // to make it fire only in cases of template instantiation

            struct dummy_type_t
            {
            };

            static_assert(
                !std::is_same_v< dummy_type_t, dummy_type_t >,  // Always false.
                "Unknown message level" );
        }
    }

protected:
    // ===============================================================
    // Mandatory override functions (logger backend interface).
    //

    virtual void log_message_trace( string_view_t message ) = 0;

    virtual void log_message_debug( string_view_t message ) = 0;

    virtual void log_message_info( string_view_t message ) = 0;

    virtual void log_message_warn( string_view_t message ) = 0;

    virtual void log_message_error( string_view_t message ) = 0;

    virtual void log_message_critical( string_view_t message ) = 0;

    virtual void log_message_trace( src_location_t src_location,
                                    string_view_t message ) = 0;

    virtual void log_message_debug( src_location_t src_location,
                                    string_view_t message ) = 0;

    virtual void log_message_info( src_location_t src_location,
                                   string_view_t message ) = 0;

    virtual void log_message_warn( src_location_t src_location,
                                   string_view_t message ) = 0;

    virtual void log_message_error( src_location_t src_location,
                                    string_view_t message ) = 0;

    virtual void log_message_critical( src_location_t src_location,
                                       string_view_t message ) = 0;

    virtual void log_flush() = 0;
    // ===============================================================

    bool needs_log( log_message_level level ) const noexcept
    {
        return static_cast< int >( log_level() ) <= static_cast< int >( level );
    }

private:
    log_level_driver_t m_log_level;
    /**
     * @brief Allocator for this logger instance.
     */
    const allocator_t m_alloc;
};

/**
 * @brief A traits alias for basic logger.
 *
 * Having a root logger type a template dependent on traits class
 * produces a distinct type for each traits class,
 * That prevents having the the same base logger for different
 * implementations.
 */
template < typename Logger_Traits >
using basic_logger_t =
    basic_logger_type_t< Logger_Traits::inline_size,
                         typename Logger_Traits::char_t,
                         typename Logger_Traits::allocator_t,
                         typename Logger_Traits::log_level_driver_t >;

//
// devirt_fixup_t
//

/**
 * @brief A fixup class to help compiler to do a better devirtualization.
 *
 * See the following snipped to learn the idea behind it:
 * https://godbolt.org/z/KvnhfW
 *
 * Current status of this class is an "optional" experemental feature
 * to use in benchmarks. Benchmarks show no noticable advantage
 * though in theory an extra devirtualization might help.
 */
template < typename Logger >
class devirt_fixup_t final : public Logger
{
public:
    using base_type_t = Logger;

    template < typename... Args >
    explicit devirt_fixup_t( Args &&... args ) noexcept
        : base_type_t{ std::forward< Args >( args )... }
    {
    }

#if !defined( LOGR_CLIENT_ZERO_LOGGER )
    // Fixup needs to be applied only in case logger is not
    // collapsed to nothing.

    // Explicitly associating frontend functions with
    // top-level class (`devirt_fixup_t<Logger>` itself)
    // helps compiler to inline `trace(),debug(),info(),...` functions
    // as if they were operating over this class level, thus knowing
    // exactly what are the implementations of a virtual-functions
    // of a given logger backend part so they can be devirtualized.

    template < typename... Args >
    constexpr void trace( Args &&... args )
    {
        base_type_t::trace( std::forward< Args >( args )... );
    }

    template < typename... Args >
    constexpr void debug( Args &&... args )
    {
        base_type_t::debug( std::forward< Args >( args )... );
    }

    template < typename... Args >
    constexpr void info( Args &&... args )
    {
        base_type_t::info( std::forward< Args >( args )... );
    }

    template < typename... Args >
    constexpr void warn( Args &&... args )
    {
        base_type_t::warn( std::forward< Args >( args )... );
    }

    template < typename... Args >
    constexpr void error( Args &&... args )
    {
        base_type_t::error( std::forward< Args >( args )... );
    }

    template < typename... Args >
    constexpr void critical( Args &&... args )
    {
        base_type_t::critical( std::forward< Args >( args )... );
    }
#endif
};

// A set of macro helpers for providing src file/line location.

// Gives a C-string of current file skipping n characters.
// TODO: can be safer with checking biundaries.
#define LOGR_STRIP_FILE_NAME_N( n ) ( &__FILE__[ n ] )

/*
 *  # Project src location definition snippet.
 *
 *  The following series of macros definition is a snipped
 *  (configured for Logr project itself and is used in samples)
 *  to be injected into a project using Logr logger
 *  as its logging slot.
 *
 * # TL;DR
 *
 * In order to have src location without useless path artifacts
 * of a given build on a given machine one can define
 * a macros `XPRJ_SRC_LOCATION` that will provide
 * pathes like "examples/ostream_logger.cpp" instead of
 * "/home/ngrodzitski/github/ngrodzitski/path/to/prj/ostream_logger.cpp"
 *
 * # Details
 *
 * As a result a macro with the name like `XPRJ_SRC_LOCATION`
 * (`LOGR_SRC_LOCATION` here) will be available and
 * should be used as an src location piece to be passed to logging functions.
 *
 * It might seem odd to use a a per-project `XPRJ_SRC_LOCATION`
 * macro, but it appears unavoidable. Imagine you have a project
 * Foo that relies on a project Bar and both of them use Logr
 * as an internal logging routine. If both projects are in the
 * same root then it is ok to use the same macro as they
 * viewd by compiler in the same root path. But depending on
 * the directory layout of your project
 * even in this case you might not be satisfied with results
 * (e.g. you might end up having "foo/include/foo/xxx.hpp" and
 * "foo/src/foo/xxx.cpp" while a less verbose vaersion would be
 * "foo/xxx.hpp" "foo/xxx.cpp"). And things get even more complicates
 * using package managers like Conan or vcpkg. If Bar project is build by Conan
 * on your machine it would have a long path (incuding things like hashes).
 * And even more complex scenario is prebuild binaries which draws path
 * not even available on your machine.
 *
 * Having this it is reasonable to get more control of src locations.
 *
 * # How to inject sourcefile hint into cmake project.
 *
 * Let's XPRJ be an uppercased name of the project.
 *
 * Here is one possible solution:
 *
 * 1. Define two options in your root cmake-file:
 *    ```
 *    option(XPRJ_PRJ_ROOT_LENGTH_HINT_ENABLED
 *           "Enable a hint to cut prj-root from sources path in logging."
 *           ON)
 *
 *    option(XPRJ_PRJ_COLLAPSE_SRC_LOCATION_ENABLED
             "Enable a hint to collapse all src location (make them unused)."
             OFF)
 *    ```
 * 2. Depending on the values of previous options in the cmake-file
 *    of the project add public definition to target:
 *    ```
 *    # For header-only libraries `INTERFACE` should be used
 *    # instead of `PUBLIC` when adding compile definitions.
 *    if (XPRJ_PRJ_COLLAPSE_SRC_LOCATION_ENABLED)
 *      target_compile_definitions(<xprj_target>
 *                                 PUBLIC "XPRJ_PRJ_COLLAPSE_SRC_LOCATION")
 *    else ()
 *        if (XPRJ_PRJ_ROOT_LENGTH_HINT_ENABLED)
 *            string(LENGTH "${CMAKE_CURRENT_LIST_DIR}/" project_root_path_len)
 *            # Or any other logic to skip unnecessary part.
 *            target_compile_definitions(
 *                <xprj_target>
 *                PUBLIC
 *                "XPRJ_PRJ_ROOT_LENGTH_HINT=${project_root_path_len}")
 *        endif ()
 *    endif ()
 *    ```
 * 3. Add the a header file introducing logr to project:
 *    ```
 *    #inlcude <logr/logr.hpp>
 *
 *    #if defined( XPRJ_PRJ_COLLAPSE_SRC_LOCATION )
 *    #    define XPRJ_SRC_LOCATION ::logr::no_src_location_t {}
 *    #else
 *    #    if !defined( XPRJ_PRJ_ROOT_LENGTH_HINT )
 *    #        define XPRJ_PRJ_ROOT_LENGTH_HINT 0
 *    #    endif
 *
 *    #    define XPRJ_SRC_LOCATION                                             \
 *            ::logr::src_location_t{                                             \
 *                LOGR_STRIP_FILE_NAME_N( XPRJ_PRJ_ROOT_LENGTH_HINT ), __LINE__ \
 *            }
 *    #endif
 *    ```
 *
 * 4. Use it the following way:
 *    ```
 *    logger.info( XPRJ_SRC_LOCATION, "log message");
 *    logger.info( XPRJ_SRC_LOCATION, [&]( auto & out ){
 *        fmt::format_to( out, "test message {}", some_str );
 *     } );
 *    ```
 */

/**
 * @brief A helper macro defining file location.
 *
 * In case project root path length hint is provided file is considered
 * __FILE__ without a prefix of a given length.
 * For cmake-based projects which operates the full path to files
 * it is handy to define a hint that points the size of prefix
 * containing useless part of project root directory on a given system.
 * TODO: add example and cmke snippet.
 */
#if defined( LOGR_PRJ_COLLAPSE_SRC_LOCATION )
// LOGR_PRJ_COLLAPSE_SRC_LOCATION is defined that means
// all src location pieces must be collapsed to nothing.
#    define LOGR_SRC_LOCATION \
        ::logr::no_src_location_t {}
#else
// LOGR_PRJ_ROOT_LENGTH_HINT macros is supposed to be set by
// users to customize src location of the project:
//    * LOGR_PRJ_ROOT_LENGTH_HINT: a size of prefix to remove from
//      __FILE__ strings.
#    if !defined( LOGR_PRJ_ROOT_LENGTH_HINT )
#        define LOGR_PRJ_ROOT_LENGTH_HINT 0
#    endif

#    define LOGR_SRC_LOCATION                                             \
        ::logr::src_location_t                                            \
        {                                                                 \
            LOGR_STRIP_FILE_NAME_N( LOGR_PRJ_ROOT_LENGTH_HINT ), __LINE__ \
        }
#endif

} /* namespace logr */
