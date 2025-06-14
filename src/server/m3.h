#ifndef M3_H
#define M3_H

#include "server/gun.h"

class M_3: public Gun {
private:
    virtual void shoot(GameWorld& game, Player& owner) override;

public:
    M_3(const GunType& type, const int& rate_of_fire, const int& base_damage,
        const int& falloff_distance, const double& base_precision, uint16_t ammo,
        const int& kill_bonification);

    M_3(const M_3&) = delete;
    M_3& operator=(const M_3&) = delete;

    virtual ~M_3() = default;
};


#endif
