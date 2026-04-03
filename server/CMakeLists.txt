cmake_minimum_required(VERSION 3.20)
project(RGAeditorServer VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
if(CLANG_TIDY_EXE)
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
endif()

find_package(nlohmann_json CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED) 
find_package(Boost REQUIRED) 

include_directories(include)

add_executable(RGAeditorServer 
    src/main.cpp
    src/RGA.cpp
    src/Room.cpp
    src/Session.cpp
    src/Listener.cpp  
)

target_link_libraries(RGAeditorServer PRIVATE nlohmann_json::nlohmann_json Boost::headers)

enable_testing()

add_executable(RGAeditorTests
    tests/RGAUnitTest.cpp      
    tests/JsonUnitTest.cpp     
    src/RGA.cpp             
)

target_link_libraries(RGAeditorTests PRIVATE 
    GTest::gtest 
    GTest::gtest_main 
    nlohmann_json::nlohmann_json
)

include(GoogleTest)
gtest_discover_tests(RGAeditorTests)

if(MSVC)
    target_compile_options(RGAeditorServer PRIVATE /W4 /WX)
    target_compile_options(RGAeditorTests  PRIVATE /W4 /WX)
else()
    target_compile_options(RGAeditorServer PRIVATE -Wall -Wextra -Wpedantic -Werror)
    target_compile_options(RGAeditorTests  PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()