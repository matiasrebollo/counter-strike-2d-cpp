#ifndef GUN_H
#define GUN_H

#include <memory>

#include "server/weapon.h"

#define GLOCK_INITIAL_AMMO 30

class Gun: public Weapon {
protected:
    uint16_t ammo;
    bool is_trigger_pressed;

public:
    explicit Gun(int initial_ammo);
    static std::unique_ptr<Gun> new_gun(const GunType& type);
    virtual GunType type() const = 0;
    uint16_t get_ammo() const { return ammo; }
    void add_ammo(uint16_t ammo_count);
    void action() override;

    virtual void shoot() = 0;

    Gun(const Gun&) = delete;
    Gun& operator=(const Gun&) = delete;

    virtual ~Gun() = default;
};

class Glock: public Gun {
public:
    Glock();
    void shoot() override;
    GunType type() const override;

    ~Glock() override = default;
};

// el resto de armas...


#endif
