#ifndef GLOCK_H
#define GLOCK_H

#include "server/gun.h"

class Glock: public Gun {

public:
    Glock(const GunType& type, const int& rate_of_fire, const int& base_damage,
          const int& falloff_distance, const double& base_precision, uint16_t ammo);

    Glock(const Glock&) = delete;
    Glock& operator=(const Glock&) = delete;

    virtual ~Glock() = default;
};


#endif
