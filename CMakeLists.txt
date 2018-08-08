#version 3.10 needed for CXX_STANDARD settings
cmake_minimum_required(VERSION 3.10)

# Specify default build type if none provided (before project() command)
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

# For generators with multiple configurations (like VS), only allow Debug and Release
if(CMAKE_CONFIGURATION_TYPES)
	set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Limited configurations" FORCE)
	mark_as_advanced(CMAKE_CONFIGURATION_TYPES)
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#add hades
add_subdirectory(hades)

add_subdirectory(breakout)
add_subdirectory(breakout_ex)
add_subdirectory(collision)
add_subdirectory(bounce)
#make visual studio debug the desired project by default
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT bounce)