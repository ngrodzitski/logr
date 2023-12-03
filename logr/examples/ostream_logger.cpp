// Logger frontend library for C++.
//
// Copyright (c) 2020 - present,  Nicolai Grodzitski
// See LICENSE file in the root of the project.

/**
 * @file
 *
 * A simple example that shows the usage of logr with ostream backend.
 */

#include <iostream>

#include <logr/logr.hpp>
#include <logr/ostream_backend.hpp>

#include "use_logger.hpp"

auto make_logger()
{
    return logr::ostream_logger_t<>{ std::cout, logr::log_message_level::trace };
}

auto make_wlogger()
{
    return logr::ostream_wlogger_t<>{ std::wcout, logr::log_message_level::trace };
}

int main( int argc, char * argv[] )
{
    try
    {
        if( 1 == argc || ( 2 == argc && 'a' == argv[ 1 ][ 0 ] ) )
        {
            make_and_use_logger( make_logger );
        }
        else if( 2 == argc && 'u' == argv[ 1 ][ 0 ] )
        {
            make_and_use_wlogger( make_wlogger );
        }
        else
        {
            throw std::runtime_error{ "Bad arguments" };
        }
    }
    catch( const std::exception & ex )
    {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
