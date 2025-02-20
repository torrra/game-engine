# ~ 2024_moteur_gpm_2027_gpm_2027_projet_moteur-mustang/engine/CMakeLists.txt

get_filename_component(TARGET_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

# -- Source files -- #
message("[${TARGET_NAME}] Starting source file fetching..")

# -- Header files -- #
file(GLOB_RECURSE TARGET_HEADER_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/*.inl)
	
# -- Source files -- #
file(GLOB_RECURSE TARGET_SOURCE_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/*.c
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

# -- Shader files -- #
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shader DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(GLOB_RECURSE TARGET_SHADER_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/shader/*.vert
	${CMAKE_CURRENT_SOURCE_DIR}/shader/*.frag
	${CMAKE_CURRENT_SOURCE_DIR}/shader/*.glsl)

# -- Asset files -- #
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/asset DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(GLOB_RECURSE TARGET_ASSET_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/asset/*.jpg
	${CMAKE_CURRENT_SOURCE_DIR}/asset/*.jpeg
	${CMAKE_CURRENT_SOURCE_DIR}/asset/*.png
	${CMAKE_CURRENT_SOURCE_DIR}/asset/*.obj
	${CMAKE_CURRENT_SOURCE_DIR}/asset/*.mtl
	${CMAKE_CURRENT_SOURCE_DIR}/asset/*.wav
	${CMAKE_CURRENT_SOURCE_DIR}/asset/*.mp3
	${CMAKE_CURRENT_SOURCE_DIR}/asset/*.mp4)

file(COPY ${DLL_DEPENDENCIES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug/)
file(COPY ${DLL_DEPENDENCIES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release/)

set(TARGET_FILES ${TARGET_HEADER_FILES} ${TARGET_SOURCE_FILES})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${TARGET_FILES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${TARGET_ASSET_FILES})

add_executable(${TARGET_NAME})
message("[${TARGET_NAME}] The executable is being linked with the project's dependencies..")

target_sources(${TARGET_NAME} PRIVATE ${TARGET_FILES} ${TARGET_ASSET_FILES})
set_source_files_properties(${TARGET_ASSET_FILES} PROPERTIES HEADER_FILE_ONLY TRUE)

target_include_directories(${TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/header)
target_link_libraries(${TARGET_NAME} PRIVATE ${ENGINE_LIBRARY})

message("[${TARGET_NAME}] Done.")