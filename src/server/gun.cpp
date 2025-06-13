#include "server/gun.h"

#include <algorithm>
#include <iostream>
#include <random>

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

double random_double(double min = 0.0, double max = 1.0) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(min, max);
    return dis(gen);
}

void Gun::execute_shoot(GameWorld& /*game*/, Player* shot_victim, double shot_distance) {
    const auto& stats = GUN_STATS.at(tipo);

    double precision = stats.precision_base;
    double falloff = std::min((shot_distance / stats.fallof_distance), 1.0);
    std::cout << "falloff: " << falloff << std::endl;
    int damage = stats.damage;
    if (tipo == GUN_KNIFE) {
        if (shot_distance < KNIFE_DISTANCE) {
            std::cout << "cuchillada" << std::endl;
            shot_victim->receive_damage(damage);
            // game.execute_shot() para guardar el evento del disparo e informar a clientes
        }
        // game.execute_shot() para guardar el evento del disparo e informar a clientes
        return;
    }
    if (!stats.ignores_precision) {
        precision *= (1.0 - 0.5 * falloff);
    }

    if (random_double() <=
        precision) {  // si el random es menor o igual a precision, el disparo impacta.
        if (stats.damage_falls_with_distance) {
            damage = static_cast<int>(damage * (1.0 - 0.5 * falloff));
        }

        double variation =
                random_double(1.0 - DAMAGE_VARIATION_FACTOR, 1.0 + DAMAGE_VARIATION_FACTOR);
        damage = static_cast<int>(damage * variation);
        shot_victim->receive_damage(damage);
        // game.execute_shot() para guardar el evento del disparo e informar a clientes
    } else {
        // game.execute_shot() con impacto en posicion player pero sin daño
    }
}

void Gun::shoot(GameWorld& game, Player& shooter) {
    if (tipo != GUN_KNIFE)
        ammo -= 1;

    const Vector2D<int> origin(shooter.rect.position.x + shooter.rect.width / 2,
                               shooter.rect.position.y + shooter.rect.height / 2);
    Shot shot(origin, shooter.get_orientation());
    shot.shoot(game, shooter);

    if (Player* hit_player = dynamic_cast<Player*>(shot.hit)) {
        execute_shoot(game, hit_player, shot.distance);
    } else {
        // game.execute_shot() con impacto en objeto del mapa
    }
}

uint16_t Gun::get_ammo() const { return ammo; }

GunType Gun::get_type() const { return type; }
