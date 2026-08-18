//
// Created by Yaroslav on 18.08.2026.
//

#ifndef DISKP_SCANNER_HPP
#define DISKP_SCANNER_HPP
#include <string>
#include <filesystem>
#include <sys/stat.h>

namespace diskp {
    class scanner {
    private:
        struct stat file_info{};
        std::filesystem::path directory;
        long count_of_files;
        long count_of_directories;
        long long total_apparent_size;
        long long total_allocated_size;

        void scan_directory(const std::filesystem::directory_entry& directory_entry);

    public:
        scanner();

        ~scanner();

        void start_scanning(const std::string &dir);

        void print_results() const;
    };
}


#endif //DISKP_SCANNER_HPP
