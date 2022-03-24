# bgfx.cmake - bgfx building in cmake
# Written in 2017 by Joshua Brookover <joshua.al.brookover@gmail.com>

# To the extent possible under law, the author(s) have dedicated all copyright
# and related and neighboring rights to this software to the public domain
# worldwide. This software is distributed without any warranty.

# You should have received a copy of the CC0 Public Domain Dedication along with
# this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.

if( TARGET nvtt )
	return()
endif()

file(
	GLOB
	NVTT_SOURCES
	${BIMG_DIR}/3rdparty/nvtt/bc6h/*.cpp
	${BIMG_DIR}/3rdparty/nvtt/bc6h/*.h
	${BIMG_DIR}/3rdparty/nvtt/bc7/*.cpp
	${BIMG_DIR}/3rdparty/nvtt/bc7/*.h
	${BIMG_DIR}/3rdparty/nvtt/nvcore/*.h
	${BIMG_DIR}/3rdparty/nvtt/nvcore/*.inl
	${BIMG_DIR}/3rdparty/nvtt/nvmath/*.cpp
	${BIMG_DIR}/3rdparty/nvtt/nvmath/*.h
	${BIMG_DIR}/3rdparty/nvtt/*.cpp
	${BIMG_DIR}/3rdparty/nvtt/*.h
)

# disable 3rd-party warning
foreach(NVTT_SOURCE IN LISTS NVTT_SOURCES)
	if(MSVC)
		set_source_files_properties(${NVTT_SOURCE} PROPERTIES COMPILE_FLAGS "/W0" )
	else()
		set_source_files_properties(${NVTT_SOURCE} PROPERTIES COMPILE_FLAGS "-w" )
	endif()
endforeach()

add_library( nvtt STATIC ${NVTT_SOURCES} )
target_include_directories( nvtt
	PUBLIC
		$<BUILD_INTERFACE:${BIMG_DIR}/3rdparty>
		$<BUILD_INTERFACE:${BIMG_DIR}/3rdparty/nvtt> )
set_target_properties( nvtt PROPERTIES FOLDER "bgfx/3rdparty" )
target_link_libraries( nvtt PUBLIC bx )