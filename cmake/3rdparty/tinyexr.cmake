if( TARGET tinyexr )
	return()
endif()

file( GLOB_RECURSE TINYEXR_SOURCES ${BIMG_DIR}/3rdparty/tinyexr/*.c ${BIMG_DIR}/3rdparty/tinyexr/*.h )

# disable 3rd-party warning
foreach(TINYEXR_SOURCE IN LISTS TINYEXR_SOURCES)
	if(MSVC)
		set_source_files_properties(${TINYEXR_SOURCE} PROPERTIES COMPILE_FLAGS "/W0" )
	else()
		set_source_files_properties(${TINYEXR_SOURCE} PROPERTIES COMPILE_FLAGS "-w" )
	endif()
endforeach()

add_library( tinyexr STATIC ${TINYEXR_SOURCES} )
target_include_directories( tinyexr PUBLIC $<BUILD_INTERFACE:${BIMG_DIR}/3rdparty> $<BUILD_INTERFACE:${BIMG_DIR}/3rdparty/tinyexr/deps/miniz> )
set_target_properties( tinyexr PROPERTIES FOLDER "bgfx/3rdparty" )