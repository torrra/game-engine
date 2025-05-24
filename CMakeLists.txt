# ~ 2024_moteur_gpm_2027_gpm_2027_projet_moteur-mustang/CMakeLists.txt

cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

project(MustangEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

include(FetchContent)
FetchContent_Declare(
    mathlib
    GIT_REPOSITORY			   ssh://git@git.isartintra.com:2424/2023/Math_Lib_/Noah.git    # Fetch our math library from github
    GIT_TAG                    master										                # Get master branch
    GIT_SHALLOW                ON                                                           # Allow branch names as GIT_TAG argument
)
FetchContent_MakeAvailable(mathlib)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(dependencies)
add_subdirectory(engine)
add_subdirectory(editor)
add_subdirectory(defaultExecutable)

if (MSVC)
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT editor)
endif()