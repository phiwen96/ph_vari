cmake_minimum_required (VERSION 3.19.4)
get_filename_component (self ${CMAKE_CURRENT_SOURCE_DIR} NAME)

project (
	${self}
	VERSION 1.0
	DESCRIPTION "Very nice project"
	LANGUAGES CXX
)	


add_subdirectory (cmake/phake)
# if (NOT TARGET ph_coroutines)
# 	add_subdirectory (dependencies/ph_coroutines)
# endif ()
if (NOT TARGET ph_type_list)
	add_subdirectory (dependencies/ph_type_list)
endif ()
if (NOT TARGET ph_macros)
	add_subdirectory (dependencies/ph_macros)
endif ()
if (NOT TARGET ph_time)
	add_subdirectory (dependencies/ph_time)
endif ()
if (NOT TARGET ph_type)
	add_subdirectory (dependencies/ph_type)
endif ()
# if (NOT TARGET ph_memory)
# 	add_subdirectory (dependencies/ph_memory)
# endif ()

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
	
	# Let's ensure -std=g++xx instead of -std=c++xx
	set (CMAKE_CXX_STANDARD 20)
	set (CMAKE_CXX_STANDARD_REQUIRED ON)
	set (CMAKE_CXX_EXTENSIONS ON)

	# Let's nicely support folders in IDEs
	set_property (GLOBAL PROPERTY USE_FOLDERS ON)



	# Testing only available if this is the main app
	# Note this needs to be done in the main CMakeLists
	# since it calls enable_testing, which must be in the
	# main CMakeLists.
	include (CTest)



   # Docs only available if this is the main app
   	# find_package (Doxygen)

	# if (Doxygen_FOUND)
	# 	add_subdirectory (docs)
   	# else()
    #  		message(STATUS "Doxygen not found, not building docs")
   	# endif()


endif()

find_package (Boost 1.74 REQUIRED)

add_subdirectory (src)


# if ((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR MODERN_CMAKE_BUILD_TESTING) AND BUILD_TESTING)
	add_subdirectory (tests)
# endif()