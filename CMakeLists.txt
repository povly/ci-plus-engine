cmake_minimum_required(VERSION 3.16)
project(Povly LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ---------- Движок: SHARED-библиотека ----------
file(GLOB POVLY_SOURCES CONFIGURE_DEPENDS src/engine/*.cpp)
add_library(Povly SHARED ${POVLY_SOURCES})

# PUBLIC: путь нужен самому движку И утекает в Sandbox при линковкеd
target_include_directories(Povly PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

# PRIVATE: определение живёт ТОЛЬКО в компиляции движка
target_compile_definitions(Povly PRIVATE POVLY_BUILD_DLL=1)

# ---------- Песочница: приложение ----------
add_executable(Sandbox src/sandbox/SandboxApp.cpp)
target_link_libraries(Sandbox PRIVATE Povly)

# ---------- Общее для обоих таргетов ----------
foreach(T Povly Sandbox)
    target_compile_options(${T} PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(${T} PRIVATE $<$<CONFIG:Debug>:-fsanitize=address,undefined>)
    target_link_options(${T} PRIVATE $<$<CONFIG:Debug>:-fsanitize=address,undefined>)
endforeach()