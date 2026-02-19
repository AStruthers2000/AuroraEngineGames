from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout


class PongConan(ConanFile):
    name = "pong"
    author = "AStruthers2000"
    description = "A simple Pong game built with C++23 and SDL3"

    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps"

    exports_sources = "CMakeLists.txt", "src/*", "public/*"

    def requirements(self):
        self.requires("aurora-engine/0.0.1")

    def layout(self):
        cmake_layout(self)

