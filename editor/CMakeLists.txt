# ~ 2024_moteur_gpm_2027_gpm_2027_projet_moteur-mustang/editor/CMakeLists.txt

get_filename_component(TARGET_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
message("[${TARGET_NAME}] Starting source file fetching..")

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
add_library(${TARGET_NAME} STATIC)
message("[${TARGET_NAME}] The executable is being linked with the project's dependencies..")

# -- Add sources to target -- #
target_sources(${TARGET_NAME} PRIVATE ${TARGET_FILES})

# -- Set public include directory for this and other projects -- #
target_include_directories(${TARGET_NAME}
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
	
# -- Set name as exposed variable to build other projects -- #
set(EDITOR_LIBRARY ${TARGET_NAME} PARENT_SCOPE)
message("[${TARGET_NAME}] Done..")