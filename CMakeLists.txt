cmake_minimum_required (VERSION 2.6)

#Create options
option(BUILD_STATIC "Tell if the static library should be compiled" ON)
option(BUILD_DYNAMIC "Tell if the dynamic library should be compiled" ON)
option(BUILD_DOC "Tell if the documentation target (make doc) should be added" ON)
option(BUILD_TEST "Compile the test applications and allow to run them with 'make test'" ON)
option(BUILD_EXAMPLES "Tell if we should also compile the examples" ON)

###################
# Backend options #
###################

#EPoll
if(WIN32)
  option(BACKEND_EPOLL "Activate the EPOLL backend (linux only)" OFF)
else(WIN32)
  option(BACKEND_EPOLL "Activate the EPOLL backend (linux only)" ON)
endif(WIN32)

#Select
if(WIN32)
  option(BACKEND_SELECT "Activate the select back-end (windows, linux, macosx, bsd)" ON)
else(WIN32)
  option(BACKEND_SELECT "Activate the select back-end (windows, linux, macosx, bsd)" OFF)
endif(WIN32)

#WSAPoll
if(WIN32)
  option(BACKEND_WSAPOLL "Activate the WSAPOLL backend (windows > Vista only)" OFF)
  add_definitions(-DSEDNL_WINDOWS)
endif(WIN32)

# Set up backends

if((BACKEND_SELECT AND BACKEND_EPOLL)
    OR (BACKEND_WSAPOLL AND BACKEND_EPOLL)
    OR (BACKEND_SELECT AND BACKEND_WSAPOLL))
  message(SEND_ERROR "You can't have more than one backend!")
endif()

if(NOT (BACKEND_SELECT OR BACKEND_EPOLL OR BACKEND_WSAPOLL))
  message(SEND_ERROR "You should choose a backend. The default one is EPOLL for linux, SELECT for windows")
endif()


if(BACKEND_SELECT)
  add_definitions(-DSEDNL_BACKEND_SELECT)
endif(BACKEND_SELECT)

if(BACKEND_EPOLL)
  add_definitions(-DSEDNL_BACKEND_EPOLL)
endif(BACKEND_EPOLL)

if(BACKEND_WSAPOLL)
  add_definitions(-DSEDNL_BACKEND_WSAPOLL)
endif(BACKEND_WSAPOLL)


# Project name
project(SEDNL)

# C++ 2011 flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -W -Wall -fvisibility=hidden")

#Remove warning from the use of gcc's __attribute__ visibility with 'enum class'.
if(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-attributes")
endif(CMAKE_COMPILER_IS_GNUCXX)

#C 99 flag
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -O3 -W -Wall -fvisibility=hidden")

# Set version numbers
set(SEDNL_VERSION_MAJOR 1)
set(SEDNL_VERSION_MINOR 0)
set(SEDNL_VERSION_MICRO 0)

#define the dyn/static library name
set(SEDNL_INCLUDE_DIRECTORY_NAME "sednl-${SEDNL_VERSION_MAJOR}.${SEDNL_VERSION_MINOR}/SEDNL")
set(SEDNL_LIBRARY_NAME "sednl-${SEDNL_VERSION_MAJOR}.${SEDNL_VERSION_MINOR}")

#Set include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

#Add subdirectories

add_subdirectory(src/SEDNL)
if(BUILD_DOC)
  add_subdirectory(doc)
endif(BUILD_DOC)
if(BUILD_TEST)
enable_testing()
add_subdirectory(test)
endif(BUILD_TEST)
if(BUILD_EXAMPLES)
  add_subdirectory(examples)
endif(BUILD_EXAMPLES)

