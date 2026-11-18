cmake_minimum_required(VERSION 3.10)

project (HelloWorld)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -std=c++23")
set (source_dir "${PROJECT_SOURCE_DIR}/src/")

add_executable (HelloWorld 
  "${source_dir}main.cpp"
)