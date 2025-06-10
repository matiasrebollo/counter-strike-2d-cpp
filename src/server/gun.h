#ifndef GUN_H
#define GUN_H

#include <memory>

#include "server/weapon.h"

#define GLOCK_INITIAL_AMMO 30
#define GLOCK_ROF 400
#define AWP_INITIAL_AMMO 10
#define M3_INITIAL_AMMO 20
#define AK47_INITIAL_AMMO 45

class Gun: public Weapon {
private:
    uint16_t ammo;
    GunType type;
    float time_since_last_shot;

    bool can_shoot();
    void shoot(GameWorld& game);

public:
    explicit Gun(const GunType& gun_type);
    // static std::unique_ptr<Gun> new_gun(const GunType& type); -> uso para eventual jerarquia
    // polimorfica
    GunType get_type() const;
    uint16_t get_ammo() const;
    void add_ammo(uint16_t ammo_count);
    void update(GameWorld& game, const float& delta_t) override;

    // eliminar copia luego con polimorfismo
    Gun(const Gun&) = default;
    Gun& operator=(const Gun&) = default;

    virtual ~Gun() = default;
};


#endif
