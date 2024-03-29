# Logr

* [Logr](#logr)
  * [What is it? (TL;DR)](#what-is-it-tldr)
  * [Build](#build)
     * [Prerequisetes](#prerequisetes)
     * [Build on linux with Conan](#build-on-linux-with-conan)
     * [Build on windows with Conan](#build-on-windows-with-conan)
  * [Using logr](#using-logr)
  * [Naming](#naming)
  * [License](#license)

## What is it? (TL;DR)

**Project status**: beta-draft.

This is a experimentsl project showing my approach to build a logger interface
used in application development. It is a revised version of what
I've been using for the last couple of years in various projects.

This is not a self-contained logging library.
Consider it as a "frontend" or a "wrapper" for whatever logging routine
of your choice (which can be integrated as a backend).
It can be used "as is" in your application or it can be adopted for your case
or maybe you can just pick some ideas and came up with a better solution.

Consider the following reasoning behing logr design
[here](./design/on-logr-design.md).

## Build

By itself logr is a header-only library.
So talking about build we mean build tests, examples and benchmarks.

### Prerequisetes

* C++17 compiler.
* CMake.
* Package manager ([conan](https://conan.io/), or [vcpkg](https://github.com/microsoft/vcpkg)).
* Clone repository with submodules (don't forget `--recurse-submodules` option).

Logr relies on "backend" logging library.
And it comes with ready to go backends for a couple of libraries
which are considered to be provided by package manager.

### Build on linux with Conan

One of the script in `build-scripts` folder can be used.

Build with gcc 9.3 and run tests.

```
./build-scripts/gcc93_conan.sh --run-ctest
```

Or build it manually:
```bash
mkdir build && cd build

# Install conan dependencies.
conan install \
    -s compiler=gcc \
    -s compiler.version=9.3 \
    -s compiler.libcxx=libstdc++11 \
    -s build_type=Release \
    --build missing \
    ..

# Assume you deafult compiler is GCC 9.3.
# Otherwise set it explicitly:
#     export CC=gcc-9
#     export CPP=g++-9
#     export AR=gcc-ar-9
#     export CXX=g++-9

cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . -j $(nproc)
ctest -T test
```

### Build on linux with Conan dockers

Using one of the dockers from [docker hub conanio](https://hub.docker.com/u/conanio):

```bash
docker run -ti --user=$(id -u):$(id -g) \
           -v $(pwd):/sources \
           -w /sources \
           -e CONAN_USER_HOME=/sources/_conan \
           conanio/gcc10 \
           ./build-scripts/gcc_default.sh \
               --run-ctest
```

### Build on windows with Conan

Build with msvc16 (aka vs2019, aka vc142):

```
rd /s /q _msvc16_conan
mkdir _msvc16_conan
cd _msvc16_conan

REM Precompiled log4cplus in conan-center seems to be broken
REM so we force its build `-b log4cplus`.
conan install -s compiler="Visual Studio" ^
              -s compiler.version=16 ^
              -s build_type=Release ^
              -s compiler.runtime=MD ^
              --build missing ^
              -b log4cplus ^
              ..

cmake -G "Visual Studio 16 2019" ^
      -DCMAKE_BUILD_TYPE=Release ^
      ..

cmake --build . -j 4 --config Release
ctest -T test
```
## Build with Vcpkg

To build tests, examples and benchmarks the following packages
should be installed: glog, spdlog, log4cplus, Boost, gtest, benchmark.

Having all dependencies in place one can build the project
using the following commands:

```bash
mkdir _vcpkg_build
cd _vcpkg_build

cmake -DCMAKE_BUILD_TYPE=Release -DLOGR_PKG_PROVIDER=vcpkg  -DCMAKE_TOOLCHAIN_FILE=PATH_TO/vcpkg/scripts/buildsystems/vcpkg.cmake ..

cmake --build . -j 4 --config Release
ctest -T test
```

## Using logr

A sample code using spdlogger with logr:

```C++
#include <spdlog/sinks/stdout_sinks.h>

#include <logr/logr.hpp>
#include <logr/spdlog_backend.hpp>

int main()
{
    int x=42, y=0xFFFF;

    logr::spdlog_logger_t<> logger{
        "console",
        std::make_shared< spdlog::sinks::stdout_sink_st >(),
        logr::log_message_level::trace
    };

    logger.info( "Hello World! [raw message]" );

    logger.info( []() {
        return "Hello World! [message-builder by return]";
    } );

    logger.info( []( auto & out ) {
        fmt::format_to( "Hello World! [message-builder by write-to]" );
    } );

    logger.info( [&]( auto & out ) {
        format_to( out, "X is {}! and Y is {}", x, y );
    } );

    logger.info( [&]( auto out ) {
        format_to( out, "Y is {}! and X is {}", y, x );
    } );

    logger.info( [&x]( auto & out ) {
        out.format_to( "X is {}! and Y is skipped", x );
    } );

    logger.info( [&y]( auto out ) {
        ::fmt::format_to( out.buf(), "Y is {}! and X is skipped", y );
    } );
}
```

See all [examples](./examples).

## Naming

The original idea behind the library came from
[restinio](https://github.com/Stiffstream/restinio) in which a custom logger
can be integrated to RESTinio to track its internal events.
That is pretty much the major idea around Logr,
so `logr = LOGger from Restinio` to show gratitude to great library.
And on the other hand it just 4 letters long which is good for namespaces.

## License

Logr is distributed under the BSD 3-Clause [License](./LICENSE).

Libraries used as logger backends are distributed under their license:

* [spdlog](https://github.com/gabime/spdlog).
* [glog](https://github.com/google/glog).
* [log4cplus](https://github.com/log4cplus/log4cplus).
* [Boost::log](https://www.boost.org/doc/libs/1_77_0/libs/log/doc/html/index.html) trivial.
