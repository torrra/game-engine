# ~ 2024_moteur_gpm_2027_gpm_2027_projet_moteur-mustang/engine/CMakeLists.txt

# CMake Prototype
get_filename_component(TARGET_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

set(Green       "${Esc}[32m")
set(ColorReset "${Esc}[m")
message("[${TARGET_NAME}] Starting source file fetching..")

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

# -- Extra files -- #
file(GLOB TARGET_EXTRA_FILES
	${CMAKE_SOURCE_DIR}/.gitignore
	${CMAKE_SOURCE_DIR}/.gitattributes
	${CMAKE_SOURCE_DIR}/*.md
    ${CMAKE_SOURCE_DIR}/*.txt
)

file(COPY ${DLL_DEPENDENCIES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug/)
file(COPY ${DLL_DEPENDENCIES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release/)

set(TARGET_FILES ${TARGET_HEADER_FILES} ${TARGET_SOURCE_FILES})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${TARGET_FILES})
source_group("Extra files" FILES ${TARGET_EXTRA_FILES})

###### TODO : CHANGE TO LIBRARY IN THE FUTURE
add_executable(${TARGET_NAME})
message("[${TARGET_NAME}] The executable is being linked with the project's dependencies..")

target_sources(${TARGET_NAME} PRIVATE ${TARGET_FILES} ${TARGET_EXTRA_FILES})

target_include_directories(${TARGET_NAME}
	PRIVATE ${MATHLIB_INCLUDE_DIR} 
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set_target_properties(${DEPENDENCY_LIBRARY} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${EDITOR_LIBRARY} PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(${TARGET_NAME}
	PRIVATE ${DEPENDENCIES_LIBRARY}
	PRIVATE ${EDITOR_LIBRARY}
)

if (MSVC)
	# Disable linker warning for glfw3's missing debug info
	target_link_options(${TARGET_NAME} PRIVATE /IGNORE:4099)
	target_compile_options(${TARGET_NAME} PRIVATE /W4 /WX)
endif()

message("[${TARGET_NAME}] Done..")