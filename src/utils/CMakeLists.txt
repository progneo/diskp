add_library(diskp_utils
        logger.cpp
)

target_include_directories(
        diskp_utils
        PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_compile_features(
        diskp_utils
        PUBLIC
        cxx_std_26
)