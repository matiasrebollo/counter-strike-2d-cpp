#ifndef GUN_H
#define GUN_H

#include <memory>

#include "common/settings.h"
#include "server/weapon.h"

enum ShotOnActionWeapon { GUN_KNIFE, GUN_GLOCK, GUN_AWP, GUN_M3, NO };

class Gun: public Weapon {
private:
    uint16_t ammo;
    GunType type;
    ShotOnActionWeapon tipo;
    int rate_of_fire;
    float time_since_last_shot;

    bool can_shoot();
    void shoot(GameWorld& game, Player& owner);

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
