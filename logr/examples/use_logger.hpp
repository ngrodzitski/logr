// Logger frontend library for C++.
//
// Copyright (c) 2020 - present,  Nicolai Grodzitski
// See LICENSE file in the root of the project.

#pragma once

#include <iostream>

#include <logr/logr.hpp>

/**
 * @brief A sample routine for using logger.
 *
 * @tparam Logger  A type of a logger.
 */
template < typename Logger >
void use_logger( Logger & logger )
{
    logger.trace( "Hello World! [raw message]" );
    logger.trace( LOGR_SRC_LOCATION, "Hello World! [raw message]" );

    logger.trace( []() { return "Hello World! [cb]"; } );
    logger.trace( LOGR_SRC_LOCATION, []() { return "Hello World! [cb]"; } );

    logger.trace( []( auto out ) {
        format_to( out, "Hello {}! [{}]", "World", "cb with explicit out" );
    } );

    logger.trace( LOGR_SRC_LOCATION, []( auto out ) {
        format_to( out, "Hello {}! [{}]", "World", "cb with explicit out" );
    } );

    logger.trace( LOGR_SRC_LOCATION, []( auto out ) {
        format_to( out,
                   FMT_STRING( "Hello {}! [{}]" ),
                   "World",
                   "cb with explicit out and FMT_STRING" );
    } );

    logger.trace( LOGR_SRC_LOCATION, []( auto out ) {
        format_to( out,
                   fmt::runtime( "Hello {}! [{}]" ),
                   "World",
                   "cb with explicit out and runtime-string" );
    } );

    logger.debug( "Hello World! [raw message]" );
    logger.debug( LOGR_SRC_LOCATION, "Hello World! [raw message]" );

    logger.debug( []() { return "Hello World! [cb]"; } );
    logger.debug( LOGR_SRC_LOCATION, []() { return "Hello World! [cb]"; } );

    logger.debug( []( auto out ) {
        format_to( out, "Hello {}! [{}]", "World", "cb with explicit out" );
    } );

    logger.debug( LOGR_SRC_LOCATION, []( auto out ) {
        format_to( out, "Hello {}! [{}]", "World", "cb with explicit out" );
    } );
    logger.debug( LOGR_SRC_LOCATION, []( auto out ) {
        format_to( out,
                   FMT_STRING( "Hello {}! [{}]" ),
                   "World",
                   "cb with explicit out and FMT_STRING" );
    } );

    logger.debug( LOGR_SRC_LOCATION, []( auto out ) {
        format_to( out,
                   fmt::runtime( "Hello {}! [{}]" ),
                   "World",
                   "cb with explicit out and runtime-string" );
    } );

    logger.info( "Hello World! [raw message]" );
    logger.info( LOGR_SRC_LOCATION, "Hello World! [raw message]" );

    logger.info( []() { return "Hello World! [cb]"; } );
    logger.info( LOGR_SRC_LOCATION, []() { return "Hello World! [cb]"; } );

    logger.info( []( auto out ) {
        format_to( out, "Hello {}! [{}]", "World", "cb with explicit out" );
    } );

    logger.info( LOGR_SRC_LOCATION, []( auto out ) {
        format_to( out, "Hello {}! [{}]", "World", "cb with explicit out" );
    } );
    logger.info( LOGR_SRC_LOCATION, []( auto out ) {
        format_to( out,
                   FMT_STRING( "Hello {}! [{}]" ),
                   "World",
                   "cb with explicit out and FMT_STRING" );
    } );

    logger.info( LOGR_SRC_LOCATION, []( auto out ) {
        format_to( out,
                   fmt::runtime( "Hello {}! [{}]" ),
                   "World",
                   "cb with explicit out and runtime-string" );
    } );

    logger.warn( "Hello World! [raw message]" );
    logger.warn( LOGR_SRC_LOCATION, "Hello World! [raw message]" );

    logger.warn( []() { return "Hello World! [cb]"; } );
    logger.warn( LOGR_SRC_LOCATION, []() { return "Hello World! [cb]"; } );

    logger.warn( []( auto out ) {
        format_to( out, "Hello {}! [{}]", "World", "cb with explicit out" );
    } );

    logger.warn( LOGR_SRC_LOCATION, []( auto out ) {
        format_to( out, "Hello {}! [{}]", "World", "cb with explicit out" );
    } );
    logger.warn( LOGR_SRC_LOCATION, []( auto out ) {
        format_to( out,
                   FMT_STRING( "Hello {}! [{}]" ),
                   "World",
                   "cb with explicit out and FMT_STRING" );
    } );

    logger.warn( LOGR_SRC_LOCATION, []( auto out ) {
        format_to( out,
                   fmt::runtime( "Hello {}! [{}]" ),
                   "World",
                   "cb with explicit out and runtime-string" );
    } );

    logger.error( "Hello World! [raw message]" );
    logger.error( LOGR_SRC_LOCATION, "Hello World! [raw message]" );

    logger.error( []() { return "Hello World! [cb]"; } );
    logger.error( LOGR_SRC_LOCATION, []() { return "Hello World! [cb]"; } );

    logger.error( []( auto out ) {
        format_to( out, "Hello {}! [{}]", "World", "cb with explicit out" );
    } );

    logger.error( LOGR_SRC_LOCATION, []( auto out ) {
        format_to( out, "Hello {}! [{}]", "World", "cb with explicit out" );
    } );
    logger.error( LOGR_SRC_LOCATION, []( auto out ) {
        format_to( out,
                   FMT_STRING( "Hello {}! [{}]" ),
                   "World",
                   "cb with explicit out and FMT_STRING" );
    } );

    logger.error( LOGR_SRC_LOCATION, []( auto out ) {
        format_to( out,
                   fmt::runtime( "Hello {}! [{}]" ),
                   "World",
                   "cb with explicit out and runtime-string" );
    } );

    logger.critical( "Hello World! [raw message]" );
    logger.critical( LOGR_SRC_LOCATION, "Hello World! [raw message]" );

    logger.critical( []() { return "Hello World! [cb]"; } );
    logger.critical( LOGR_SRC_LOCATION, []() { return "Hello World! [cb]"; } );

    logger.critical( []( auto out ) {
        format_to( out, "Hello {}! [{}]", "World", "cb with explicit out" );
    } );

    logger.critical( LOGR_SRC_LOCATION, []( auto out ) {
        format_to( out, "Hello {}! [{}]", "World", "cb with explicit out" );
    } );
    logger.critical( LOGR_SRC_LOCATION, []( auto out ) {
        format_to( out,
                   FMT_STRING( "Hello {}! [{}]" ),
                   "World",
                   "cb with explicit out and FMT_STRING" );
    } );

    logger.critical( LOGR_SRC_LOCATION, []( auto out ) {
        format_to( out,
                   fmt::runtime( "Hello {}! [{}]" ),
                   "World",
                   "cb with explicit out and runtime-string" );
    } );

    logger.flush();
}

/**
 * @brief A sample routine for using unicode logger.
 *
 * @tparam Logger  A type of a logger.
 */
template < typename Logger >
void use_wlogger( Logger & logger )
{
    logger.trace( L"Привет Мир! [raw message]" );
    logger.flush();
    logger.trace( LOGR_SRC_LOCATION, L"Привет Мир! [raw message]" );
    logger.flush();

    logger.trace( []() { return L"Привет Мир! [cb]"; } );
    logger.flush();
    logger.trace( LOGR_SRC_LOCATION, []() { return L"Привет Мир! [cb]"; } );
    logger.flush();

    logger.trace( []( auto out ) {
        format_to( out, L"Привет {}! [{}]", L"Мир", L"cb with explicit out" );
    } );
    logger.flush();

    logger.trace( LOGR_SRC_LOCATION, []( auto out ) {
        format_to( out, L"Привет {}! [{}]", L"Мир", L"cb with explicit out" );
    } );
    logger.flush();
    logger.trace( LOGR_SRC_LOCATION, []( auto out ) {
        format_to( out,
                   FMT_STRING( L"Привет {}! [{}]" ),
                   L"Мир",
                   L"cb with explicit out FMT_STRING" );
    } );
    logger.flush();
    logger.trace( LOGR_SRC_LOCATION, []( auto out ) {
        format_to( out,
                   fmt::runtime( L"Привет {}! [{}]" ),
                   L"Мир",
                   L"cb with explicit out runtime-string" );
    } );
    logger.flush();

    logger.debug( L"Привет Мир! [raw message]" );
    logger.debug( LOGR_SRC_LOCATION, L"Привет Мир! [raw message]" );

    logger.debug( []() { return L"Привет Мир! [cb]"; } );
    logger.debug( LOGR_SRC_LOCATION, []() { return L"Привет Мир! [cb]"; } );

    logger.debug( []( auto out ) {
        format_to( out, L"Привет {}! [{}]", L"Мир", L"cb with explicit out" );
    } );

    logger.debug( LOGR_SRC_LOCATION, []( auto out ) {
        format_to( out, L"Привет {}! [{}]", L"Мир", L"cb with explicit out" );
    } );

    logger.info( L"Привет Мир! [raw message]" );
    logger.info( LOGR_SRC_LOCATION, L"Привет Мир! [raw message]" );

    logger.info( []() { return L"Привет Мир! [cb]"; } );
    logger.info( LOGR_SRC_LOCATION, []() { return L"Привет Мир! [cb]"; } );

    logger.info( []( auto out ) {
        format_to( out, L"Привет {}! [{}]", L"Мир", L"cb with explicit out" );
    } );

    logger.info( LOGR_SRC_LOCATION, []( auto out ) {
        format_to( out, L"Привет {}! [{}]", L"Мир", L"cb with explicit out" );
    } );

    logger.warn( L"Привет Мир! [raw message]" );
    logger.warn( LOGR_SRC_LOCATION, L"Привет Мир! [raw message]" );

    logger.warn( []() { return L"Привет Мир! [cb]"; } );
    logger.warn( LOGR_SRC_LOCATION, []() { return L"Привет Мир! [cb]"; } );

    logger.warn( []( auto out ) {
        format_to( out, L"Привет {}! [{}]", L"Мир", L"cb with explicit out" );
    } );

    logger.warn( LOGR_SRC_LOCATION, []( auto out ) {
        format_to( out, L"Привет {}! [{}]", L"Мир", L"cb with explicit out" );
    } );

    logger.error( L"Привет Мир! [raw message]" );
    logger.error( LOGR_SRC_LOCATION, L"Привет Мир! [raw message]" );

    logger.error( []() { return L"Привет Мир! [cb]"; } );
    logger.error( LOGR_SRC_LOCATION, []() { return L"Привет Мир! [cb]"; } );

    logger.error( []( auto out ) {
        format_to( out, L"Привет {}! [{}]", L"Мир", L"cb with explicit out" );
    } );

    logger.error( LOGR_SRC_LOCATION, []( auto out ) {
        format_to( out, L"Привет {}! [{}]", L"Мир", L"cb with explicit out" );
    } );

    logger.critical( L"Привет Мир! [raw message]" );
    logger.critical( LOGR_SRC_LOCATION, L"Привет Мир! [raw message]" );

    logger.critical( []() { return L"Привет Мир! [cb]"; } );
    logger.critical( LOGR_SRC_LOCATION, []() { return L"Привет Мир! [cb]"; } );

    logger.critical( []( auto out ) {
        format_to( out, L"Привет {}! [{}]", L"Мир", L"cb with explicit out" );
    } );

    logger.critical( LOGR_SRC_LOCATION, []( auto out ) {
        format_to( out, L"Привет {}! [{}]", L"Мир", L"cb with explicit out" );
    } );

    logger.flush();
}

template < typename Logger_Factory >
void make_and_use_logger( Logger_Factory factory )
{
    auto logger = factory();

    use_logger( logger );

    logger.info( "This must be the last message" );

    logger.log_level( logr::log_message_level::nolog );

    // Should print nothing:
    use_logger( logger );
}

template < typename Logger_Factory >
void make_and_use_wlogger( Logger_Factory factory )
{
    std::locale::global( std::locale( "" ) );

    auto logger = factory();

    use_wlogger( logger );

    logger.info( L"This must be the last message" );

    logger.log_level( logr::log_message_level::nolog );

    // Should print nothing:
    use_wlogger( logger );
}
