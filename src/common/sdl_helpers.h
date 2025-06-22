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
    KNIFE_SHOP,
    BOMB_GAME
};

enum SoundEffect {
    DENY_SELECT,
    SELECT,
    MOVE_SELECT,
    OPEN_SHOP,
    CLOSE_SHOP,
    DIRT_STEP_ONE,
    DIRT_STEP_TWO,
    GLOCK_SHOT,
    AWP_SHOT,
    KNIFE_HIT,
    M3_SHOT,
    AK_SHOT,
    START_ROUND_TT,
    START_ROUND_CT,
    ROUND_ALMOST_FINISH,
    BOMB_DEFUSE,
    BOMB_PLANTED,
    BOMB_TICK,
    CT_WINS,
    TT_WINS,
    FAST_TICK_CLOCK,
    BOMB_EXPLOSION,
    BOMB_ACTION,
    DEATH_SOUND_ONE,
    DEATH_SOUND_TWO,
    DEATH_SOUND_THREE
};

enum Others { AWP_SHOT_FLARE, BLOOD, BLOOD_SCREEN };


#endif
