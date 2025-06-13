#ifndef AK47_H
#define AK47_H

#include "server/gun.h"

class Ak_47: public Gun {
private:
    virtual bool can_shoot() const override;
    virtual void shoot(GameWorld& game, Player& owner) override;

public:
    Ak_47(const GunType& type, const int& rate_of_fire, const int& base_damage,
          const int& falloff_distance, const double& base_precision, uint16_t ammo);

    Ak_47(const Ak_47&) = delete;
    Ak_47& operator=(const Ak_47&) = delete;

    virtual ~Ak_47() = default;
};


#endif
