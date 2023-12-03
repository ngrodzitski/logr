// Logger frontend library for C++.
//
// Copyright (c) 2021 - present,  Nicolai Grodzitski
// See LICENSE file in the root of the project.

/**
 * @file
 *
 * A simple example that shows the usage of logr with trivial boost log backend.
 */

#include <iostream>

#include <logr/logr.hpp>
#include <logr/boostlog_backend.hpp>

#include "use_logger.hpp"

auto make_logger()
{
    return logr::boostlog_logger_t<>{};
}

int main( int /*argc*/, char ** /*argv[]*/ )
{
    try
    {
        make_and_use_logger( make_logger );
    }
    catch( const std::exception & ex )
    {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
