# CMakeLists.txt
# Auto configuration script
# For basic plugin, by Audrey Durand
# Laboratoire de vision et systèmes numériques, Université Laval
# Copyright (c) 2011
#

project(SCHNAPSPLUGIN_BASIC CXX)
set(SCHNAPSPLUGIN_BASIC_VERSION 1.0)

# This is the shared library versioning info
set(LIBRARY_VERSION 1.0.0)
#                   | | |
#            +------+ | +---+
#            |        |     |
#         current:revision:age
#            |        |     |
#            |        |     +- increment if interfaces have been added
#            |        |        set to zero if interfaces have been removed
#            |        |        or changed
#            |        +- increment if source code has changed
#            |           set to zero if current is incremented
#            +- increment if interfaces have been added, removed or changed

# Minimum cmake version required
cmake_minimum_required(VERSION 2.6)

# Define whether we build static or shared libs
if(NOT DEFINED LIBRARY_TYPE)
	if(NOT MSVC)
		message(STATUS "Building shared library (default)")
		option(BUILD_STATIC_LIB "Build static library instead of shared one?" OFF)
	else(NOT MSVC)
		message(STATUS "Building static library (default on Microsoft Visual C++)")
		option(BUILD_STATIC_LIB "Build static library instead of shared one?" ON)
	endif(NOT MSVC)
elseif(LIBRARY_TYPE MATCHES "static")
	message(STATUS "Building static library")
	option(BUILD_STATIC_LIB "Build static library instead of shared one?" ON)
else(NOT DEFINED LIBRARY_TYPE)
	message(STATUS "Building dynamic library")
	option(BUILD_STATIC_LIB "Build static library instead of shared one?" OFF)
endif(NOT DEFINED LIBRARY_TYPE)


if(BUILD_STATIC_LIB)
	set(SCHNAPSPLUGIN_BASIC_LIBRARY_TYPE "STATIC")
else(BUILD_STATIC_LIB)
	set(SCHNAPSPLUGIN_BASICS_LIBRARY_TYPE "SHARED")
endif(BUILD_STATIC_LIB)

# Define build mode
if(NOT DEFINED CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
	# Release mode as default
	message(STATUS "Building in release mode (default)...")
	set(CMAKE_BUILD_TYPE "RELEASE")
else(NOT DEFINED CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
	if(CMAKE_BUILD_TYPE STREQUAL "RELEASE")
		# Release mode
		message(STATUS "Building in release mode...")
		set(CMAKE_BUILD_TYPE "RELEASE")
	elseif(CMAKE_BUILD_TYPE STREQUAL "DEBUG")
		# Debug mode
		message(STATUS "Building in debug mode...")
	elseif(CMAKE_BUILD_TYPE STREQUAL "MINSIZEREL" OR CMAKE_BUILD_TYPE STREQUAL "RELWITHDEBINFO")
		# Min size release
		message(STATUS "Building in release mode with some debug outputs...")
		set(CMAKE_BUILD_TYPE "RELEASE" FORCE)
	else(CMAKE_BUILD_TYPE STREQUAL "RELEASE")
		# Unknown mode
		message(STATUS "## Warning : unhandled build mode ${CMAKE_BUILD_TYPE}, will build in release mode")
		set(CMAKE_BUILD_TYPE "RELEASE" FORCE)
	endif(CMAKE_BUILD_TYPE STREQUAL "RELEASE")
endif(NOT DEFINED CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")

message(STATUS "Begin configuration tests...")

# Check for PACC
message(STATUS "Checking for PACC library...")
find_library(PACC_LIB pacc PATH ${MORE_LIB_DIR})
if(PACC_LIB MATCHES PACC_LIB-NOTFOUND)
	message(FATAL_ERROR "Cannot find PACC library!")
else(PACC_LIB MATCHES PACC_LIB-NOTFOUND)
#	message(STATUS "PACC library found - OK")
	message(STATUS "++ PACC library found - ${PACC_LIB}")
endif(PACC_LIB MATCHES PACC_LIB-NOTFOUND)
# Extract path to PACC library
get_filename_component(PACC_LIB_DIR ${PACC_LIB} PATH)

message(STATUS "Checking for PACC headers...")
find_path(PACC_HEADERS PACC.hpp PATH ${MORE_INCLUDE_DIR})
if(PACC_HEADERS MATCHES PACC_HEADERS-NOTFOUND)
	message(FATAL_ERROR "Cannot find PACC headers!")
else(PACC_HEADERS MATCHES PACC_HEADERS-NOTFOUND)
#	message(STATUS "PACC headers found - OK")
	message(STATUS "++ PACC headers found - ${PACC_HEADERS}")
endif(PACC_HEADERS MATCHES PACC_HEADERS-NOTFOUND)

# Check for SCHNAPS
message(STATUS "Checking for SCHNAPS library...")
find_library(SCHNAPS_LIB schnaps PATH ${MORE_LIB_DIR})
if(SCHNAPS_LIB MATCHES SCHNAPS_LIB-NOTFOUND)
	message(FATAL_ERROR "Cannot find SCHNAPS library!")
else(SCHNAPS_LIB MATCHES SCHNAPS_LIB-NOTFOUND)
#	message(STATUS "SCHNAPS library found - OK")
	message(STATUS "++ SCHNAPS library found - ${SCHNAPS_LIB}")
endif(SCHNAPS_LIB MATCHES SCHNAPS_LIB-NOTFOUND)
# Extract path to SCHNAPS library
get_filename_component(SCHNAPS_LIB_DIR ${SCHNAPS_LIB} PATH)

message(STATUS "Checking for SCHNAPS headers...")
find_path(SCHNAPS_HEADERS SCHNAPS.hpp PATH ${MORE_INCLUDE_DIR})
if(SCHNAPS_HEADERS MATCHES SCHNAPS_HEADERS-NOTFOUND)
	message(FATAL_ERROR "Cannot find SCHNAPS headers!")
else(SCHNAPS_HEADERS MATCHES PACC_HEADERS-NOTFOUND)
#	message(STATUS "SCHNAPS headers found - OK")
	message(STATUS "++ SCHNAPS headers found - ${SCHNAPS_HEADERS}")
endif(SCHNAPS_HEADERS MATCHES SCHNAPS_HEADERS-NOTFOUND)

# Check for dl (dynamic library support)
message(STATUS "Checking for dl library...")
find_library(DL_LIB dl PATH ${MORE_LIB_DIR})
if(DL_LIB MATCHES DL_LIB-NOTFOUND)
	message(FATAL_ERROR "Cannot find dl library!")
else(DL_LIB MATCHES DL_LIB-NOTFOUND)
#	message(STATUS "dl library found - OK")
	message(STATUS "++ dl library found - ${DL_LIB}")
endif(DL_LIB MATCHES DL_LIB-NOTFOUND)
# Extract path to dl library
get_filename_component(DL_LIB_DIR ${DL_LIB} PATH)

message(STATUS "Looking for SCHNAPS plugin Basic sourcefiles...")
# Include all SCHNAPS plugin Basic sourcefiles and headers
file(GLOB_RECURSE SCHNAPSPLUGIN_BASIC_SOURCES src/SCHNAPS-plugins/Basic/*.cpp )
file(GLOB_RECURSE SCHNAPSPLUGIN_BASICS_ALL_HEADERS src/SCHNAPS-plugins/Basic/*.hpp)

file(GLOB SCHNAPSPLUGIN_BASIC_MAIN_HEADER src/SCHNAPS-plugins/Basic/Basic.hpp)
file(GLOB SCHNAPSPLUGIN_BASIC_GEN_HEADERS src/SCHNAPS-plugins/Basic/*.hpp )

message(STATUS "Configuration...")

include_directories("${PROJECT_SOURCE_DIR}/src" "${CMAKE_CURRENT_BINARY_DIR}/include" ${PACC_HEADERS} ${SCHNAPS_HEADERS} ${ZLIB_HEADERS})
link_directories(${PACC_LIB_DIR} ${SCHNAPS_LIB_DIR} ${DL_LID_DIR})

# Setting up library
add_library(
	basic
	${SCHNAPSPLUGIN_BASIC_LIBRARY_TYPE}
	${SCHNAPSPLUGIN_BASIC_SOURCES}
	${SCHNAPSPLUGIN_BASIC_ALL_HEADERS}
	)

target_link_libraries(basic pacc)
target_link_libraries(basic schnaps)
target_link_libraries(basic dl)

set_target_properties(basic PROPERTIES VERSION ${LIBRARY_VERSION} LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib" COMPILE_FLAGS "-rdynamic")

# Install dependencies
install(FILES ${SCHNAPSPLUGIN_BASIC_MAIN_HEADER} DESTINATION include/SCHNAPS-plugins)
install(FILES ${SCHNAPSPLUGIN_BASIC_GEN_HEADERS} DESTINATION include/SCHNAPS-plugins/Basic)

# Install targets
install(TARGETS basic
  LIBRARY DESTINATION lib/SCHNAPS-plugins
  ARCHIVE DESTINATION lib/SCHNAPS-plugins
)

include(CPack)
