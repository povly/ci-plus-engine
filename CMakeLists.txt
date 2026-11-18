cmake_minimum_required(VERSION 3.16)
project(engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# find_package(Vulkan REQUIRED)
# find_package(glfw3  REQUIRED)


file(GLOB sources CONFIGURE_DEPENDS src/*.cpp)
add_executable(engine ${sources})

target_include_directories(engine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_compile_options(engine PRIVATE -Wall -Wextra -Wpedantic)
target_compile_options(engine PRIVATE $<$<CONFIG:Debug>:-fsanitize=address,undefined>)
target_link_options(engine PRIVATE $<$<CONFIG:Debug>:-fsanitize=address,undefined>)
target_compile_definitions(engine PRIVATE $<$<CONFIG:Release>:ENGINE_RELEASE=1>)
# target_link_libraries(engine PRIVATE glfw Vulkan::Vulkan)