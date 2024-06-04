cmake_minimum_required(VERSION 3.16)

project(
  SAT
  LANGUAGES CXX)

find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

set(SRC
        src/SAT.cpp
        src/Collider.cpp
        src/Drawing.cpp
)

add_executable(sat ${SRC})

target_precompile_headers(sat PRIVATE src/PCH.hpp)

target_link_libraries(sat PRIVATE SDL3::SDL3 SDL3_image::SDL3_image)
