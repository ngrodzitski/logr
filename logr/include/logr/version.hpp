// Logger frontend library for C++.
//
// Copyright (c) 2023 - present,  Nicolai Grodzitski
// See LICENSE file in the root of the project.

// clang-format off
#pragma once

#define LOGR_VERSION_CODE( major, minor, patch ) \
    ( ( ( major ) << 16UL ) + ( ( minor ) << 8UL ) + ( ( patch ) << 0UL ))

#define LOGR_VERSION_MAJOR 0ull
#define LOGR_VERSION_MINOR 7ull
#define LOGR_VERSION_PATCH 0ull

#define LOGR_VERSION \
    LOGR_VERSION_CODE( LOGR_VERSION_MAJOR, LOGR_VERSION_MINOR, LOGR_VERSION_PATCH )

