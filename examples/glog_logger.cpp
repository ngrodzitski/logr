// Logger frontend library for C++.
//
// Copyright (c) 2020 - present,  Nicolai Grodzitski
// See LICENSE file in the root of the project.

/**
 * @file
 *
 * A simple example that shows the usage of logr with glog backend.
 */

#include <iostream>

#include <gflags/gflags.h>
#include <glog/logging.h>
#include <glog/stl_logging.h>

#include <logr/logr.hpp>
#include <logr/glog_backend.hpp>

#include "use_logger.hpp"

auto make_logger()
{
    return logr::glog_logger_t{ logr::log_message_level::trace };
}

int main( int /*argc*/, char * argv[] )
{
    try
    {
        google::InitGoogleLogging( argv[ 0 ] );

        // Optional: parse command line flags
        // gflags::ParseCommandLineFlags(&argc, &argv, true);
        FLAGS_alsologtostderr = true;
        FLAGS_stderrthreshold = 0;
        FLAGS_v               = 2;

        make_and_use_logger( make_logger );
    }
    catch( const std::exception & ex )
    {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
