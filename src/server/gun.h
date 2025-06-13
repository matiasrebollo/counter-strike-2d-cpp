#ifndef GUN_H
#define GUN_H

#include <memory>
#include <unordered_map>

#include "common/settings.h"
#include "server/weapon.h"

enum ShotOnActionWeapon { GUN_KNIFE, GUN_GLOCK, GUN_AWP, GUN_M3, NO };

struct GunStats {
    double precision_base;
    int fallof_distance;
    int damage;
    int rate_of_fire;
    bool damage_falls_with_distance;
    bool ignores_precision;  // por ejemplo, AWP no falla
};

static const std::unordered_map<ShotOnActionWeapon, GunStats> GUN_STATS = {
        {GUN_GLOCK, {GLOCK_PRECISION, GLOCK_FALLOF, GLOCK_DMG, GLOCK_ROF, true, false}},
        {GUN_AWP, {AWP_PRECISION, AWP_FALLOF, AWP_DMG, AWP_ROF, false, true}},
        {GUN_KNIFE, {KNIFE_PRECISION, KNIFE_FALLOF, KNIFE_DMG, KNIFE_ROF, false, false}},
        {GUN_M3, {AWP_PRECISION, M3_FALLOF, M3_DMG, M3_ROF, true, false}}};

class Gun: public Weapon {
private:
    uint16_t ammo;
    GunType type;
    ShotOnActionWeapon tipo;
    int rate_of_fire;
    float time_since_last_shot;

    bool can_shoot();
    void shoot(GameWorld& game, Player& owner);
    void execute_shoot(GameWorld& game, Player* shot_victim, double shot_distance);

public:
    explicit Gun(const GunType& gun_type);
    // static std::unique_ptr<Gun> new_gun(const GunType& type); -> uso para eventual jerarquia
    // polimorfica
    GunType get_type() const;
    uint16_t get_ammo() const;
    void add_ammo(uint16_t ammo_count);
    void update(const float& delta_t, Player& owner, GameWorld& game) override;

    // eliminar copia luego con polimorfismo
    Gun(const Gun&) = default;
    Gun& operator=(const Gun&) = default;

    virtual ~Gun() = default;
};


#endif
