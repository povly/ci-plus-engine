cmake_minimum_required(VERSION 3.16)
project(engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Vulkan REQUIRED)
find_package(glfw3  REQUIRED)

file(GLOB sources CONFIGURE_DEPENDS src/*.cpp)
add_executable(engine ${sources})
target_link_libraries(engine PRIVATE glfw Vulkan::Vulkan)