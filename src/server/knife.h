#ifndef KNIFE_H
#define KNIFE_H

#include <memory>

#include "common/settings.h"
#include "server/weapon.h"

class Knife: public Weapon {
private:
    float time_since_last_stab;

    bool can_stab();
    void stab(GameWorld& game);

public:
    Knife();

    void update(const float& delta_t, Player& owner, GameWorld& game) override;

    Knife(const Knife&) = delete;
    Knife& operator=(const Knife&) = delete;

    virtual ~Knife() = default;
};

#endif
