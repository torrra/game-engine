# root CMakeLists.txt

cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

project(MustangEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

include(FetchContent)
set(FETCHED_PACKAGE_DIR ${CMAKE_BINARY_DIR}/_deps)
file(MAKE_DIRECTORY ${FETCHED_PACKAGE_DIR})

# Add each keyword separatly to avoid issues with quotation marks
set(GIT_DIR_COMMAND git)
list(APPEND GIT_DIR_COMMAND config --global --add safe.directory)

# Add a directory to the global safe directory list
# This sucks because it WILL add duplicates
# and add a new entry every time this is called 
macro(SetSafeDirectory DIR)

    set(TEMP_GIT_CMD ${GIT_DIR_COMMAND})
    list(APPEND TEMP_GIT_CMD ${DIR})
    execute_process(COMMAND ${TEMP_GIT_CMD})
    unset(TEMP_GIT_CMD)
    
endmacro()

function(FetchPackage PACKAGE_NAME REPO_LINK TAG)

    FetchContent_Declare(
    ${PACKAGE_NAME}
    GIT_REPOSITORY               ${REPO_LINK}    # Fetch repo
    GIT_TAG                    ${TAG}           # Get provided tag
    GIT_SHALLOW                ON              # Allow branch names as GIT_TAG argument
    )

    set(TEMP_SRC_DIR ${FETCHED_PACKAGE_DIR}/${PACKAGE_NAME}-src)
    set(${PACKAGE_NAME}_SOURCE_DIR ${TEMP_SRC_DIR})

    SetSafeDirectory(${TEMP_SRC_DIR})

    FetchContent_MakeAvailable(${PACKAGE_NAME})
    unset(TEMP_SRC_DIR)
    
endfunction()

# Mark this repository as safe
SetSafeDirectory(${CMAKE_SOURCE_DIR})
SetSafeDirectory(${CMAKE_SOURCE_DIR}/.git)

# Fetch math library
FetchPackage(mathlib git@github.com:torrra/mathlib.git main)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(dependencies)
add_subdirectory(engine)
add_subdirectory(editor)
add_subdirectory(defaultExecutable)

if (MSVC)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT editor)
endif()