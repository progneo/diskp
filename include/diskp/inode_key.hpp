//
// Created by Yaroslav on 20.08.2026.
//

#ifndef DISKP_INODE_KEY_HPP
#define DISKP_INODE_KEY_HPP

#include <sys/stat.h>

namespace diskp {
    template<class T>
    void hash_combine(std::size_t &seed, const T &value) {
        seed ^= std::hash<T>{}(value)
                + 0x9e3779b9
                + (seed << 6)
                + (seed >> 2);
    }

    struct inode_key {
        dev_t device;
        ino_t inode;

        bool operator==(const inode_key &) const = default;
    };

    struct inode_key_hash {
        std::size_t operator()(const inode_key &key) const noexcept {
            std::size_t seed = 0;

            hash_combine(seed, key.device);
            hash_combine(seed, key.inode);

            return seed;
        }
    };
}

#endif //DISKP_INODE_KEY_HPP
