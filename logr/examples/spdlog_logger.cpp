// Logger frontend library for C++.
//
// Copyright (c) 2020 - present,  Nicolai Grodzitski
// See LICENSE file in the root of the project.

/**
 * @file
 *
 * A simple example that shows the usage of logr with spdlog backend.
 */

#include <iostream>

#include <spdlog/sinks/stdout_sinks.h>

#include <logr/logr.hpp>
#include <logr/spdlog_backend.hpp>

#include "use_logger.hpp"

auto make_logger()
{
    auto sink = std::make_shared< spdlog::sinks::stdout_sink_st >();
    sink->set_pattern( "[%Y-%m-%d %T][%n][%l] %v [%g]" );

    return logr::spdlog_logger_t<>{ "console",
                                    std::move( sink ),
                                    logr::log_message_level::trace };
}

int main()
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
