cmake_minimum_required(VERSION 3.12)
project(VisualCryptography CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OpenCV package
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS} include)

# Source files list
set(SOURCES
    src/main.cpp
    src/TaskFactory.cpp
    src/ClassicVCTask.cpp
    src/DualLayerVCTask.cpp
    src/ASCIIArtTask.cpp
    src/Utils.cpp
)

# Unified binary
add_executable(visual_crypto ${SOURCES})
target_link_libraries(visual_crypto ${OpenCV_LIBS})

# Enable testing frameworks
enable_testing()

# Compile and register the test runner
add_executable(visual_crypto_tests tests/main.cpp src/Utils.cpp)
target_link_libraries(visual_crypto_tests ${OpenCV_LIBS})
add_test(NAME run_tests COMMAND visual_crypto_tests)
