// Logger frontend library for C++.
//
// Copyright (c) 2020 - present,  Nicolai Grodzitski
// See LICENSE file in the root of the project.

/**
 * @file
 *
 * A simple example that shows the usage of logr with log4cplus backend.
 */

#include <iostream>

#include <log4cplus/consoleappender.h>
#include <log4cplus/initializer.h>

#include <logr/log4cplus_backend.hpp>

#include "use_logger.hpp"

auto make_logger()
{
    log4cplus::SharedAppenderPtr append_1( new log4cplus::ConsoleAppender() );

    append_1->setName( LOG4CPLUS_TEXT( "First" ) );
    append_1->setLayout( std::make_unique< log4cplus::PatternLayout >(
        LOG4CPLUS_TEXT( "%-5p [%F,%L] %m%n" ) ) );

    log4cplus::Logger::getRoot().addAppender( append_1 );

    return logr::log4cplus_logger_t{ log4cplus::Logger::getInstance(
                                         LOG4CPLUS_TEXT( "sample" ) ),
                                     logr::log_message_level::trace };
}

int main()
{
    try
    {
        log4cplus::Initializer initializer;

// `UNICODE` is defined by log4cplus.
// It is not a great name for a macro so I don't reccomend to use
// such names in production. But for example it is ok.
#if UNICODE
        make_and_use_wlogger( make_logger );
#else
        make_and_use_logger( make_logger );
#endif
    }
    catch( const std::exception & ex )
    {
        std::cout << "Error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
