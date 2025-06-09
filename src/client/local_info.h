#ifndef LOCAL_INFO_H
#define LOCAL_INFO_H

#include <string>

#include "../common/skins.h"

struct LocalInfo {
    std::string username;
    std::string gamename;
    CounterTerroristSkin ct_skin;
    TerroristSkin tt_skin;

    bool is_ct = false;
    int x = 0;
    int y = 0;
    int life = 0;
};

#endif
