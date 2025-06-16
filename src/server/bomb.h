#ifndef BOMB_H
#define BOMB_H

#include <memory>
#include <unordered_map>

#include "common/bomb_status.h"
#include "common/settings.h"
#include "server/weapon.h"


class Bomb: public Weapon {
private:
    BombStatus status;
    bool just_been_planted;
    float time_since_planted;

public:
    Bomb();

    BombStatus get_status();
    void restart();
    void defuse();
    bool just_planted();
    int detonation_time();
    void action() override;
    void stop_action() override;
    void update(const float& delta_t, Player& owner, GameWorld& game) override;
    void update_planted(const float& delta_t);

    Bomb(const Bomb&) = delete;
    Bomb& operator=(const Bomb&) = delete;

    virtual ~Bomb() = default;
};


#endif
