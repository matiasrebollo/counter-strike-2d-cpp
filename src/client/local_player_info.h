#ifndef LOCAL_PLAYER_INFO_H
#define LOCAL_PLAYER_INFO_H

#include <string>

#include "../common/skins.h"

struct LocalPlayerInfo {
    std::string username;
    std::string gamename;
    CounterTerroristSkin ct_skin;
    TerroristSkin tt_skin;
};

#endif  // LOCAL_PLAYER_INFO_H
