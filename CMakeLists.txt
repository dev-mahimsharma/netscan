cmake_minimum_required(VERSION 3.16)

project(NetLens VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Output executables to the root directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_library(netlens_core
    src/scanner.c
    src/ports.c
    src/worker_queue.c
)

target_include_directories(netlens_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(netlens_core PUBLIC Threads::Threads)

if(WIN32)
    target_link_libraries(netlens_core PUBLIC ws2_32)
endif()

add_executable(netlens src/main.c)
target_link_libraries(netlens PRIVATE netlens_core)

enable_testing()
add_executable(test_netlens tests/test_netlens.c)
target_link_libraries(test_netlens PRIVATE netlens_core)
add_test(NAME test_netlens COMMAND test_netlens)
