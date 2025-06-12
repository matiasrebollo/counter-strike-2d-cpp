#include "server/gun.h"

#include <iostream>

#include "server/game_world.h"

Gun::Gun(const GunType& gun_type): type(gun_type) {
    switch (type) {
        case GunType::GLOCK:
            ammo = GLOCK_INITIAL_AMMO;
            tipo = GUN_GLOCK;
            rate_of_fire = GLOCK_ROF;
            break;
        case GunType::AK47:
            ammo = AK47_INITIAL_AMMO;
            tipo = NO;
            rate_of_fire = AK47_ROF;
            break;
        case GunType::AWP:
            ammo = AWP_INITIAL_AMMO;
            tipo = GUN_AWP;
            rate_of_fire = AWP_ROF;
            break;
        case GunType::M3:
            ammo = M3_INITIAL_AMMO;
            tipo = GUN_M3;
            rate_of_fire = M3_ROF;
            break;
        case GunType::NONE:
            ammo = 0;
            tipo = GUN_KNIFE;
            rate_of_fire = KNIFE_ROF;
            break;
        default:
            throw std::invalid_argument("not a gun type");
    }
    time_since_last_shot = 60.0f / rate_of_fire;
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

bool Gun::can_shoot() {
    if (tipo == GUN_KNIFE)
        return time_since_last_shot >= (60.0f / rate_of_fire);
    return ammo > 0 && time_since_last_shot >= (60.0f / rate_of_fire);
}

void Gun::update(const float& delta_t, Player& owner, GameWorld& game) {
    if (just_triggered_action && can_shoot()) {
        shoot(game, owner);
        time_since_last_shot = 0.0f;
    } else {
        time_since_last_shot += delta_t;
    }
    Weapon::update(delta_t, owner, game);
}

void Gun::shoot(GameWorld& game, Player& shooter) {
    if (tipo != GUN_KNIFE)
        ammo -= 1;

    const Vector2D<int> origin(shooter.rect.position.x + shooter.rect.width / 2,
                               shooter.rect.position.y + shooter.rect.height / 2);
    Shot shot(origin, shooter.get_orientation());
    shot.shoot(game, shooter);


    if (shot.hit != nullptr || shot.distance < 0) {
        if (tipo == GUN_GLOCK) {
            shot.hit->receive_damage(10);
        } else if (tipo == GUN_AWP) {
            shot.hit->receive_damage(100);
        } else if (tipo == GUN_KNIFE) {
            shot.hit->receive_damage(30);
        } else if (tipo == GUN_M3) {
            shot.hit->receive_damage(50);
        }

        // game.execute_shot() para guardar el evento del disparo e informar a clientes
    } else {
        // excepcion? no debería "no pegar en nada" un disparo
    }
}

uint16_t Gun::get_ammo() const { return ammo; }

GunType Gun::get_type() const { return type; }
