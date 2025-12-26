cmake_minimum_required(VERSION 3.20)

project(RGAeditorServer VERSION 0.1.0 LANGUAGES CXX)

#Configuring modern c++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#Clang-Tidy integration
find_program(CLANG_TIDY_EXE NAMES "clang-tidy")

if(CLANG_TIDY_EXE)
    message(STATUS "Clang-Tidy was found: ${CLANG_TIDY_EXE}")
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
else()
    message(WARNING "Clang-Tidy was not found! The static analysis will not be performed.")
endif()

#Include folders
include_directories(include)

#Executable
add_executable(RGAeditorServer 
    src/main.cpp
)

#Compilation flags
if(MSVC)
    target_compile_options(RGAeditorServer PRIVATE /W4 /WX)
else()
    target_compile_options(RGAeditorServer PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()