#ifndef LOCAL_INFO_H
#define LOCAL_INFO_H

#include <string>

#include "../common/skins.h"

// quizas esto llamarlo game state y luego englobar las cosas de nuestro player en un
// localplayerinfo.
struct LocalInfo {
    std::string username;
    std::string gamename;
    CounterTerroristSkin ct_skin;
    TerroristSkin tt_skin;

    bool is_ct = false;
    int x = 0;
    int y = 0;
    double orientation = 270;
    int life = 0;
    int money = 0;
    int equipped_gun_ammo = 0;
    GunType primary_gun = NONE;
    GunType secondary_gun = NONE;
};

#endif
