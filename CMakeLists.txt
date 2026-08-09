cmake_minimum_required(VERSION 3.12)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_BUILD_TYPE Debug)

if(NOT DEFINED ENV{VAS_LIB_HOME})
    message(FATAL_ERROR "Please export VAS_LIB_HOME to the correct path")
endif()

project(vas-lib VERSION 1.0.0 LANGUAGES C CXX)
link_directories($ENV{VAS_LIB_HOME}/build)

# Force modern C++ standard requirements
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Locate and configure the system multi-threading library (pthread, etc.)
find_package(Threads REQUIRED)

# Add library target
add_library(c_queue_impl STATIC src/queue_impl.c)
target_include_directories(c_queue_impl PRIVATE src inc)

add_library(wl_impl STATIC src/hashed_list.c src/word_list.c)
target_include_directories(wl_impl PRIVATE src inc)

add_library(simple_logger STATIC src/SimpleLogger.cpp)
target_include_directories(simple_logger PRIVATE src inc)

add_executable(cli_args_test tests/cli_args_test.cpp)
if(APPLE)
    target_link_options(cli_args_test PRIVATE)
else()
target_link_options(cli_args_test PRIVATE -static)
endif()
target_include_directories(cli_args_test PRIVATE src inc)

# Add the executable target (assumes your source code file is named main.cpp)
#add_executable(c_queue_impl src/SimpleLogger.cpp src/thread_test.cpp)

foreach (PROJ_NAME IN ITEMS c_queue_impl wl_impl simple_logger)
    target_include_directories(${PROJ_NAME} PUBLIC inc)

    # Enable aggressive compiler warnings to keep the code tight and clean
    if(MSVC)
        target_compile_options(${PROJ_NAME} PRIVATE /W4)
    else()
        target_compile_options(${PROJ_NAME} PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endforeach()

# Link the threads library to the target
#target_link_libraries(c_queue_impl PRIVATE Threads::Threads)
