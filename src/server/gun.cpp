#include "server/gun.h"

#include <iostream>

#include "server/game_world.h"

Gun::Gun(const GunType& gun_type): type(gun_type), time_since_last_shot((60.0f / GLOCK_ROF)) {
    switch (type) {
        case GunType::GLOCK:
            ammo = GLOCK_INITIAL_AMMO;
            break;
        case GunType::AK47:
            ammo = AK47_INITIAL_AMMO;
            break;
        case GunType::AWP:
            ammo = AWP_INITIAL_AMMO;
            break;
        case GunType::M3:
            ammo = M3_INITIAL_AMMO;
            break;
        case GunType::NONE:
            ammo = 0;
            break;
        default:
            throw std::invalid_argument("not a gun type");
    }
}

/*std::unique_ptr<Gun> Gun::new_gun(const GunType& type) {
    switch (type) {
        case GunType::GLOCK:
            return std::make_unique<Glock>();
        case GunType::AK47:
            return std::make_unique<AK47>();
        case GunType::AWP:
            return std::make_unique<AWP>();
        case GunType::M3:
            return std::make_unique<M3>();
        default:
            throw std::invalid_argument("not a gun type");
    }
}*/

void Gun::add_ammo(uint16_t ammo_count) { ammo += ammo_count; }

bool Gun::can_shoot() { return ammo > 0 && time_since_last_shot >= (60.0f / GLOCK_ROF); }

void Gun::update(const float& delta_t, Player& owner, GameWorld& game) {
    if (just_triggered_action() && can_shoot()) {
        shoot(game, owner);
        time_since_last_shot = 0.0f;
    } else {
        time_since_last_shot += delta_t;
    }
    Weapon::update(delta_t, owner, game);
}

void Gun::shoot(GameWorld& game, Player& shooter) {
    std::cout << "disparo! " << std::endl;
    ammo -= 1;

    const Vector2D<int> origin(shooter.rect.position.x + shooter.rect.width / 2,
                               shooter.rect.position.y + shooter.rect.height / 2);
    Shot shot(origin, shooter.get_orientation());

    const Collidable* hit = shot.shoot(game, shooter);

    if (hit != nullptr) {
        // que hit reciba daño de shot, reemplazar prints
        Rect h = hit->rect;
        std::cout << "¡Impacto! Disparo acertó a objeto en (" << h.position.x << ", "
                  << h.position.y << ")\n";
    } else {
        std::cout << "Disparo fallido. No impactó ningún objeto.\n";
    }
}

uint16_t Gun::get_ammo() const { return ammo; }

GunType Gun::get_type() const { return type; }
