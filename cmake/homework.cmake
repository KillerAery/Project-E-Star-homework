set( HOMEWORK_DIR "${CMAKE_CURRENT_SOURCE_DIR}/homework")

# 在此处添加文件
set( HOMEWORK_SOURCES
	${HOMEWORK_DIR}/homework.cpp
	${HOMEWORK_DIR}/bgfx_logo.h
 )

add_executable( homework ${HOMEWORK_SOURCES} )
target_link_libraries( homework example-common )
target_include_directories( homework PRIVATE ${HOMEWORK_DIR} )

# Special Visual Studio Flags
if( MSVC )
	target_compile_definitions( homework PRIVATE "_CRT_SECURE_NO_WARNINGS" )
endif()
