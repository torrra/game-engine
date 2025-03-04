# ~ 2024_moteur_gpm_2027_gpm_2027_projet_moteur-mustang/CMakeLists.txt

cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

project(MustangEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

project(${PROJECT_NAME})

include(FetchContent)
FetchContent_Declare(
    mathlib
    GIT_REPOSITORY			   ssh://git@git.isartintra.com:2424/2023/Math_Lib_/Noah.git    # Fetch our math library from github
    GIT_TAG                    main										                    # Get main branch
    GIT_SHALLOW                ON                                                           # Allow branch names as GIT_TAG argument
)
FetchContent_MakeAvailable(mathlib)

add_subdirectory(dependencies)
add_subdirectory(editor)
add_subdirectory(engine)

if (MSVC)
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT editor)
endif()