from conans import ConanFile, CMake, tools

class LogrConan(ConanFile):
    name = "logr"
    version = "0.1.0"
    license = "BSD 3-Clause License"
    author = "Nicolai Grodzitski <utromvecherom@gmail.com>"
    url = "https://github.com/ngrodzitski/logr"
    description = "Logger frontend substitution for spdlog, glog, etc for server/desktop applications"
    topics = ("logger", "development", "util", "utils")

    generators = "cmake_find_package"
    settings = "os", "compiler", "build_type", "arch"

    build_policy = "missing"

    # If CI wants test then consider building of examples and benchmarks
    # is a kind of test.
    logr_build_test_and_others = tools.get_env("CONAN_RUN_TESTS", False)

    exports_sources = "logr/*", "CMakeLists.txt", "cmake-scripts/*", "LICENSE.txt"

    options = { 'spdlog_backend' : [True, False],
                'glog_backend' : [True, False],
                'log4cplus_backend' : [True, False] }

    default_options = { 'spdlog_backend': True,
                        'glog_backend': True,
                        'log4cplus_backend': True }

    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        if self.logr_build_test_and_others:
            self.exports_sources += "tests/*", "examples/*", "benchmarks/*"

    def requirements(self):
        self.requires( "fmt/7.1.2" )

    def build_requirements(self):
        if self.options.spdlog_backend:
            self.build_requires( "spdlog/1.8.0" )
            self.build_requires_options["spdlog"].header_only = True

            # For benchmarks in similar conditions,
            # spdlog should do no exception catching.
            self.build_requires_options["spdlog"].no_exceptions = True

        if self.options.glog_backend:
            self.build_requires( "glog/0.4.0" )

        if self.options.log4cplus_backend:
            self.build_requires( "log4cplus/2.0.5" )
            self.build_requires_options["log4cplus"].unicode = False

        self.build_requires( "gtest/1.10.0" )
        self.build_requires( "benchmark/1.5.2" )

    def _configure_cmake(self):
        cmake = CMake(self)
        cmake.definitions['LOGR_WITH_SPDLOG_BACKEND'] = self.options.spdlog_backend
        cmake.definitions['LOGR_WITH_GLOG_BACKEND'] = self.options.glog_backend
        cmake.definitions['LOGR_WITH_LOG4CPLUS_BACKEND'] = self.options.log4cplus_backend

        cmake.definitions['LOGR_BUILD_TESTS'] = self.logr_build_test_and_others
        cmake.definitions['LOGR_BUILD_EXAMPLES'] = self.logr_build_test_and_others
        cmake.definitions['LOGR_BUILD_BENCHMARKS'] = self.logr_build_test_and_others

        if self.settings.compiler == "gcc" or self.settings.compiler == "clang":
            cmake.definitions['EXPLICIT_LIBCXX'] = self.settings.compiler.libcxx

        if self.settings.compiler == "Visual Studio":
            rt = self.settings.compiler.runtime
            cmake.definitions['EXPLICIT_STATIC_RUNTIME'] = (rt == "MT" or rt == "MTd")

        cmake.configure()
        return cmake

    def package(self):
        cmake = self._configure_cmake()
        self.output.info(cmake.definitions)
        if self.logr_build_test_and_others:
            # If test are enabled build project and run tests.
            cmake.build()
            cmake.test(output_on_failure=True)

        cmake.install()

    def package_id(self):
        self.info.header_only()

    def package_info(self):
        self.info.header_only()
