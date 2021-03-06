# CMakeLists.txt
# Auto configuration script
# For SCHNAPS, by Audrey Durand
# Laboratoire de vision et systèmes numériques, Université Laval
# Copyright (c) 2011
#

project(SCHNAPS CXX)
set(SCHNAPS_VERSION 3.0.0)

# This is the shared library versioning info
set(LIBRARY_VERSION 3.0.0)
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
	message(STATUS "Building dynamic library")
	option(BUILD_STATIC_LIB "Build static library instead of shared one?" OFF)
endif(NOT DEFINED LIBRARY_TYPE)


if(BUILD_STATIC_LIB)
	set(SCHNAPS_LIBRARY_TYPE "STATIC")
else(BUILD_STATIC_LIB)
	set(SCHNAPS_LIBRARY_TYPE "SHARED")
endif(BUILD_STATIC_LIB)

# Define build mode
if(NOT DEFINED CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
	# Release mode as default
	message(STATUS "Building in release mode (default)...")
	set(CMAKE_BUILD_TYPE "RELEASE")
	set(SCHNAPS_NDEBUG true)
	set(SCHNAPS_FULL_DEBUG false)
else(NOT DEFINED CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
	if(CMAKE_BUILD_TYPE STREQUAL "RELEASE")
		# Release mode
		message(STATUS "Building in release mode...")
		set(CMAKE_BUILD_TYPE "RELEASE")
		set(SCHNAPS_NDEBUG true)
		set(SCHNAPS_FULL_DEBUG false)
	elseif(CMAKE_BUILD_TYPE STREQUAL "DEBUG")
		# Debug mode
		message(STATUS "Building in debug mode...")
		set(SCHNAPS_NDEBUG false)
		set(SCHNAPS_FULL_DEBUG true)
	elseif(CMAKE_BUILD_TYPE STREQUAL "MINSIZEREL" OR CMAKE_BUILD_TYPE STREQUAL "RELWITHDEBINFO")
		# Min size release
		message(STATUS "Building in release mode with some debug outputs...")
		set(CMAKE_BUILD_TYPE "RELEASE" FORCE)
		set(SCHNAPS_NDEBUG false)
		set(SCHNAPS_FULL_DEBUG false)
	else(CMAKE_BUILD_TYPE STREQUAL "RELEASE")
		# Unknown mode
		message(STATUS "## Warning : unhandled build mode ${CMAKE_BUILD_TYPE}, will build in release mode")
		set(CMAKE_BUILD_TYPE "RELEASE" FORCE)
		set(SCHNAPS_NDEBUG true)
		set(SCHNAPS_FULL_DEBUG false)
	endif(CMAKE_BUILD_TYPE STREQUAL "RELEASE")
endif(NOT DEFINED CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")

# Define OS
if(WIN32)
	set(SCHNAPS_IS_UNIX false)
	set(SCHNAPS_IS_WINDOWS true)
else(WIN32)
	set(SCHNAPS_IS_UNIX true)
	set(SCHNAPS_IS_WINDOWS false)
endif(WIN32)

# Others macros to include
include(CMakeMacros/PerformTest.cmake)

message(STATUS "Begin configuration tests...")

# Check for /dev/urandom
if(EXISTS /dev/urandom)
	set(SCHNAPS_HAVE_DEV_URANDOM 1)
endif(EXISTS /dev/urandom)

# Directory and extension used to find test files
if(NOT DEFINED CMAKE_DIR_TEST)
	set(CMAKE_DIR_TEST CMakeTestCode)
endif(NOT DEFINED CMAKE_DIR_TEST)
if(NOT DEFINED CMAKE_TEST_SUFFIX)
	set(CMAKE_TEST_SUFFIX ".cmake.cpp")
endif(NOT DEFINED CMAKE_TEST_SUFFIX)

# Check for hash_map C++ template
PerformTest(HASHMAP_STD)
PerformTest(HASHMAP_GNUCXX)
PerformTest(HASHMAP_STDEXT)

if(HASHMAP_STD)
	message(STATUS "++ std::hash_map found")
	set(SCHNAPS_HAVE_STD_HASHMAP 1)
endif(HASHMAP_STD)
if(HASHMAP_GNUCXX)
	message(STATUS "++ tr1/unordered_map found")
	set(SCHNAPS_HAVE_GNUCXX_HASHMAP 0)
	message(STATUS "++ tr1/unordered_map will not be enabled since segfault occurs while using it.")
endif(HASHMAP_GNUCXX)
if(HASHMAP_STDEXT)
	message(STATUS "++ stdext::hash_map found")
	set(SCHNAPS_HAVE_STDEXT_HASHMAP 1)
endif(HASHMAP_STDEXT)
if(NOT DEFINED HASHMAP_STD AND NOT DEFINED HASHMAP_GNUCXX AND NOT DEFINED HASHMAP_STDEXT)
	message(STATUS "++ no hash_map found")
endif(NOT DEFINED HASHMAP_STD AND NOT DEFINED HASHMAP_GNUCXX AND NOT DEFINED HASHMAP_STDEXT)

# Check for cast types
PerformTest(CXX_HAVE_DYN_CAST)
if(CXX_HAVE_DYN_CAST)
	message(STATUS "++ compiler supports dynamic_cast<>")
	set(SCHNAPS_HAVE_DYNAMIC_CAST 1)
else(CXX_HAVE_DYN_CAST)
	message(STATUS "++ dynamic_cast<> not supported")
endif(CXX_HAVE_DYN_CAST)

PerformTest(CXX_HAVE_NUMERIC_LIMITS)
if(CXX_HAVE_NUMERIC_LIMITS)
	message(STATUS "++ compiler defines numeric limits")
	set(SCHNAPS_HAVE_NUMERIC_LIMITS 1)
else(CXX_HAVE_NUMERIC_LIMITS)
	message(STATUS "++ compiler does not define numeric limits")
endif(CXX_HAVE_NUMERIC_LIMITS)

# Check for Run-time Type Identification
PerformTest(CXX_HAVE_RTTI)
if(CXX_HAVE_RTTI)
	message(STATUS "++ compiler supports Run-Time Type Identification")
	set(SCHNAPS_HAVE_RTTI 1)
else(CXX_HAVE_RTTI)
	message(STATUS "++ compiler does not support Run-Time Type Identification")
endif(CXX_HAVE_RTTI)

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
find_path(PACC_HEADERS_PATH PACC.hpp)
if(PACC_HEADERS_PATH MATCHES PACC_HEADERS_PATH-NOTFOUND)
	message(FATAL_ERROR "Cannot find PACC headers!")
else(PACC_HEADERS_PATH MATCHES PACC_HEADERS_PATH-NOTFOUND)
#	message(STATUS "PACC headers found - OK")
	message(STATUS "++ PACC headers found - ${PACC_HEADERS_PATH}")
endif(PACC_HEADERS_PATH MATCHES PACC_HEADERS_PATH-NOTFOUND)

# Check for zlib
message(STATUS "Checking for zlib library...")
find_library(ZLIB_LIB z)
if(ZLIB_LIB MATCHES ZLIB_LIB-NOTFOUND)
	message(FATAL_ERROR "Cannot find zlib library!")
else(ZLIB_LIB MATCHES ZLIB_LIB-NOTFOUND)
	# Extract path to zlib library
	get_filename_component(ZLIB_LIB_PATH ${ZLIB_LIB} PATH)
#	message(STATUS "zlib library found - OK")
	message(STATUS "++ zlib library found - ${ZLIB_LIB_PATH}")
endif(ZLIB_LIB MATCHES ZLIB_LIB-NOTFOUND)

message(STATUS "Checking for zlib headers...")
find_path(ZLIB_HEADERS_PATH zlib.h)
if(ZLIB_HEADERS_PATH MATCHES ZLIB_HEADERS_PATH-NOTFOUND)
	message(FATAL_ERROR "Cannot find zlib headers!")
else(ZLIB_HEADERS_PATH MATCHES ZLIB_HEADERS_PATH-NOTFOUND)
#	message(STATUS "zlib headers found - OK")
	message(STATUS "++ zlib headers found - ${ZLIB_HEADERS_PATH}")
endif(ZLIB_HEADERS_PATH MATCHES ZLIB_HEADERS_PATH-NOTFOUND)

# Check for dl (dynamic library support)
message(STATUS "Checking for dl library...")
find_library(DL_LIB dl PATH ${MORE_LIB_DIR})
if(DL_LIB MATCHES DL_LIB-NOTFOUND)
	message(FATAL_ERROR "Cannot find dl library!")
else(DL_LIB MATCHES DL_LIB-NOTFOUND)
	# Extract path to dl library
	get_filename_component(DL_LIB_PATH ${DL_LIB} PATH)
#	message(STATUS "dl library found - OK")
	message(STATUS "++ dl library found - ${DL_LIB_PATH}")
endif(DL_LIB MATCHES DL_LIB-NOTFOUND)

message(STATUS "Looking for SCHNAPS sourcefiles...")
# Include all SCHNAPS sourcefiles and headers
file(GLOB_RECURSE SCHNAPS_SOURCES src/SCHNAPS/*.cpp )
file(GLOB_RECURSE SCHNAPS_ALL_HEADERS src/SCHNAPS/*.hpp src/SCHNAPS/*.h)

# Include sourcefiles and headers by section
file(GLOB SCHNAPS_CORE_SRCS	src/SCHNAPS/Core/*.cpp)
file(GLOB SCHNAPS_SIMULATION_SRCS src/SCHNAPS/Simulation/*.cpp)

file(GLOB SCHNAPS_GEN_HEADERS src/SCHNAPS/*.hpp src/SCHNAPS/*.h)
file(GLOB SCHNAPS_CORE_HEADERS src/SCHNAPS/Core/*.hpp )
file(GLOB SCHNAPS_SIMULATION_HEADERS src/SCHNAPS/Simulation/*.hpp )

message(STATUS "Configuration...")
# Writing results in configuration file
configure_file("${PROJECT_SOURCE_DIR}/src/SCHNAPS/config.hpp.in" "${PROJECT_SOURCE_DIR}/src/SCHNAPS/config.hpp")
file(GLOB SCHNAPS_CONFIG_HEADER	"${CMAKE_CURRENT_BINARY_DIR}/include/SCHNAPS/config.hpp")
message(STATUS "++ config.hpp")

include_directories("${PROJECT_SOURCE_DIR}/src" "${CMAKE_CURRENT_BINARY_DIR}/include" ${PACC_HEADERS_PATH} ${ZLIB_HEADERS_PATH})
message(STATUS "++ include directories")

link_directories(${PACC_LIB_PATH} ${ZLIB_LIB_PATH} ${DL_LIB_PATH} "${CMAKE_INSTALL_PREFIX}/lib/schnaps-plugins")
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
	schnaps-lib
	${SCHNAPS_LIBRARY_TYPE}
	${SCHNAPS_SOURCES}
	${SCHNAPS_ALL_HEADERS}
	)

target_link_libraries(schnaps-lib pacc)
target_link_libraries(schnaps-lib z)
target_link_libraries(schnaps-lib dl)

set_target_properties(schnaps-lib PROPERTIES OUTPUT_NAME schnaps VERSION ${LIBRARY_VERSION} LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib" COMPILE_FLAGS "-rdynamic")

# Setting up executable
add_executable(
	schnaps-bin
	${SCHNAPS_SOURCES}
	${SCHNAPS_ALL_HEADERS}
	)

target_link_libraries(schnaps-bin pacc)
target_link_libraries(schnaps-bin z)
target_link_libraries(schnaps-bin dl)

set_target_properties(schnaps-bin PROPERTIES OUTPUT_NAME schnaps VERSION ${SCHNAPS_VERSION} RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin" COMPILE_FLAGS "-rdynamic")

# Install dependencies
install(FILES ${SCHNAPS_GEN_HEADERS} ${SCHNAPS_config_HEADER} DESTINATION include/SCHNAPS)
install(FILES ${SCHNAPS_CORE_HEADERS} DESTINATION include/SCHNAPS/Core)
install(FILES ${SCHNAPS_SIMULATION_HEADERS} DESTINATION include/SCHNAPS/Simulation)

# Install targets
install(TARGETS schnaps-bin schnaps-lib
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

include(CPack)
