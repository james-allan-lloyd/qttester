cmake_minimum_required(VERSION 2.8.11)

# set(Python_ADDITIONAL_VERSIONS 3.6)
project(QtTester)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

find_package(PythonLibs 3 REQUIRED)
find_package(Boost COMPONENTS python3)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

include_directories(${PYTHON_INCLUDE_DIRS})

# Find the QtWidgets library
find_package(Qt5Widgets)

set(SOURCES ${SOURCES} main.cpp mainwindow.cpp)
qt5_wrap_ui(SOURCES mainwindow.ui)

# Tell CMake to create the helloworld executable
add_executable(test-app WIN32 ${SOURCES})
file(WRITE ${CMAKE_BINARY_DIR}/bin/qt.conf "[Paths]\nPrefix = ..\n")

# Use the Widgets module from Qt 5.
target_link_libraries(test-app Qt5::Widgets ${PYTHON_LIBRARIES})
