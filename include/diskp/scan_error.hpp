//
// Created by Yaroslav on 18.08.2026.
//

#ifndef DISKP_SCAN_ERROR_HPP
#define DISKP_SCAN_ERROR_HPP
#include <string>
#include <iostream>
#include <system_error>

namespace diskp {
    struct scan_error {
        std::string path;
        std::string action;
        std::error_code error_code;

        [[nodiscard]] std::string get_message() const {
            return "\npath: " + this->path +
                   "\naction: " + this->action +
                   "\nmessage: " + this->error_code.message();
        }
    };
}

#endif //DISKP_SCAN_ERROR_HPP
