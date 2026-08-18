//
// Created by Yaroslav on 18.08.2026.
//
#include <utility>

#include "diskp/scanner.hpp"

#include <iostream>
#include <ostream>
#include <filesystem>
#include <sys/stat.h>

namespace diskp {
    void scanner::scan_directory(const std::filesystem::directory_entry &directory_entry) {
        for (const auto &entry: std::filesystem::directory_iterator(directory_entry)) {
            if (entry.is_directory()) {
                std::cout << "[debug]: directory: " << entry.path() << std::endl;
                this->count_of_directories += 1;
                this->scan_directory(entry);
            } else if (entry.is_regular_file()) {
                std::cout << "[debug]: file: " << entry.path() << std::endl;
                this->count_of_files += 1;

                if (stat(entry.path().c_str(), &this->file_info) != 0) {
                    std::cerr << "[error]: " << entry.path() << std::endl;
                } else {
                    this->total_apparent_size += file_info.st_size;
                    this->total_allocated_size += file_info.st_blocks * 512;
                }
            }
        }
    }

    scanner::scanner() : count_of_files(0), count_of_directories(0), total_apparent_size(0), total_allocated_size(0) {
    }

    scanner::~scanner() = default;

    void scanner::start_scanning(const std::string &dir) {
        this->directory = dir;

        try {
            if (std::filesystem::exists(this->directory) && std::filesystem::is_directory(this->directory)) {
                const std::filesystem::directory_entry directoryEntry(this->directory);

                this->scan_directory(directoryEntry);

                std::cout << "[info]: starting scanning directory: " << dir << std::endl;
            } else {
                std::cerr << "[error]: provided path not exists or not a valid directory" << std::endl;
            }
        } catch (const std::filesystem::filesystem_error &e) {
            std::cerr << "[error]: " << e.what() << std::endl;
        }
    }

    void scanner::print_results() const {
        std::cout << "[info]: scanning results" << std::endl;
        std::cout << "[info]: count of directories - " << this->count_of_directories << std::endl;
        std::cout << "[info]: count of files - " << this->count_of_files << std::endl;
        std::cout << "[info]: total allocated size - " << this->total_allocated_size << std::endl;
        std::cout << "[info]: total apparent size - " << this->total_apparent_size << std::endl;
    }
}
