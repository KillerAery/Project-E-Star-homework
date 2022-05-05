set( HOMEWORK_DIR "${CMAKE_CURRENT_SOURCE_DIR}/homework")

# 在此处添加文件
set( HOMEWORK_SOURCES
	${HOMEWORK_DIR}/homework.cpp
	${HOMEWORK_DIR}/MyApp.h
	${HOMEWORK_DIR}/MyApp.cpp
	${HOMEWORK_DIR}/Camera.h
	${HOMEWORK_DIR}/DirectionalLight.h
	${HOMEWORK_DIR}/LightProbe.h
	${HOMEWORK_DIR}/InputLayout.h
	${HOMEWORK_DIR}/InputLayout.cpp
	${HOMEWORK_DIR}/IMaterial.h
	${HOMEWORK_DIR}/BaseMaterial.h
	${HOMEWORK_DIR}/BaseMaterial.cpp
	${HOMEWORK_DIR}/BlinnPhongMaterial.h
	${HOMEWORK_DIR}/BlinnPhongMaterial.cpp
	${HOMEWORK_DIR}/PBRMaterial.h
	${HOMEWORK_DIR}/PBRMaterial.cpp
	${HOMEWORK_DIR}/IBLMaterial.h
	${HOMEWORK_DIR}/IBLMaterial.cpp
	${HOMEWORK_DIR}/ShadowMaterial.h
	${HOMEWORK_DIR}/ShadowMaterial.cpp
	${HOMEWORK_DIR}/SkyMaterial.h
	${HOMEWORK_DIR}/SkyMaterial.cpp
	${HOMEWORK_DIR}/IMesh.h
	${HOMEWORK_DIR}/CubeMesh.h
	${HOMEWORK_DIR}/CubeMesh.cpp
	${HOMEWORK_DIR}/PBRMesh.h
	${HOMEWORK_DIR}/PBRMesh.cpp
	${HOMEWORK_DIR}/SkyboxMesh.h
	${HOMEWORK_DIR}/SkyboxMesh.cpp
 )

add_executable( homework ${HOMEWORK_SOURCES} )
target_link_libraries( homework example-common )
target_include_directories( homework PRIVATE ${HOMEWORK_DIR} )

# Special Visual Studio Flags
if( MSVC )
	target_compile_definitions( homework PRIVATE "_CRT_SECURE_NO_WARNINGS" )
endif()
