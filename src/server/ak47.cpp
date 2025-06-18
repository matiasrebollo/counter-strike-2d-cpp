#include "server/ak47.h"

#include "server/game_world.h"

#define BULLETS_PER_BURST 3

Ak_47::Ak_47(const GunType& type, const int& rate_of_fire, const int& base_damage,
             const int& falloff_distance, const double& base_precision, uint16_t ammo,
             const int& kill_bonification):
        Gun(type, rate_of_fire, base_damage, falloff_distance, base_precision, ammo,
            kill_bonification) {
    if (type != AK47) {
        throw std::runtime_error("Un ak47 debe tener tipo AK47");
    }
}

bool Ak_47::can_shoot() const {
    return making_action && ammo >= BULLETS_PER_BURST &&
           time_since_last_shot >= (60.0f / rate_of_fire);
}

void Ak_47::shoot(GameWorld& game, Player& shooter) {
    const int num_shots = BULLETS_PER_BURST;
    ammo -= num_shots;

    const Vector2D<int> origin(shooter.rect.position.x + shooter.rect.width / 2,
                               shooter.rect.position.y + shooter.rect.height / 2);

    Shot shot(origin, shooter.get_orientation());
    shot.shoot(game, shooter);

    if (Player* hit_player = dynamic_cast<Player*>(shot.hit)) {
        for (int i = 0; i < num_shots; ++i) {
            execute_shot(hit_player, shot.impact_info->first);
        }
        if (!hit_player->is_alive())
            shooter.count_kill(kill_bonification);
    }
}
