cmake_minimum_required(VERSION 3.17)
project(export)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_BUILD_TYPE Release)

if($<CONFIG:DEBUG>)
    add_compile_options(-g3 -Wall -Wextra -Og)
else()
    add_compile_options(-O3 -flto -Os)
endif()

include_directories(lib)

add_subdirectory(lib)
add_subdirectory(unitTest)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC lib)