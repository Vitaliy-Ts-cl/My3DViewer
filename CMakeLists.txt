cmake_minimum_required(VERSION 3.20)

project(My3DViewer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(glm CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)

add_executable(My3DViewer main.cpp)
target_link_libraries(My3DViewer PRIVATE glm::glm-header-only glfw glad::glad)
