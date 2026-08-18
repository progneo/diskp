//
// Created by Yaroslav on 18.08.2026.
//
#include "diskp/logger.hpp"

#include <iostream>
#include <ostream>

namespace diskp {
    void logger::set_min_level(log_level level) {
        min_level = level;
    }

    void logger::debug(const std::string &message) {
        if (min_level >= log_level::DEBUG) {
            std::cout << "[debug]: " << message << "\n";
        }
    }

    void logger::info(const std::string &message) {
        if (min_level >= log_level::INFO) {
            std::cout << "[info]: " << message << "\n";
        }
    }

    void logger::warning(const std::string &message) {
        if (min_level >= log_level::WARNING) {
            std::cout << "[warning]: " << message << "\n";
        }
    }

    void logger::error(const std::string &message) {
        if (min_level >= log_level::ERROR) {
            std::cout << "[error]: " << message << "\n";
        }
    }
}
