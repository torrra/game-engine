# ~ 2024_moteur_gpm_2027_gpm_2027_projet_moteur-mustang/librairies/CMakeLists.txt

get_filename_component(TARGET_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

message("[${TARGET_NAME}] Starting source file fetching..")

# -- Header files -- #
file(GLOB_RECURSE TARGET_HEADER_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/dependencies/header/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/dependencies/header/*.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/dependencies/header/*.inl)

# -- Source files -- #
file(GLOB_RECURSE TARGET_SOURCE_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/dependencies/source/*.c
	${CMAKE_CURRENT_SOURCE_DIR}/dependencies/source/*.cpp)
	
# -- Lib files -- #
file(GLOB_RECURSE TARGET_LIB_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/dependencies/lib/*.lib)

# -- Dll files -- #
file(GLOB_RECURSE TARGET_DLL_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/dependencies/dll/*.dll)
set(DLL_DEPENDENCIES ${TARGET_DLL_FILES} PARENT_SCOPE)

# -- Set a common name for header and source files -- #
set(TARGET_FILES ${TARGET_HEADER_FILES} ${TARGET_SOURCE_FILES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${TARGET_FILES})

# set(PHYSX_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/header/physx)

add_library(${TARGET_NAME} STATIC)
message("[${TARGET_NAME}] The executable is being linked with the project's dependencies..")

target_sources(${TARGET_NAME} PRIVATE ${TARGET_FILES})

target_link_libraries(${TARGET_NAME} PRIVATE ${TARGET_LIB_FILES})

# file(GLOB TARGET_DIRECTORIES LIST_DIRECTORIES true
	# ${CMAKE_CURRENT_SOURCE_DIR}/*)

target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/header)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/header/imgui)


# foreach(DIRECTORY ${TARGET_DIRECTORIES})
	# # if(IS_DIRECTORY ${PHYSX_DIR})
		# # target_include_directories(${TARGET_NAME} PUBLIC ${PHYSX_DIR})
		# # message("[${TARGET_NAME}] physx include directory")
	# # endif()
	# if(IS_DIRECTORY ${DIRECTORY})
		# target_include_directories(${TARGET_NAME} PUBLIC ${DIRECTORY}/header)
		# message("[${TARGET_NAME}] other include directories")
	# else()
		# continue()
	# endif()
# endforeach()

set(ENGINE_LIBRARY ${TARGET_NAME} PARENT_SCOPE)
message("[${TARGET_NAME}] Done.")