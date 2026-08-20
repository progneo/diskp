#include <filesystem>
#include <iostream>
#include <map>
#include <vector>
#include <sys/resource.h>
#include <format>
#include <chrono>

#include "diskp/logger.hpp"
#include "diskp/scanner.hpp"

namespace {
    enum Command { Unknown, Dir, Help, Version };

    const std::map<std::string, Command> commandMap = {
        {"-d", Dir},
        {"--dir", Dir},
        {"-h", Help},
        {"--help", Help},
        {"-v", Version},
        {"--version", Version},
    };
}

static void print_memory_usage() {
    rusage usage{};
    getrusage(RUSAGE_SELF, &usage);

#ifdef __APPLE__
    long memory_bytes = usage.ru_maxrss;
#else
    long memory_bytes = usage.ru_maxrss * 1024;
#endif

    diskp::logger::debug(std::format("peak memory used - {} MB",
                                     static_cast<double>(memory_bytes) / (1024.0 * 1024.0)));
}

int main(const int argc, char *argv[]) {
    diskp::logger::set_min_level(diskp::log_level::DEBUG);

    const auto start = std::chrono::high_resolution_clock::now();

    const std::vector<std::string> args(argv + 1, argv + argc);
    std::string dir;

    for (std::size_t i = 0; i < args.size(); ++i) {
        const auto command =
                commandMap.contains(args[i])
                    ? commandMap.at(args[i])
                    : Unknown;

        switch (command) {
            case Dir:
                if (i + 1 >= args.size()) {
                    std::cerr << "[err]: no directory provided\n";
                    return 1;
                }

                dir = args[i + 1];
                ++i;
                break;

            case Version:
                std::cout << "DiskP Version: 1.0.0" << "\n";
                return 0;

            case Help:
                std::cout << "help will provided later" << "\n";
                return 0;

            default:
                std::cerr << std::format("[err]: unknown argument '{}'\n", args[i]);
                return 1;
        }
    }

    if (dir.empty()) {
        dir = std::filesystem::current_path().string();
    }

    const diskp::scan_result result = diskp::scanner::scan(dir);
    diskp::logger::info(std::format("scanning results:\n{}", result.get_result_message()));

    std::cout << "[err]: total errors: " << result.error_list.size() << "\n";
    for (const diskp::scan_error &error: result.error_list) {
        diskp::logger::error(error.get_message());
    }

    const auto end = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double, std::milli> duration = end - start;
    diskp::logger::info(std::format("execution time - {} ms", duration.count()));
    print_memory_usage();

    return 0;
}
