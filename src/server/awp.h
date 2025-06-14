#ifndef AWP_H
#define AWP_H

#include "server/gun.h"

class Awp: public Gun {
private:
    virtual double calculate_falloff(const double& shot_distance) const override;
    virtual double calculate_precision(const double& falloff) const override;
    virtual int calculate_damage(const double& falloff) const override;

public:
    Awp(const GunType& type, const int& rate_of_fire, const int& base_damage,
        const int& falloff_distance, const double& base_precision, uint16_t ammo,
        const int& kill_bonification);

    Awp(const Awp&) = delete;
    Awp& operator=(const Awp&) = delete;

    virtual ~Awp() = default;
};


#endif
