//
// Created by Yaroslav on 18.08.2026.
//
#include "diskp/scanner.hpp"

#include <iostream>
#include <queue>
#include <filesystem>
#include <unordered_set>
#include <sys/stat.h>

#include "diskp/inode_key.hpp"

namespace diskp {
    constexpr auto dir_opts = std::filesystem::directory_options::skip_permission_denied;
    constexpr std::uint64_t block_unit_bytes = 512;

    scan_result scanner::scan(const std::string &dir) {
        scan_result result = {
            .count_of_files = 0,
            .count_of_directories = 0,
            .count_of_symlinks = 0,
            .count_of_others = 0,
            .total_apparent_size = 0,
            .total_allocated_size = 0
        };

        std::queue<std::filesystem::path> queue;
        queue.emplace(dir);

        std::unordered_set<inode_key, inode_key_hash> seen_inodes;

        struct stat root_info{};
        if (lstat(dir.c_str(), &root_info) == 0) {
            if (S_ISDIR(root_info.st_mode)) {
                ++result.count_of_directories;
                result.total_allocated_size += static_cast<std::uint64_t>(root_info.st_blocks) * block_unit_bytes;
                result.total_apparent_size += static_cast<std::uint64_t>(root_info.st_size);
            } else if (S_ISLNK(root_info.st_mode)) {
                result.error_list.push_back({
                    .path = dir,
                    .action = "dir is symlink",
                    .error_code = std::error_code{}
                });
                return result;
            } else {
                result.error_list.push_back({
                    .path = dir,
                    .action = "dir is not directory",
                    .error_code = std::error_code{}
                });
                return result;
            }
        } else {
            const int err = errno;
            result.error_list.push_back({
                .path = dir,
                .action = "lstat",
                .error_code = std::error_code(err, std::generic_category())
            });
            return result;
        }

        while (!queue.empty()) {
            std::filesystem::path current_path = std::move(queue.front());
            queue.pop();

            std::error_code ec;

            std::filesystem::directory_iterator it(current_path, dir_opts, ec);
            const std::filesystem::directory_iterator end;

            if (ec) {
                result.error_list.push_back({
                    .path = current_path.string(),
                    .action = "iterate",
                    .error_code = ec
                });
                continue;
            }

            while (it != end) {
                const auto &entry = *it;
                bool should_count = false;

                struct stat file_info{};

                if (lstat(entry.path().c_str(), &file_info) == 0) {
                    should_count = true;
                    if (S_ISLNK(file_info.st_mode)) {
                        ++result.count_of_symlinks;
                    } else if (S_ISDIR(file_info.st_mode)) {
                        ++result.count_of_directories;
                        queue.emplace(entry.path());
                    } else if (S_ISREG(file_info.st_mode)) {
                        ++result.count_of_files;

                        inode_key key{
                            .device = file_info.st_dev,
                            .inode = file_info.st_ino
                        };
                        if (!seen_inodes.insert(key).second) {
                            should_count = false;
                        }
                    } else {
                        ++result.count_of_others;
                    }
                } else {
                    const int err = errno;
                    result.error_list.push_back({
                        .path = entry.path().string(),
                        .action = "lstat",
                        .error_code = std::error_code(err, std::generic_category())
                    });
                }

                if (should_count) {
                    result.total_allocated_size += static_cast<std::uint64_t>(file_info.st_blocks) * block_unit_bytes;
                    result.total_apparent_size += static_cast<std::uint64_t>(file_info.st_size);
                }

                it.increment(ec);

                if (ec) {
                    result.error_list.push_back({
                        .path = current_path.string(),
                        .action = "increment",
                        .error_code = ec
                    });

                    ec.clear();
                    break;
                }
            }
        }

        return result;
    }
}
