from conan import ConanFile
from conan.errors import ConanInvalidConfiguration
from conan.tools.build import check_min_cppstd
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps, cmake_layout
from conan.tools.files import rmdir
from conan.tools.scm import Version
import os, sys, re

required_conan_version = ">=1.53.0"


class LogrConan(ConanFile):
    def set_version(self):
        version_file_path = os.path.join(
            self.recipe_folder,
            "logr/include/logr/version.hpp"
        )
        with open(version_file_path, 'r') as file:
            content = file.read()
            major_match = re.search(r'VERSION_MAJOR\s+(\d+)ull', content)
            minor_match = re.search(r'VERSION_MINOR\s+(\d+)ull', content)
            patch_match = re.search(r'VERSION_PATCH\s+(\d+)ull', content)

            if major_match and minor_match and patch_match:
                major = int(major_match.group(1))
                minor = int(minor_match.group(1))
                patch = int(patch_match.group(1))

                self.version = f"{major}.{minor}.{patch}"
            else:
                raise ValueError(f"cannot detect version from {version_file_path}")

    name = "logr"
    license = "BSD 3-Clause License"
    author = "Nicolai Grodzitski <utromvecherom@gmail.com>"
    url = "https://github.com/ngrodzitski/logr"
    description = "Logger frontend substitution for spdlog, glog, etc for server/desktop applications"
    topics = ("logger", "development", "util", "utils")

    settings = "os", "compiler", "build_type", "arch"

    exports_sources = [
        "CMakeLists.txt",
        "logr/*",
        "cmake-scripts/*"
    ]

    options = { 'spdlog_backend' : [True, False],
                'glog_backend' : [True, False],
                'log4cplus_backend' : [True, False],
                'boostlog_backend' : [True, False] }

    default_options = { 'spdlog_backend': True,
                        'glog_backend': True,
                        'log4cplus_backend': True,
                        'boostlog_backend' : True }


    no_copy_source = False
    build_policy = "missing"

    def _compiler_support_lut(self):
        return {
            "gcc": "9",
            "clang": "11",
            "apple-clang": "12",
            "Visual Studio": "17",
            "msvc": "191"
        }

    # This hint tells that this conanfile acts as
    # a conanfile for a package, which implies
    # it is responsible only for library itself.
    # Used to eliminate tests-related stuff (gtest, building tests)
    ACT_AS_PACKAGE_ONLY_CONANFILE = False

    def _is_package_only(self):
        return (
            self.ACT_AS_PACKAGE_ONLY_CONANFILE
            # The environment variable below can be used
            # to run conan create localy (used for debugging issues).
            or os.environ.get("LOGR_CONAN_PACKAGING") == "ON"
        )

    def requirements(self):
        self.requires( "fmt/10.1.1" )

        if self.options.spdlog_backend:
            self.requires( "spdlog/1.12.0" )

        if self.options.glog_backend:
            self.requires( "glog/0.6.0" )

        if self.options.log4cplus_backend:
            self.requires( "log4cplus/2.1.0" )

        if self.options.boostlog_backend:
            self.requires( "boost/1.83.0")

    def build_requirements(self):
        if not self._is_package_only():
            self.test_requires("gtest/1.14.0")
            self.test_requires("benchmark/1.8.3")

    def configure(self):
        if self.options.spdlog_backend:
            # self.options["spdlog"].header_only = True
            # For benchmarks in similar conditions,
            # spdlog should do no exception catching.
            self.options["spdlog"].no_exceptions = True
        if self.options.log4cplus_backend:
            self.options["log4cplus"].unicode = False

    def validate(self):
        minimal_cpp_standard = "17"
        if self.settings.compiler.get_safe("cppstd"):
            check_min_cppstd(self, minimal_cpp_standard)
        minimal_version = self._compiler_support_lut()

        compiler = str(self.settings.compiler)
        if compiler not in minimal_version:
            self.output.warning(
                "%s recipe lacks information about the %s compiler standard version support" % (self.name, compiler))
            self.output.warning(
                "%s requires a compiler that supports at least C++%s" % (self.name, minimal_cpp_standard))
            return

        version = Version(self.settings.compiler.version)
        if version < minimal_version[compiler]:
            raise ConanInvalidConfiguration("%s requires a compiler that supports at least C++%s" % (self.name, minimal_cpp_standard))

    def layout(self):
        cmake_layout(self, src_folder=".", build_folder=".")
        self.folders.generators = ""

    def generate(self):
        tc = CMakeToolchain(self)
        tc.variables["LOGR_INSTALL"] = True
        tc.variables[
            "LOGR_BUILD_TESTS"
        ] = not self._is_package_only()
        tc.variables[
            "LOGR_BUILD_EXAMPLES"
        ] = not self._is_package_only()
        tc.variables[
            "LOGR_BUILD_BENCHMARK"
        ] = not self._is_package_only()

        tc.variables['LOGR_WITH_SPDLOG_BACKEND'] = self.options.spdlog_backend
        tc.variables['LOGR_WITH_GLOG_BACKEND'] = self.options.glog_backend
        tc.variables['LOGR_WITH_LOG4CPLUS_BACKEND'] = self.options.log4cplus_backend
        tc.variables['LOGR_WITH_BOOSTLOG_BACKEND'] = self.options.boostlog_backend

        tc.generate()

        cmake_deps = CMakeDeps(self)
        cmake_deps.generate()

    def package_id(self):
        self.info.clear()

    def build(self):
        cmake = CMake(self)
        cmake.configure(build_script_folder=self.source_folder)
        cmake.build()

    def package(self):
        cmake = CMake(self)
        if not self._is_package_only():
            cmake.build()
            cmake.test()
        cmake.install()
        rmdir(self, os.path.join(self.package_folder, "lib", "cmake"))

    def package_info(self):
        self.cpp_info.bindirs = []
        self.cpp_info.frameworkdirs = []
        self.cpp_info.libdirs = []
        self.cpp_info.set_property("cmake_file_name", "logr")

        self.cpp_info.names["cmake_find_package"] = "logr"
        self.cpp_info.names["cmake_find_package_multi"] = "logr"

        self.cpp_info.components["logr_base"].includedirs = ["include"]
        self.cpp_info.components["logr_base"].requires = ["fmt::fmt"]

        if self.options.spdlog_backend:
            self.cpp_info.components["logr_spdlog"].includedirs = []
            self.cpp_info.components["logr_spdlog"].requires = [
                "logr_base",
                "spdlog::spdlog",
            ]
        if self.options.glog_backend:
            self.cpp_info.components["logr_glog"].includedirs = []
            self.cpp_info.components["logr_glog"].requires = [
                "logr_base",
                "glog::glog",
            ]
        if self.options.log4cplus_backend:
            self.cpp_info.components["logr_log4cplus"].includedirs = []
            self.cpp_info.components["logr_log4cplus"].requires = [
                "logr_base",
                "log4cplus::log4cplus",
            ]
        if self.options.boostlog_backend:
            self.cpp_info.components["logr_boostlog"].includedirs = []
            self.cpp_info.components["logr_boostlog"].requires = [
                "logr_base",
                "boost::log",
            ]
