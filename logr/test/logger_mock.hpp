// Basic logger mock for unit tests.

#pragma once

#include <gmock/gmock.h>

#include <logr/logr.hpp>

namespace logr_test
{

//
// basic_logger_mock_t
//

/**
 * @brief A basic mock logger for tests.
 */

template < typename Logger_Traits >
class basic_logger_mock_t : public logr::basic_logger_t< Logger_Traits >
{
public:
    using base_type_t = logr::basic_logger_t< Logger_Traits >;
    using typename base_type_t::allocator_t;
    using message_container_t = typename base_type_t::message_container_t;

    explicit basic_logger_mock_t(
        logr::log_message_level level = logr::log_message_level::error,
        const allocator_t & alloc     = allocator_t() )
        : base_type_t{ level, alloc }
    {
    }

    MOCK_METHOD( void, log_message_trace, ( std::string_view ), ( override ) );

    MOCK_METHOD( void, log_message_debug, ( std::string_view ), ( override ) );

    MOCK_METHOD( void, log_message_info, ( std::string_view ), ( override ) );

    MOCK_METHOD( void, log_message_warn, ( std::string_view ), ( override ) );

    MOCK_METHOD( void, log_message_error, ( std::string_view ), ( override ) );

    MOCK_METHOD( void, log_message_critical, ( std::string_view ), ( override ) );

    MOCK_METHOD( void,
                 log_message_trace,
                 ( logr::src_location_t, std::string_view ),
                 ( override ) );

    MOCK_METHOD( void,
                 log_message_debug,
                 ( logr::src_location_t, std::string_view ),
                 ( override ) );

    MOCK_METHOD( void,
                 log_message_info,
                 ( logr::src_location_t, std::string_view ),
                 ( override ) );

    MOCK_METHOD( void,
                 log_message_warn,
                 ( logr::src_location_t, std::string_view ),
                 ( override ) );

    MOCK_METHOD( void,
                 log_message_error,
                 ( logr::src_location_t, std::string_view ),
                 ( override ) );

    MOCK_METHOD( void,
                 log_message_critical,
                 ( logr::src_location_t, std::string_view ),
                 ( override ) );

    MOCK_METHOD( void, log_flush, (), ( override ) );
};

//
// logger_mock_t
//

template < std::size_t Inline_Size = 1024,
           typename Allocator      = std::allocator< char > >
using logger_mock_t = basic_logger_mock_t<
    logr::basic_logger_traits_t< Inline_Size, char, Allocator > >;

} /* namespace logr_test */
