# ~ 2024_moteur_gpm_2027_gpm_2027_projet_moteur-mustang/engine/CMakeLists.txt

# CMake Prototype
get_filename_component(TARGET_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

message(STATUS "[${TARGET_NAME}] Starting source file fetching..")

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

file(GLOB_RECURSE TARGET_SCRIPT_FILES
	${CMAKE_BINARY_DIR}/scripts/*.lua
)

file(COPY ${DLL_DEPENDENCIES} DESTINATION ${CMAKE_BINARY_DIR}/bin/Debug)
file(COPY ${DLL_DEPENDENCIES} DESTINATION ${CMAKE_BINARY_DIR}/bin/Release)

set(TARGET_FILES ${TARGET_HEADER_FILES} ${TARGET_SOURCE_FILES})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${TARGET_FILES})
source_group("Extra files" FILES ${TARGET_EXTRA_FILES})
source_group("Engine scripts" FILES ${TARGET_SCRIPT_FILES})

add_library(${TARGET_NAME} SHARED)

target_sources(${TARGET_NAME} PRIVATE ${TARGET_FILES} ${TARGET_EXTRA_FILES} ${TARGET_SCRIPT_FILES})

MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
      LIST(APPEND dirlist ${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()

SUBDIRLIST(SUB_DIRS ${CMAKE_CURRENT_SOURCE_DIR})

foreach(DIR ${SUB_DIRS})
	target_include_directories(${TARGET_NAME} 
	PRIVATE ${DIR}
	)
endforeach()

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
	target_link_options(${TARGET_NAME} PRIVATE /IGNORE:4099)
	target_compile_options(${TARGET_NAME} PRIVATE /W4 /WX)
endif()

set(ENGINE_LIB ${TARGET_NAME} PARENT_SCOPE)
set(ENGINE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include PARENT_SCOPE)

message(STATUS "[${TARGET_NAME}] Done..")