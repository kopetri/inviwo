
# Search for zlib

find_package(ZLIB QUIET)
# For some reason we cannot entirely trust that 
# ZLIB_FOUND is set correctly (Windows). We therefore also
# check if the include directories are set and exists
if (ZLIB_INCLUDE_DIRS)
    foreach(include_dir ${ZLIB_INCLUDE_DIRS})
        set(ZLIB_FOUND (ZLIB_FOUND AND EXISTS ${include_dir}))
    endforeach(include_dir)
else() 
    set(ZLIB_FOUND false)
endif()

# Use existing zlib or build dependency from source if not found
if (ZLIB_FOUND)
	# we need to make the ZLIB configuration available to other modules (via PARENT_SCOPE)
	# in case zlib was found by find_package()

	set(ZLIB_INCLUDE_DIRS ${ZLIB_INCLUDE_DIRS} PARENT_SCOPE)
	# Some libraries use ZLIB_INCLUDE_DIR even though find_package(ZLIB) specifies that ZLIB_INCLUDE_DIRS should be used
	set(ZLIB_INCLUDE_DIR ${ZLIB_INCLUDE_DIR} PARENT_SCOPE)

	set(ZLIB_LIBRARIES ${ZLIB_LIBRARIES} PARENT_SCOPE)
	
	set(ZLIB_VERSION_STRING ${VERSION} PARENT_SCOPE)
	set(ZLIB_VERSION_MAJOR 1 PARENT_SCOPE)
	set(ZLIB_VERSION_MINOR 2 PARENT_SCOPE)
	set(ZLIB_VERSION_PATCH 8 PARENT_SCOPE)
	set(ZLIB_VERSION_TWEAK 0 PARENT_SCOPE)

	set(ZLIB_FOUND ${ZLIB_FOUND} PARENT_SCOPE)
else()
	ivw_debug_message(STATUS "Compiling zlib from source since it was not found")
	if(BUILD_SHARED_LIBS)
		add_definitions(-DZLIB_INTERNAL)
		ivw_add_definition(ZLIB_DLL)
	endif()

	add_subdirectory(ext/zlib-1.2.8)

	set(ZLIB_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/ext/zlib-1.2.8 ${CMAKE_CURRENT_BINARY_DIR}/ext/zlib-1.2.8 PARENT_SCOPE)
	set(ZLIB_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/ext/zlib-1.2.8 ${CMAKE_CURRENT_BINARY_DIR}/ext/zlib-1.2.8)
	# Some libraries use ZLIB_INCLUDE_DIR even though find_package(ZLIB) specifies that ZLIB_INCLUDE_DIRS should be used
	set(ZLIB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ext/zlib-1.2.8 ${CMAKE_CURRENT_BINARY_DIR}/ext/zlib-1.2.8 PARENT_SCOPE)
	
	# overwrite the ZLIB_INCLUDE_DIR in the cache (created by find_pacakge(ZLIB)) and hide it
	set(ZLIB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ext/zlib-1.2.8 ${CMAKE_CURRENT_BINARY_DIR}/ext/zlib-1.2.8 CACHE STRING "zlib_include_dir" FORCE)

	mark_as_internal(ZLIB_LIBRARY_DEBUG)
	mark_as_internal(ZLIB_LIBRARY_RELEASE)
	
	set(ZLIB_VERSION_STRING ${VERSION} PARENT_SCOPE)
	set(ZLIB_VERSION_MAJOR 1 PARENT_SCOPE)
	set(ZLIB_VERSION_MINOR 2 PARENT_SCOPE)
	set(ZLIB_VERSION_PATCH 8 PARENT_SCOPE)
	set(ZLIB_VERSION_TWEAK 0 PARENT_SCOPE)

	set(ZLIB_FOUND 1 PARENT_SCOPE)
	set(ZLIB_FOUND 1 CACHE BOOL "zlib_found" FORCE)
	if (BUILD_SHARED_LIBS)
		set(ZLIB_LIBRARIES zlib PARENT_SCOPE)
		set(ZLIB_LIBRARIES zlib)
	else()
		set(ZLIB_LIBRARIES zlibstatic PARENT_SCOPE)
		set(ZLIB_LIBRARIES zlibstatic)
    endif()
endif()

#--------------------------------------------------------------------
# Inviwo zlib Module
ivw_module(zlib)

ivw_include_directories(${ZLIB_INCLUDE_DIRS})
add_dependency_libs_to_module(${ZLIB_LIBRARIES})
#--------------------------------------------------------------------
# Add header files
set(HEADER_FILES
    #${CMAKE_CURRENT_SOURCE_DIR}/zlibprocessor.h
)
ivw_group("Header Files" ${HEADER_FILES})

#--------------------------------------------------------------------
# Add source files
set(SOURCE_FILES
    #${CMAKE_CURRENT_SOURCE_DIR}/zlibprocessor.cpp
)
ivw_group("Source Files" ${SOURCE_FILES})


#--------------------------------------------------------------------
# Add shaders
set(SHADER_FILES
    #${CMAKE_CURRENT_SOURCE_DIR}/glsl/zlibprocessor.frag
)
ivw_group("Shader Files" ${SHADER_FILES})


#--------------------------------------------------------------------
# Add Unittests
set(TEST_FILES
	#${CMAKE_CURRENT_SOURCE_DIR}/tests/zlib-test.cpp
)
ivw_add_unittest(${TEST_FILES})

#--------------------------------------------------------------------
# Create module
ivw_create_module(${SOURCE_FILES} ${HEADER_FILES} ${SHADER_FILES})

#--------------------------------------------------------------------
# Add shader directory to pack
# ivw_add_to_module_pack(${CMAKE_CURRENT_SOURCE_DIR}/glsl)