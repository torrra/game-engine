# ~ 2024_moteur_gpm_2027_gpm_2027_projet_moteur-mustang/editor/CMakeLists.txt

get_filename_component(TARGET_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
message(STATUS "[${TARGET_NAME}] Starting source file fetching..")

# -- C++ version -- #
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# -- Header files -- #
file(GLOB_RECURSE TARGET_HEADER_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/*.inl)
	
# -- Source files -- #
file(GLOB_RECURSE TARGET_SOURCE_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/*.c
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

# -- Group header and source under same name -- #
set(TARGET_FILES ${TARGET_HEADER_FILES} ${TARGET_SOURCE_FILES})

# -- Create VS filter -- #
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${TARGET_FILES})

# -- Create target library -- #
add_executable(${TARGET_NAME})
message(STATUS "[${TARGET_NAME}] The executable is being linked with the project's dependencies..")

# -- Add sources to target -- #
target_sources(${TARGET_NAME} PRIVATE ${TARGET_FILES})

# -- Set public include directory for this and other projects -- #
target_include_directories(${TARGET_NAME}
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
	# PRIVATE ${ENGINE_INCLUDE_DIR}
)

target_link_libraries(${TARGET_NAME}
	PRIVATE ${ENGINE_LIB}
)

message(STATUS "[${TARGET_NAME}] Done..")