# Changelog

## v0.2.0 (beta-draft)

* Switch to `spdlog/1.8.2` version in conanfile.
* Make a core logr class independent from traits class.
This allows having the same base class (as a root) for logers based on traits.
* Make adjustments in cmake-scripts for packaging in [conan-center-index](https://github.com/conan-io/conan-center-index).
* Rethinking write-to msg builder wrapped output.
Introduce a wrapper for output in write-to message builders.
This allows a message builder to receive `out` by reference and also by value.
Thus it is a breaking change as previously using `fmt::format_to(out, ...)` doesn't work anymore.
The following ways to use write-to message builders are possible:

```C++
    int x = make_int();
    std::string s = make_string();

    // All the logging is equivalent.

    // Out by value.
    logger.info( [&]( auto out ) {
        format_to( out, "Write-to message builder 1,  int:'{}', str: '{}'", x, s );
    } );
    logger.info( [&]( auto out ) {
        out.format_to( "Write-to message builder 2,  int:'{}', str: '{}'", x, s );
    } );
    logger.info( [&]( auto out ) {
        ::fmt::format_to( out.buf() "Write-to message builder 3,  int:'{}', str: '{}'", x, s );
    } );

    // Out by reference.
    logger.info( [&]( auto & out ) {
        format_to( out, "Write-to message builder 1,  int:'{}', str: '{}'", x, s );
    } );
    logger.info( [&]( auto & out ) {
        out.format_to( "Write-to message builder 2,  int:'{}', str: '{}'", x, s );
    } );
    logger.info( [&]( auto & out ) {
        ::fmt::format_to( out.buf() "Write-to message builder 3,  int:'{}', str: '{}'", x, s );
    } );
```

## v0.1.0 (alpha-draft)

First release (alpha). includes the following

* Three backends: spdlog, glog and log4cplus.
* A set of tests, examples and a number of simple basic benchmarks.
