// Test that logr is a self sufficient header file.

#include <logr/logr.hpp>
#include <logr/null_backend.hpp>
#include <logr/ostream_backend.hpp>

#include <gtest/gtest.h>

namespace /* anonymous */
{

TEST( LogrRootLoggerType, CheckHasTheSameBase )  // NOLINT
{
    // This test etalon
    using etalon_base_t = logr::basic_logger_type_t< 100, char >;

    using sample_logger_1_t = logr::null_logger_t< 100 >;

    {
        auto check_result =
            std::is_same_v< etalon_base_t, sample_logger_1_t::root_logger_type_t >;
        ASSERT_TRUE( check_result );
    }
    {
        auto check_result = std::is_base_of_v< etalon_base_t, sample_logger_1_t >;
        ASSERT_TRUE( check_result );
    }

    using sample_logger_2_t = logr::ostream_logger_t< 100 >;

    {
        auto check_result =
            std::is_same_v< etalon_base_t, sample_logger_2_t::root_logger_type_t >;
        ASSERT_TRUE( check_result );
    }
    {
        auto check_result = std::is_base_of_v< etalon_base_t, sample_logger_2_t >;
        ASSERT_TRUE( check_result );
    }

    using sample_logger_3_t = logr::ostream_logger_t< 200 >;
    {
        auto check_result =
            std::is_same_v< etalon_base_t, sample_logger_3_t::root_logger_type_t >;
        ASSERT_FALSE( check_result );
    }
    {
        auto check_result = std::is_base_of_v< etalon_base_t, sample_logger_3_t >;
        ASSERT_FALSE( check_result );
    }
}

TEST( LogrRootLoggerType, CheckHasTheSameBaseWideChar )  // NOLINT
{
    // This test etalon
    using etalon_base_t = logr::basic_logger_type_t< 100, wchar_t >;

    using sample_wlogger_1_t = logr::null_wlogger_t< 100 >;

    {
        auto check_result =
            std::is_same_v< etalon_base_t,
                            sample_wlogger_1_t::root_logger_type_t >;
        ASSERT_TRUE( check_result );
    }
    {
        auto check_result = std::is_base_of_v< etalon_base_t, sample_wlogger_1_t >;
        ASSERT_TRUE( check_result );
    }

    using sample_wlogger_2_t = logr::ostream_wlogger_t< 100 >;

    {
        auto check_result =
            std::is_same_v< etalon_base_t,
                            sample_wlogger_2_t::root_logger_type_t >;
        ASSERT_TRUE( check_result );
    }
    {
        auto check_result = std::is_base_of_v< etalon_base_t, sample_wlogger_2_t >;
        ASSERT_TRUE( check_result );
    }

    using sample_wlogger_3_t = logr::ostream_wlogger_t< 200 >;
    {
        auto check_result =
            std::is_same_v< etalon_base_t,
                            sample_wlogger_3_t::root_logger_type_t >;
        ASSERT_FALSE( check_result );
    }
    {
        auto check_result = std::is_base_of_v< etalon_base_t, sample_wlogger_3_t >;
        ASSERT_FALSE( check_result );
    }
}

} /* anonymous namespace */
