#ifndef GUN_H
#define GUN_H

#include <cstdint>
#include <memory>
#include <unordered_map>

#include "common/settings.h"
#include "server/weapon.h"

class Gun: public Weapon {
private:
    const GunType type;

protected:
    const int rate_of_fire;
    const int base_damage;
    const int falloff_distance;
    const double base_precision;
    float time_since_last_shot;
    uint16_t ammo;

    Gun(const GunType& type, const int& rate_of_fire, const int& base_damage,
        const int& falloff_distance, const double& base_precision, uint16_t ammo);
    virtual bool can_shoot() const;
    virtual void shoot(GameWorld& game, Player& owner);
    virtual double calculate_falloff(const double& shot_distance) const;
    virtual double calculate_precision(const double& falloff) const;
    virtual int calculate_damage(const double& falloff) const;
    double random_double(double min = 0.0, double max = 1.0) const;
    bool should_impact(const double& falloff) const;
    void execute_shot(Player* shot_victim, const double& shot_distance);

public:
    static std::unique_ptr<Gun> new_gun(const GunType& type);
    GunType get_type() const;
    uint16_t get_ammo() const;
    void add_ammo(uint16_t ammo_count);
    void update(const float& delta_t, Player& owner, GameWorld& game) override;

    Gun(const Gun&) = delete;
    Gun& operator=(const Gun&) = delete;

    virtual ~Gun() = default;
};


#endif
