# ~ 2024_moteur_gpm_2027_gpm_2027_projet_moteur-mustang/engine/CMakeLists.txt

# CMake Prototype
get_filename_component(TARGET_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

message(STATUS "[${TARGET_NAME}] Starting source file fetching..")

set(ENGINE_SHADER_DIR ${CMAKE_SOURCE_DIR}/workspace/shaders)
set(ENGINE_SCRIPT_DIR ${CMAKE_SOURCE_DIR}/workspace/scripts)

# -- Header files -- #
file(GLOB_RECURSE TARGET_HEADER_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/*.inl
)

# -- Source files -- #
file(GLOB_RECURSE TARGET_SOURCE_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/*.c
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

# -- Shader files -- #
file(GLOB TARGET_ENGINE_SHADER_FILES
	${ENGINE_SHADER_DIR}/*.vs
	${ENGINE_SHADER_DIR}/*.vert
	${ENGINE_SHADER_DIR}/*.fs
	${ENGINE_SHADER_DIR}/*.frag
)

# -- Extra files -- #
file(GLOB TARGET_EXTRA_FILES
	${CMAKE_SOURCE_DIR}/.gitignore
	${CMAKE_SOURCE_DIR}/.gitattributes
	${CMAKE_SOURCE_DIR}/*.md
    ${CMAKE_SOURCE_DIR}/*.txt
)

file(GLOB_RECURSE TARGET_SCRIPT_FILES
	${ENGINE_SCRIPT_DIR}/scripts/*.lua
)

set(TARGET_FILES ${TARGET_HEADER_FILES} ${TARGET_SOURCE_FILES})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${TARGET_FILES})
source_group("Shaders" FILES ${TARGET_ENGINE_SHADER_FILES})
source_group("Extra files" FILES ${TARGET_EXTRA_FILES})
source_group("Scripts" FILES ${TARGET_SCRIPT_FILES})

add_library(${TARGET_NAME} SHARED)

target_sources(${TARGET_NAME} PRIVATE ${TARGET_FILES} ${TARGET_EXTRA_FILES} ${TARGET_ENGINE_SHADER_FILES} ${TARGET_SCRIPT_FILES})

target_include_directories(${TARGET_NAME}
	PUBLIC ${MATHLIB_INCLUDE_DIR} 
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/public
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/public/engine
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/private
)

target_link_libraries(${TARGET_NAME}
	PRIVATE ${DEPENDENCIES_LIBRARY}
	PUBLIC ${MATHLIB_LIB}
)

if (MSVC)
	# Disable linker warning for glfw3's missing debug info
	# and ignore non multithread runtime libs
	target_link_options(${TARGET_NAME} PRIVATE
	/IGNORE:4099
	/NODEFAULTLIB:libcmt.lib
	/NODEFAULTLIB:libcmtd.lib
	)

	target_compile_options(${TARGET_NAME} PRIVATE /W4 /WX)
	set_target_properties(${TARGET_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/workspace)

endif()

set(ENGINE_LIB ${TARGET_NAME} PARENT_SCOPE)

message(STATUS "[${TARGET_NAME}] Done..")
