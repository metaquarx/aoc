# SPDX-FileCopyrightText: 2021 metaquarx <metaquarx@protonmail.com>
# SPDX-License-Identifier: Apache-2.0

# Setup

cmake_minimum_required(VERSION 3.20 FATAL_ERROR)
project(aoc VERSION 0.1.0 LANGUAGES CXX)

# Build types

set(default_build_type "Release")
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
	set (CMAKE_BUILD_TYPE "${default_build_type}" CACHE STRING "Choose the type of build." FORCE)

	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# Helpers

add_library(Helpers SHARED "src/Helpers.cpp")
set_target_properties(Helpers PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
)
target_include_directories(Helpers PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")

# aoc

macro(RegisterFiles FILES YEAR)
	add_custom_target("${YEAR}")
	foreach(FILE ${FILES})
		add_executable(${FILE} EXCLUDE_FROM_ALL "${CMAKE_CURRENT_SOURCE_DIR}/${FILE}.cpp")
		set_target_properties(${FILE} PROPERTIES
			CXX_STANDARD 17
			CXX_STANDARD_REQUIRED ON
			CXX_EXTENSIONS OFF
		)
		target_link_libraries(${FILE} PRIVATE Helpers)
		add_dependencies(${YEAR} ${FILE})
	endforeach()
endmacro()

# years

add_subdirectory("src/2021" "2021")
