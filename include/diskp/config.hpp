//
// Created by Yaroslav on 18.08.2026.
//

#ifndef DISKP_CONFIG_H
#define DISKP_CONFIG_H

namespace diskp {
    class config {
    public:
        config();

        ~config();

        config load_config();

        void save_config(config c);
    };
}

#endif //DISKP_CONFIG_H
