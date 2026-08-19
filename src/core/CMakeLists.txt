add_library(diskp_core
        scanner.cpp
)

target_include_directories(
        diskp_core
        PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_compile_features(
        diskp_core
        PUBLIC
        cxx_std_26
)