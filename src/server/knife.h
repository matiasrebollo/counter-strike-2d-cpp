#ifndef knife_H
#define knife_H

#include <memory>
#include <unordered_map>

#include "server/weapon.h"

class Knife: public Weapon {
private:
    int damage;
    int attack_rate;
    float time_since_last_stab;

    bool can_stab();
    void stab(GameWorld& game, Player& owner);
    void execute_stab(Player* shot_victim, double shot_distance);

public:
    Knife();
    void update(const float& delta_t, Player& owner, GameWorld& game) override;

    Knife(const Knife&) = delete;
    Knife& operator=(const Knife&) = delete;

    virtual ~Knife() = default;
};


#endif
