#include <iostream>
#include <logr/ostream_backend.hpp>

int main()
{
    logr::ostream_logger_t<> logger{ std::cout,
                                     logr::log_message_level::trace };

    logger.info( "Welcome logr provided with conan!" );
}
