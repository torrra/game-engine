# ~ 2024_moteur_gpm_2027_gpm_2027_projet_moteur-mustang/CMakeLists.txt

cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

project(${PROJECT_NAME})

add_subdirectory(librairies)
add_subdirectory(engine)

if (MSVC)
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT engine)
endif()