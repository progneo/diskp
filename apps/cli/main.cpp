#include <iostream>
#include <map>
#include <vector>

#include "diskp/scanner.hpp"

namespace {
    enum Command { Unknown, Dir };

    std::map<std::string, Command> commandMap = {
        {"-d", Dir},
        {"--dir", Dir},
    };
}

int main(const int argc, char *argv[]) {
    const std::vector<std::string> args(argv + 1, argv + argc);
    std::string dir;

    if (args.empty()) {
        std::cerr << "[err]: no arguments provided" << std::endl;
        return 1;
    }

    if (args.at(0) == "-h" || args.at(0) == "--help") {
        std::cout << "[debug]: help will provided later" << std::endl;
        return 0;
    }

    if (args.at(0) == "-v" || args.at(0) == "--version") {
        std::cout << "DiskP Version: 1.0.0" << std::endl;
        return 0;
    }

    switch (commandMap.contains(args.at(0)) ? commandMap.at(args.at(0)) : Unknown) {
        case Dir:
            dir = args.at(1);
            break;
        default:
            std::cerr << "[err]: unknown command" << std::endl;
            break;
    }

    if (!dir.empty()) {
        diskp::scanner scanner;
        scanner.start_scanning(dir);
        scanner.print_results();
    }

    return 0;
}
