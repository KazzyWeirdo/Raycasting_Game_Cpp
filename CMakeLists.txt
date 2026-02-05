cmake_minimum_required(VERSION 3.10)

project(RaycasterPortfolio VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Compiler Warnings
add_compile_options(-Wall -Wextra)

# Fetch SFML and ImGui-SFML using FetchContent
include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.1
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

# ImGui-SFML depends on ImGui, so we need to set the IMGUI_DIR variable before fetching it
FetchContent_Declare(ImGui
    GIT_REPOSITORY https://github.com/ocornut/imgui
    GIT_TAG v1.91.1
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(ImGui)
FetchContent_GetProperties(ImGui SOURCE_DIR IMGUI_DIR)
set(IMGUI_SFML_FIND_SFML OFF)

# ImGui-SFML depends on ImGui, so we need to set the IMGUI_DIR variable before fetching it
FetchContent_Declare(ImGui-SFML
    GIT_REPOSITORY https://github.com/SFML/imgui-sfml
    GIT_TAG v3.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(ImGui-SFML)

# Grab all source files
file(GLOB SOURCES "src/*.cpp")

add_executable(Game main.cpp ${SOURCES})

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set_target_properties(Game PROPERTIES WIN32_EXECUTABLE ON)
endif()

# Copy assets to the output directory after building
# This ensures that the assets are available in the same directory as the executable, which is necessary for the game to run correctly.
if(EXISTS "${CMAKE_SOURCE_DIR}/assets")
    add_custom_command(TARGET Game POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets" "$<TARGET_FILE_DIR:Game>/assets"
    )
endif()

target_compile_features(Game PRIVATE cxx_std_17)
target_link_libraries(Game PRIVATE SFML::Graphics ImGui-SFML::ImGui-SFML)