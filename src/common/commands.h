#ifndef COMMANDS_H
#define COMMANDS_H

#include <string>

enum CommandType {
    CREATE_USERNAME,
    CREATE_GAME,
    JOIN_GAME,
    SELECT_MAP,
    BUY_WEAPON,
    BUY_AMMO,
    ROTATE,
    MOVE,
    SHOOT,
    CHANGE_WEAPON,
    PLANT_BOMB,
    DEFUSE_BOMB,
    GAME_STARTED,
    SHOT,
    GAME_ENDED
};

#endif
