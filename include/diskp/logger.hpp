//
// Created by Yaroslav on 18.08.2026.
//

#ifndef DISKP_LOGGING_HPP
#define DISKP_LOGGING_HPP
#include <string>

namespace diskp {
    enum class log_level {
        DISABLED = 0,
        ERROR = 1,
        WARNING = 2,
        INFO = 3,
        DEBUG = 4
    };

    class logger {
    private:
        inline static log_level min_level = log_level::INFO;

    public:
        static void set_min_level(log_level level);

        static void debug(const std::string &message);

        static void info(const std::string &message);

        static void warning(const std::string &message);

        static void error(const std::string &message);
    };
}

#endif //DISKP_LOGGING_HPP
