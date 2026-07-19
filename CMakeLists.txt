cmake_minimum_required(VERSION 3.15)
project(space-shooter VERSION 0.1.0 LANGUAGES C CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Point to SFML cmake configs in UCRT64
list(APPEND CMAKE_PREFIX_PATH "C:/msys64/ucrt64")
set(SFML_DIR "C:/msys64/ucrt64/lib/cmake/SFML")

# Find SFML 3.0 (MinGW)
find_package(SFML 3.0 COMPONENTS Graphics Window System Audio Network REQUIRED)

add_executable(space-shooter
    src/main.cpp
    src/Game.cpp
    src/systems/PlayerMovementSystem.cpp
)

# Add include directories
target_include_directories(space-shooter PRIVATE src)

# Link SFML libraries
target_link_libraries(space-shooter PRIVATE 
    SFML::Graphics 
    SFML::Window 
    SFML::System 
    SFML::Audio 
    SFML::Network
)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
