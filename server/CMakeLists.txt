cmake_minimum_required(VERSION 3.15)

project(boost_asio_test LANGUAGES CXX)

# --------------------------------------------------
# C++ Standard
# --------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --------------------------------------------------
# Compiler
# --------------------------------------------------
# DO NOT set gcc/g++ here.
# CMake will automatically use:
#   - MSVC on Visual Studio
#   - MinGW GCC on Windows/MSYS2
#   - Clang on macOS
#
# set(CMAKE_C_COMPILER "gcc")
# set(CMAKE_CXX_COMPILER "g++")

# --------------------------------------------------
# Boost
# --------------------------------------------------
# Only specify BOOST_ROOT on Windows/MSYS2.
if(WIN32 AND NOT MSVC)
    set(BOOST_ROOT "/mingw64")
endif()

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
# Platform-specific libraries
# --------------------------------------------------
if(WIN32)

    # Windows / MinGW
    target_link_libraries(server PRIVATE
        Boost::boost
        ws2_32
        mswsock
    )

elseif(APPLE)

    # macOS
    target_link_libraries(server PRIVATE
        Boost::boost
    )

elseif(UNIX)

    # Linux
    target_link_libraries(server PRIVATE
        Boost::boost
    )

endif()

# --------------------------------------------------
# Compiler warnings
# --------------------------------------------------
if(MSVC)

    target_compile_options(server PRIVATE
        /W4
    )

else()

    target_compile_options(server PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )

endif()