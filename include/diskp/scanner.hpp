//
// Created by Yaroslav on 18.08.2026.
//

#ifndef DISKP_SCANNER_HPP
#define DISKP_SCANNER_HPP

#include <string>

#include "scan_result.hpp"

namespace diskp {
    class scanner {
    public:
        static scan_result scan(const std::string &dir);
    };
}

#endif //DISKP_SCANNER_HPP
