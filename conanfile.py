from conans import ConanFile, CMake

boost_lib_list = ['math', 'wave', 'container', 'contract', 'exception', 'graph', 'iostreams', 'locale', 'log',
                  'program_options', 'random', 'regex', 'mpi', 'serialization', 'signals',
                  'coroutine', 'fiber', 'context', 'timer', 'thread', 'chrono', 'date_time',
                  'atomic', 'filesystem', 'system', 'graph_parallel', 'python',
                  'stacktrace', 'test', 'type_erasure']

class QtTesterConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    requires = "Qt/5.11.0@bincrafters/stable", "boost/1.68.0@conan/stable"
    build_requires = "cmake_installer/3.11.2@conan/stable"
    generators = "cmake", "gcc", "txt", "virtualenv"

    default_options = ["boost:without_%s=True" % libname for libname in boost_lib_list if libname != "python"]
    default_options.append("boost:without_python=False")
    default_options = tuple(default_options)

    def imports(self):
        self.copy("*.dll", dst="bin", src="bin") # From bin to bin
        self.copy("*.dylib*", dst="bin", src="lib") # From lib to bin
        self.copy("*", dst="plugins/platforms", src="plugins/platforms")

    def configure(self):
        # for without_lib in :
        #     if without_lib.endswith("python"):
        #         setattr(self.options['boost'], without_lib, False)
        #     else:
        #         setattr(self.options['boost'], without_lib, True)
        pass


    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
