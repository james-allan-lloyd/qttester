cmake_minimum_required(VERSION 2.8.11)

# set(Python_ADDITIONAL_VERSIONS 3.6)
project(QtTester)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
add_definitions(-DBOOST_DEBUG_PYTHON=1)
conan_basic_setup(TARGETS)
#add_definitions(${CONAN_DEFINES})

find_package(Python3 COMPONENTS Development)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

include_directories(${Python3_INCLUDE_DIRS})
link_directories(${Python3_LIBRARY_DIRS})

# Find the QtWidgets library
find_package(Qt5Widgets)

set(SOURCES
    ${SOURCES}
    main.cpp
    mainwindow.cpp
    applicationundertest.cpp
    qttester.py)
qt5_wrap_ui(SOURCES mainwindow.ui)

add_executable(test-app ${SOURCES})
file(WRITE ${CMAKE_BINARY_DIR}/bin/qt.conf "[Paths]\nPrefix = ..\n")
target_link_libraries(test-app Qt5::Widgets CONAN_PKG::boost Python3::Python)
