add_executable(diskp_cli
        main.cpp
)

target_link_libraries(
        diskp_cli
        PRIVATE
        diskp_core
        diskp_utils
)