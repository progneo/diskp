//
// Created by Yaroslav on 18.08.2026.
//
#include "diskp/scanner.hpp"

#include <filesystem>
#include <sys/stat.h>

#include "diskp/logger.hpp"

namespace diskp {
    void scanner::scan_directory(const std::filesystem::directory_entry &directory_entry, scan_result &result) {
        try {
            std::error_code error_code;
            std::filesystem::directory_options opts = std::filesystem::directory_options::skip_permission_denied;

            for (const auto &entry: std::filesystem::directory_iterator(directory_entry, opts, error_code)) {
                if (error_code) {
                    result.error_list.push_back({
                        .path = directory_entry.path().string(),
                        .action = "read",
                        .error_code = error_code
                    });
                    error_code.clear();
                    continue;
                }

                if (entry.is_symlink()) {
                    calculate_symlink_size(entry, result);
                } else if (entry.is_directory()) {
                    calculate_directory_size(entry, result);
                    scan_directory(entry, result);
                } else if (entry.is_regular_file()) {
                    calculate_file_size(entry, result);
                }
            }
        } catch (const std::filesystem::filesystem_error &e) {
            result.error_list.push_back({
                .path = directory_entry.path().string(),
                .action = "lstat",
                .error_code = e.code()
            });
        }
    }

    void scanner::calculate_directory_size(const std::filesystem::directory_entry &directory_entry,
                                           scan_result &result) {
        struct stat file_info{};
        result.count_of_directories += 1;

        if (lstat(directory_entry.path().c_str(), &file_info) != 0) {
            result.error_list.push_back({
                .path = directory_entry.path().string(),
                .action = "lstat"
            });
        } else {
            result.total_apparent_size += file_info.st_size;
            result.total_allocated_size += file_info.st_blocks * 512;
        }
    }

    void scanner::calculate_file_size(const std::filesystem::directory_entry &directory_entry, scan_result &result) {
        struct stat file_info{};
        result.count_of_files += 1;

        if (lstat(directory_entry.path().c_str(), &file_info) != 0) {
            result.error_list.push_back({
                .path = directory_entry.path().string(),
                .action = "lstat"
            });
        } else {
            result.total_apparent_size += file_info.st_size;
            result.total_allocated_size += file_info.st_blocks * 512;
        }
    }

    void scanner::calculate_symlink_size(const std::filesystem::directory_entry &directory_entry, scan_result &result) {
        struct stat file_info{};
        result.count_of_symlinks += 1;

        if (lstat(directory_entry.path().c_str(), &file_info) != 0) {
            result.error_list.push_back({
                .path = directory_entry.path().string(),
                .action = "lstat"
            });
        } else {
            result.total_apparent_size += file_info.st_size;
            result.total_allocated_size += file_info.st_blocks * 512;
        }
    }

    scan_result scanner::start_scanning(const std::string &dir) {
        scan_result result = {
            .count_of_files = 0,
            .count_of_directories = 0,
            .count_of_symlinks = 0,
            .total_apparent_size = 0,
            .total_allocated_size = 0
        };

        try {
            if (std::filesystem::exists(dir) && std::filesystem::is_directory(dir)) {
                std::error_code error_code;

                const std::filesystem::directory_entry directory_entry(dir, error_code);
                calculate_directory_size(directory_entry, result);

                if (!error_code) {
                    scan_directory(directory_entry, result);
                } else {
                    result.error_list.push_back({
                        .path = dir,
                        .action = "init",
                        .error_code = error_code
                    });
                }
            } else {
                logger::error("provided path not exists or not a valid directory");
            }
        } catch (const std::filesystem::filesystem_error &e) {
            logger::error(e.what());
        }

        return result;
    }
}
