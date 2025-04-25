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

# -- Add sources to target -- #
target_sources(${TARGET_NAME} PRIVATE ${TARGET_FILES})

# -- Set public include directory for this and other projects -- #
target_include_directories(${TARGET_NAME}
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

if (MSVC)

set_target_properties(${TARGET_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/workspace)
endif()

message(STATUS "[${TARGET_NAME}] Done..")