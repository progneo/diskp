//
// Created by Yaroslav on 18.08.2026.
//

#ifndef DISKP_SCANNER_HPP
#define DISKP_SCANNER_HPP
#include <filesystem>

#include "scan_result.hpp"

namespace diskp {
    class scanner {
    private:
        static void scan_directory(const std::filesystem::directory_entry &directory_entry, scan_result &result);
        static void calculate_directory_size(const std::filesystem::directory_entry &directory_entry, scan_result &result);
        static void calculate_file_size(const std::filesystem::directory_entry &directory_entry, scan_result &result);
        static void calculate_symlink_size(const std::filesystem::directory_entry &directory_entry, scan_result &result);
    public:
        static scan_result start_scanning(const std::string &dir);
    };
}


#endif //DISKP_SCANNER_HPP
