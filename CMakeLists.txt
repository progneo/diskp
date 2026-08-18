cmake_minimum_required(VERSION 4.3)

project(
        DiskP
        VERSION 1.0.0
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(src)
add_subdirectory(apps/cli)

include(CTest)

if (BUILD_TESTING)
    add_subdirectory(tests)
endif ()