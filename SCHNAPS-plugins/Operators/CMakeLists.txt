# CMakeLists.txt
# Auto configuration script
# For basic plugin, by Audrey Durand
# Laboratoire de vision et systèmes numériques, Université Laval
# Copyright (c) 2011
#

project(SCHNAPS_PLUGINS_OPERATORS CXX)
set(SCHNAPS_PLUGINS_OPERATORS_VERSION 2.0.1)

# This is the shared library versioning info
set(LIBRARY_VERSION 2.0.1)
#                   | | |
#            +------+ | +---+
#            |        |     |
#         major:minor:little
#            |        |     |
#            |        |     +- increment when little bugs are corrected
#            |        +- increment if minor changes occured such as new primitives added
#            +- increment if major changes occured (or milestone)

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
	message(STATUS "Building shared library")
	option(BUILD_STATIC_LIB "Build static library instead of shared one?" OFF)
endif(NOT DEFINED LIBRARY_TYPE)

if(BUILD_STATIC_LIB)
	set(SCHNAPS_PLUGINS_OPERATORS_LIBRARY_TYPE "STATIC")
else(BUILD_STATIC_LIB)
	set(SCHNAPS_PLUGINS_OPERATORS_LIBRARY_TYPE "SHARED")
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
find_library(PACC_LIB pacc)
if(PACC_LIB MATCHES PACC_LIB-NOTFOUND)
	message(FATAL_ERROR "Cannot find PACC library!")
else(PACC_LIB MATCHES PACC_LIB-NOTFOUND)
	# Extract path to PACC library
	get_filename_component(PACC_LIB_PATH ${PACC_LIB} PATH)
#	message(STATUS "PACC library found - OK")
	message(STATUS "++ PACC library found - ${PACC_LIB_PATH}")
endif(PACC_LIB MATCHES PACC_LIB-NOTFOUND)

message(STATUS "Checking for PACC headers...")
find_path(PACC_HEADERS PACC.hpp)
if(PACC_HEADERS MATCHES PACC_HEADERS-NOTFOUND)
	message(FATAL_ERROR "Cannot find PACC headers!")
else(PACC_HEADERS MATCHES PACC_HEADERS-NOTFOUND)
#	message(STATUS "PACC headers found - OK")
	message(STATUS "++ PACC headers found - ${PACC_HEADERS}")
endif(PACC_HEADERS MATCHES PACC_HEADERS-NOTFOUND)

# Check for SCHNAPS
message(STATUS "Checking for SCHNAPS library...")
find_library(SCHNAPS_LIB schnaps PATH_SUFFIXES "SCHNAPS")
if(SCHNAPS_LIB MATCHES SCHNAPS_LIB-NOTFOUND)
	message(FATAL_ERROR "Cannot find SCHNAPS library!")
else(SCHNAPS_LIB MATCHES SCHNAPS_LIB-NOTFOUND)
	# Extract path to SCHNAPS library
	get_filename_component(SCHNAPS_LIB_PATH ${SCHNAPS_LIB} PATH)
#	message(STATUS "SCHNAPS library found - OK")
	message(STATUS "++ SCHNAPS library found - ${SCHNAPS_LIB_PATH}")
endif(SCHNAPS_LIB MATCHES SCHNAPS_LIB-NOTFOUND)

message(STATUS "Checking for SCHNAPS headers...")
find_path(SCHNAPS_HEADERS SCHNAPS.hpp PATH_SUFFIXES "SCHNAPS")
if(SCHNAPS_HEADERS MATCHES SCHNAPS_HEADERS-NOTFOUND)
	message(FATAL_ERROR "Cannot find SCHNAPS headers!")
else(SCHNAPS_HEADERS MATCHES SCHNAPS_HEADERS-NOTFOUND)
	# Extract path to SCHNAPS headers
	get_filename_component(SCHNAPS_HEADERS_PATH ${SCHNAPS_HEADERS} PATH)
#	message(STATUS "SCHNAPS headers found - OK")
	message(STATUS "++ SCHNAPS headers found - ${SCHNAPS_HEADERS_PATH}")
endif(SCHNAPS_HEADERS MATCHES SCHNAPS_HEADERS-NOTFOUND)

# Check for dl (dynamic library support)
message(STATUS "Checking for dl library...")
find_library(DL_LIB dl)
if(DL_LIB MATCHES DL_LIB-NOTFOUND)
	message(FATAL_ERROR "Cannot find dl library!")
else(DL_LIB MATCHES DL_LIB-NOTFOUND)
	# Extract path to dl library
	get_filename_component(DL_LIB_PATH ${DL_LIB} PATH)
#	message(STATUS "dl library found - OK")
	message(STATUS "++ dl library found - ${DL_LIB_PATH}")
endif(DL_LIB MATCHES DL_LIB-NOTFOUND)

message(STATUS "Looking for SCHNAPS plugin sourcefiles...")
# Include all SCHNAPS plugin sourcefiles and headers
file(GLOB_RECURSE SCHNAPS_PLUGINS_OPERATORS_SOURCES src/SCHNAPS/Plugins/Operators/*.cpp )
file(GLOB SCHNAPS_PLUGINS_OPERATORS_HEADERS src/SCHNAPS/Plugins/Operators/*.hpp )

message(STATUS "Configuration...")

include_directories("${PROJECT_SOURCE_DIR}/src" "${CMAKE_CURRENT_BINARY_DIR}/include" ${PACC_HEADERS_PATH} ${SCHNAPS_HEADERS_PATH})
message(STATUS "++ include directories")

link_directories(${PACC_LIB_PATH} ${SCHNAPS_LIB_PATH} ${DL_LIB_PATH})
message(STATUS "++ link directories")

# use, i.e. don't skip the full RPATH for the build tree
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)
# when building, don't use the install RPATH already
# (but later on when installing)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# Setting up library
add_library(
	operators
	${SCHNAPS_PLUGINS_OPERATORS_LIBRARY_TYPE}
	${SCHNAPS_PLUGINS_OPERATORS_SOURCES}
	${SCHNAPS_PLUGINS_OPERATORS_AHEADERS}
	)

target_link_libraries(operators pacc)
target_link_libraries(operators schnaps)
target_link_libraries(operators dl)

set_target_properties(operators PROPERTIES VERSION ${LIBRARY_VERSION} LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib" COMPILE_FLAGS "-rdynamic")

# Install dependencies
install(FILES ${SCHNAPS_PLUGINS_OPERATORS_HEADERS} DESTINATION include/SCHNAPS/Plugins/Operators)

# Install targets
install(TARGETS operators
  LIBRARY DESTINATION lib/schnaps-plugins
  ARCHIVE DESTINATION lib/schnaps-plugins
)

include(CPack)
