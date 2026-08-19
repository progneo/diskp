#include <iostream>
#include <map>
#include <vector>

#include "diskp/logger.hpp"
#include "diskp/scanner.hpp"

namespace {
    enum Command { Unknown, Dir };

    std::map<std::string, Command> commandMap = {
        {"-d", Dir},
        {"--dir", Dir},
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

    if (args.empty()) {
        std::cerr << "[err]: no arguments provided" << "\n";
        return 1;
    }

    if (args.at(0) == "-h" || args.at(0) == "--help") {
        std::cout << "[debug]: help will provided later" << "\n";
        return 0;
    }

    if (args.at(0) == "-v" || args.at(0) == "--version") {
        std::cout << "DiskP Version: 1.0.0" << "\n";
        return 0;
    }

    switch (commandMap.contains(args.at(0)) ? commandMap.at(args.at(0)) : Unknown) {
        case Dir:
            if (!args.at(1).empty()) {
                dir = args.at(1);
            } else {
                std::cerr << "[err]: no directory provided" << "\n";
                return 1;
            }
            break;
        default:
            std::cerr << "[err]: unknown command" << "\n";
            break;
    }

    if (dir.empty()) {
        dir = std::filesystem::current_path().string();
    }

    const diskp::scan_result result = diskp::scanner::start_scanning(dir);
    diskp::logger::info(std::format("[info]: scanning results:\n{}", result.get_result_message()));

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
