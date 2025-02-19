cmake_minimum_required(VERSION 3.11)

# CLI toolkit inspired by https://github.com/NobWow/admin-console-python, but written in C++
# Implements an (not yet) advanced, almost independent command system.

project(NNWCLI VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)

# Use relative paths in target_sources
cmake_policy(SET CMP0076 NEW)

# Do not use FindBoost module
cmake_policy(SET CMP0167 NEW)

# dependencies
# set(Boost_USE_STATIC_LIBS ON)
# set(Boost_USE_RELEASE_LIBS ON)
# find_package(Boost CONFIG COMPONENTS coroutine regex REQUIRED)

# the library
add_library(nnwcli STATIC)

# example executable
add_executable(nnwcli_example)

target_include_directories(nnwcli PUBLIC include)
# target_include_directories(nnwcli PUBLIC ${Boost_INCLUDE_DIR})
target_include_directories(nnwcli_example PUBLIC include)
# target_include_directories(nnwcli_example PUBLIC ${Boost_INCLUDE_DIR})

target_link_libraries(nnwcli_example PUBLIC nnwcli)

# sources for the targets
add_subdirectory(src)

# best optimization
if(CMAKE_BUILD_TYPE EQUAL Release)
    target_compile_options(nnwcli PRIVATE -O3)
    target_compile_options(nnwcli_example PRIVATE -O3)
endif()

# target_link_libraries(nnwcli PUBLIC ${Boost_LIBRARIES})
