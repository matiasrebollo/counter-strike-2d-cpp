#include "server/glock.h"

Glock::Glock(const GunType& type, const int& rate_of_fire, const int& base_damage,
             const int& falloff_distance, const double& base_precision, uint16_t ammo):
        Gun(type, rate_of_fire, base_damage, falloff_distance, base_precision, ammo) {
    if (type != GLOCK) {
        throw std::runtime_error("Una glock debe tener tipo GLOCK");
    }
}
