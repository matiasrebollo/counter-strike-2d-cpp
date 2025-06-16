#ifndef BOMB_H
#define BOMB_H

#include <memory>
#include <unordered_map>

#include "common/bomb_status.h"
#include "server/weapon.h"

#define DETONATION_TIME 60
#define PLANTATION_TIME 5
#define DEFUSE_TIME 5

class Bomb: public Weapon {
private:
    BombStatus status;
    bool just_been_planted;
    float time_since_planted;

public:
    Bomb();

    BombStatus get_status();
    void restart();
    bool just_planted();
    int detonation_time();
    void update(const float& delta_t, Player& owner, GameWorld& game) override;
    void update_planted(const float& delta_t);

    Bomb(const Bomb&) = delete;
    Bomb& operator=(const Bomb&) = delete;

    virtual ~Bomb() = default;
};


#endif
