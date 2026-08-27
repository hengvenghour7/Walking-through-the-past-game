cmake_minimum_required(VERSION 3.15)

project(boost_asio_test LANGUAGES CXX)

# --------------------------------------------------
# C++ Standard
# --------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --------------------------------------------------
# MinGW / MSYS2
# --------------------------------------------------
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

# --------------------------------------------------
# Boost
# --------------------------------------------------
set(BOOST_ROOT "/mingw64")

find_package(Boost REQUIRED)

# --------------------------------------------------
# Automatically find ALL .cpp files
# --------------------------------------------------
file(GLOB_RECURSE SOURCES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

# --------------------------------------------------
# Create server executable
# --------------------------------------------------
add_executable(server ${SOURCES})

# --------------------------------------------------
# Include directories
# --------------------------------------------------
target_include_directories(server PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    ${Boost_INCLUDE_DIRS}
)

# --------------------------------------------------
# Libraries
# --------------------------------------------------
target_link_libraries(server PRIVATE
    Boost::boost
    ws2_32
    mswsock
)

# --------------------------------------------------
# Compiler warnings
# --------------------------------------------------
if(MSVC)
    target_compile_options(server PRIVATE /W4)
else()
    target_compile_options(server PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()