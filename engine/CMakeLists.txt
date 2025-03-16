# ~ 2024_moteur_gpm_2027_gpm_2027_projet_moteur-mustang/engine/CMakeLists.txt

# CMake Prototype
get_filename_component(TARGET_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

message(STATUS "[${TARGET_NAME}] Starting source file fetching..")

set(ENGINE_SHADER_DIR ${CMAKE_BINARY_DIR}/engineShader)

# Create directory for engine shader files if it does not exist
if (NOT EXISTS ${ENGINE_SHADER_DIR})
	message(STATUS "[${TARGET_NAME}] Creating engine shader folder..")
	file(MAKE_DIRECTORY ${ENGINE_SHADER_DIR})
endif()

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
	${CMAKE_BINARY_DIR}/scripts/*.lua
)

file(COPY ${DLL_DEPENDENCIES} DESTINATION ${CMAKE_BINARY_DIR}/bin/Debug)
file(COPY ${DLL_DEPENDENCIES} DESTINATION ${CMAKE_BINARY_DIR}/bin/Release)

set(TARGET_FILES ${TARGET_HEADER_FILES} ${TARGET_SOURCE_FILES})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${TARGET_FILES})
source_group("shaders" FILES ${TARGET_ENGINE_SHADER_FILES})
source_group("Extra files" FILES ${TARGET_EXTRA_FILES})
source_group("Engine scripts" FILES ${TARGET_SCRIPT_FILES})

add_library(${TARGET_NAME} SHARED)

target_sources(${TARGET_NAME} PRIVATE ${TARGET_FILES} ${TARGET_EXTRA_FILES} ${TARGET_ENGINE_SHADER_FILES})

target_include_directories(${TARGET_NAME}
	PUBLIC ${MATHLIB_INCLUDE_DIR} 
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/engine
)

set_target_properties(${DEPENDENCY_LIBRARY} PROPERTIES LINKER_LANGUAGE CXX)
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
endif()

set(ENGINE_LIB ${TARGET_NAME} PARENT_SCOPE)
set(ENGINE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include PARENT_SCOPE)

message(STATUS "[${TARGET_NAME}] Done..")
