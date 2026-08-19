//
// Created by Yaroslav on 18.08.2026.
//

#ifndef DISKP_SCAN_RESULT_HPP
#define DISKP_SCAN_RESULT_HPP

#include <vector>

#include "scan_error.hpp"

namespace diskp {
    struct scan_result {
        std::uint64_t count_of_files;
        std::uint64_t count_of_directories;
        std::uint64_t count_of_symlinks;
        std::uint64_t total_apparent_size;
        std::uint64_t total_allocated_size;
        std::vector<scan_error> error_list;


        [[nodiscard]] std::string get_result_message() const {
            return std::format("count of directories - {}\n", this->count_of_directories) +
                   std::format("count of files - {}\n", this->count_of_files) +
                   std::format("count of symlinks - {}\n", this->count_of_symlinks) +
                   std::format("total allocated size - {}\n", this->total_allocated_size) +
                   std::format("total apparent size - {}\n", this->total_apparent_size);
        }
    };
}

#endif //DISKP_SCAN_RESULT_HPP
