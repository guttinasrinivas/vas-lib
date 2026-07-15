cmake_minimum_required(VERSION 3.12)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)


project(vas-lib VERSION 1.0.0 LANGUAGES C CXX)
link_directories(${HOME}/src/python/vproj/vas-lib/build)

# Force modern C++ standard requirements
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Locate and configure the system multi-threading library (pthread, etc.)
find_package(Threads REQUIRED)

# Add library target
add_library(c_queue_impl STATIC src/queue_impl.c)
add_library(wl_impl STATIC src/word_list.c)

# Add the executable target (assumes your source code file is named main.cpp)
#add_executable(c_queue_impl src/SimpleLogger.cpp src/thread_test.cpp)

foreach (PROJ_NAME IN ITEMS c_queue_impl wl_impl)
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
