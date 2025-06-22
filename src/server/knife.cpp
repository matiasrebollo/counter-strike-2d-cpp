#include "server/knife.h"

#include "common/settings.h"
#include "server/game_world.h"

Knife::Knife():
        damage(Settings::getInstance().get_knife_dmg()),
        attack_rate(Settings::getInstance().get_knife_ar()),
        time_since_last_stab(60.0f / attack_rate),
        kill_bonification(Settings::getInstance().get_knife_kill_bonus()) {}

bool Knife::can_stab() {
    return just_triggered_action && time_since_last_stab >= (60.0f / attack_rate);
}

void Knife::update(const float& delta_t, Player& owner, GameWorld& game) {
    if (can_stab()) {
        stab(game, owner);
        time_since_last_stab = 0.0f;
    } else {
        time_since_last_stab += delta_t;
    }
    Weapon::update(delta_t, owner, game);
}

void Knife::execute_stab(Player* shot_victim, double shot_distance) {
    if (shot_distance < Settings::getInstance().get_knife_distance()) {
        shot_victim->receive_damage(damage, game);
    }
}

void Knife::stab(GameWorld& game, Player& shooter) {

    const Vector2D<int> origin(shooter.rect.position.x + shooter.rect.width / 2,
                               shooter.rect.position.y + shooter.rect.height / 2);
    Shot shot(origin, shooter.get_orientation());
    shot.shoot(game, shooter);

    if (Player* hit_player = dynamic_cast<Player*>(shot.hit)) {
        execute_stab(hit_player, shot.impact_info->first, game);
        if (!hit_player->is_alive())
            shooter.count_kill(*hit_player, kill_bonification);
    }
}
