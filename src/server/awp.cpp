#include "server/awp.h"

Awp::Awp(const GunType& type, const int& rate_of_fire, const int& base_damage,
         const int& falloff_distance, const double& base_precision, uint16_t ammo):
        Gun(type, rate_of_fire, base_damage, falloff_distance, base_precision, ammo) {
    if (type != AWP) {
        throw std::runtime_error("Un awp debe tener tipo AWP");
    }
}

double Awp::calculate_falloff(const double&) const { return 0.0; }

double Awp::calculate_precision(const double&) const { return base_precision; }

int Awp::calculate_damage(const double&) const { return base_damage; }
