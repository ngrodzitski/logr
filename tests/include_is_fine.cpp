// Test that logr is a self sufficient header file.

#include <logr/logr.hpp>

#include <gtest/gtest.h>

namespace /* anonymous */
{

TEST( LogrInclude, LogrHeaderMustBeSelfSufficient )  // NOLINT
{
    ASSERT_TRUE( true );
}

} /* anonymous namespace */
