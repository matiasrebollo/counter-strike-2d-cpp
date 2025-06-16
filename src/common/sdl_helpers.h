#ifndef SDL_HELPERS_H
#define SDL_HELPERS_H


enum HudNumbers {
    ZERO = 0,
    ONE = 1,
    TWO = 2,
    THREE = 3,
    FOUR = 4,
    FIVE = 5,
    SIX = 6,
    SEVEN = 7,
    EIGHT = 8,
    NINE = 9,
    DP = 10
};

enum HudSymbols { HEALTH = 0, CLOCK = 2, SHOP = 4, BOMB_ACTIVE = 6, MONEY = 7 };

enum FontsAndBackground { BACKGROUND = 0, FONT_WAITING = 1, FONT_SHOP = 2 };

enum Crosshairs { GREEN = 0, RED = 1, YELLOW = 2, TIME = 3 };

enum GunSprites {
    AK47_GAME,
    AK47_SHOP,
    AWP_GAME,
    AWP_SHOP,
    M3_GAME,
    M3_SHOP,
    GLOCK_GAME,
    GLOCK_SHOP,
    KNIFE_GAME,
    KNIFE_SHOP
};

enum SoundEffect {
    DENY_SELECT,
    SELECT,
    MOVE_SELECT,
    OPEN_SHOP,
    CLOSE_SHOP,
    DIRT_STEP_ONE,
    DIRT_STEP_TWO,
    GLOCK_SHOT
};


#endif
