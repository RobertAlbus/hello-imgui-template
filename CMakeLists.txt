cmake_minimum_required(VERSION 3.12)
project(helloworld_with_helloimgui)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20 CACHE STRING "C++ standard to use")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_compile_options(-march=native -O3)
include(FetchContent)


# Build hello_imgui
# =================

set(GLFW_BUILD_TESTS OFF CACHE BOOL "Disable GLFW tests" FORCE)
FetchContent_Declare(
    glfw3
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.3.4
)
FetchContent_MakeAvailable(glfw3)
message(STATUS ${glfw3_BINARY_DIR})
set(glfw3_DIR ${glfw3_BINARY_DIR}/src)


if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/external/hello_imgui)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/hello_imgui)
endif()

FetchContent_Declare(
    portaudio
    GIT_REPOSITORY git@github.com:PortAudio/portaudio.git
    GIT_TAG        v19.7.0
)
FetchContent_MakeAvailable(portaudio)


# Build your app
# ==============
hello_imgui_add_app(hello_world_ hello_world.main.cpp)
target_link_libraries(hello_world_ PRIVATE portaudio)
target_include_directories(hello_world_ PRIVATE
    ${clover_SOURCE_DIR}/include
)


# hello_imgui_add_app is a helper function, similar to cmake's "add_executable"
# Usage:
# hello_imgui_add_app(app_name file1.cpp file2.cpp ...)
#
# Features: 
# * It will automatically link the target to the required libraries (hello_imgui, OpenGl, glad, etc)
# * It will embed the assets (for desktop, mobile, and emscripten apps)
# * It will perform additional customization (app icon and name on mobile platforms, etc)

# Now you can build your app with
#     mkdir build && cd build && cmake .. && cmake --build .
