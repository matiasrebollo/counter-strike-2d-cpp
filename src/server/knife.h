#ifndef KNIFE_H
#define KNIFE_H

#include <memory>

#include "server/weapon.h"

#define KNIFE_ROF 60

class Knife: public Weapon {
private:
    float time_since_last_stab;

    bool can_stab();
    void stab(GameWorld& game);

public:
    Knife();

    void update(GameWorld& game, const float& delta_t) override;

    Knife(const Knife&) = delete;
    Knife& operator=(const Knife&) = delete;

    virtual ~Knife() = default;
};

#endif
