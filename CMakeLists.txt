cmake_minimum_required(VERSION 3.13)
project(realtime_multisensor_fusion)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    src/main.cpp
    src/thread_safe_queue.cpp
    src/sensor_simulator.cpp
    src/sliding_window.cpp
    src/fusion1.cpp
    src/fusion2.cpp
    src/aggregator.cpp
)

add_executable(fusion_pipeline ${SOURCES})

target_include_directories(fusion_pipeline PRIVATE include)

# Link pthread (required for std::thread on Linux/macOS)
find_package(Threads REQUIRED)
target_link_libraries(fusion_pipeline Threads::Threads)

# Static linking: bundle all runtime DLLs into the binary.
# Prevents DLL version conflicts when running from Git Bash or other environments.
if (MINGW)
    target_link_options(fusion_pipeline PRIVATE -static)
endif()
