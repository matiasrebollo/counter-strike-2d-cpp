#include "server/gun.h"

#include <algorithm>
#include <iostream>
#include <random>

#include "server/ak47.h"
#include "server/awp.h"
#include "server/game_world.h"
#include "server/glock.h"
#include "server/m3.h"


Gun::Gun(const GunType& type, const int& rate_of_fire, const int& base_damage,
         const int& falloff_distance, const double& base_precision, uint16_t ammo,
         const int& kill_bonification):
        type(type),
        rate_of_fire(rate_of_fire),
        base_damage(base_damage),
        falloff_distance(falloff_distance),
        base_precision(base_precision),
        time_since_last_shot(0.0f),
        ammo(ammo),
        kill_bonification(kill_bonification) {}

std::unique_ptr<Gun> Gun::new_gun(const GunType& type) {
    switch (type) {
        case GunType::GLOCK:
            return std::make_unique<Glock>(type, GLOCK_ROF, GLOCK_DMG, GLOCK_FALLOF,
                                           GLOCK_PRECISION, GLOCK_INITIAL_AMMO, GLOCK_KILL_BONUS);
        case GunType::AK47:
            return std::make_unique<Ak_47>(type, AK47_ROF, AK47_DMG, AK47_FALLOF, AK47_PRECISION,
                                           AK47_INITIAL_AMMO, AK47_KILL_BONUS);
        case GunType::AWP:
            return std::make_unique<Awp>(type, AWP_ROF, AWP_DMG, -1, 1.0, AWP_INITIAL_AMMO,
                                         AWP_KILL_BONUS);
        case GunType::M3:
            return std::make_unique<M_3>(type, M3_ROF, M3_DMG, M3_FALLOF, M3_PRECISION,
                                         M3_INITIAL_AMMO, M3_KILL_BONUS);
        default:
            throw std::invalid_argument("not a gun type");
    }
}

void Gun::add_ammo(uint16_t ammo_count) { ammo += ammo_count; }

uint16_t Gun::get_ammo() const { return ammo; }

GunType Gun::get_type() const { return type; }

double Gun::random_double(double min, double max) const {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(min, max);
    return dis(gen);
}

double Gun::calculate_falloff(const double& shot_distance) const {
    return std::min((shot_distance / falloff_distance), 1.0);
}

double Gun::calculate_precision(const double& falloff) const {
    double precision = base_precision;
    precision *= (1.0 - 0.5 * falloff);
    return precision;
}

int Gun::calculate_damage(const double& falloff) const {
    int damage = base_damage;
    damage = static_cast<int>(damage * (1.0 - 0.5 * falloff));
    double variation = random_double(1.0 - DAMAGE_VARIATION_FACTOR, 1.0 + DAMAGE_VARIATION_FACTOR);
    damage = static_cast<int>(damage * variation);
    return damage;
}

bool Gun::should_impact(const double& precision) const { return random_double() < precision; }

void Gun::execute_shot(Player* shot_victim, const double& shot_distance, GameWorld& game) {
    const double shot_falloff = calculate_falloff(shot_distance);
    const double shot_precision = calculate_precision(shot_falloff);

    if (should_impact(shot_precision)) {
        const int shot_damage = calculate_damage(shot_falloff);
        shot_victim->receive_damage(shot_damage, game);
    }
}

void Gun::shoot(GameWorld& game, Player& shooter) {
    ammo -= 1;
    const Vector2D<int> origin(shooter.rect.position.x + shooter.rect.width / 2,
                               shooter.rect.position.y + shooter.rect.height / 2);
    Shot shot(origin, shooter.get_orientation());
    shot.shoot(game, shooter);

    if (Player* hit_player = dynamic_cast<Player*>(shot.hit)) {
        execute_shot(hit_player, shot.impact_info->first, game);
        if (!hit_player->is_alive())
            // avisar al gameworld que murio para drop items.
            shooter.count_kill(*hit_player, kill_bonification);
    }
}

bool Gun::can_shoot() const {
    return just_triggered_action && ammo > 0 && time_since_last_shot >= (60.0f / rate_of_fire);
}

void Gun::update(const float& delta_t, Player& owner, GameWorld& game) {
    if (can_shoot()) {
        shoot(game, owner);
        time_since_last_shot = 0.0f;
    } else {
        time_since_last_shot += delta_t;
    }
    Weapon::update(delta_t, owner, game);
}
