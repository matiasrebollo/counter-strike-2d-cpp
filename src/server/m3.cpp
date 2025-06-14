#include "server/m3.h"

#include "server/game_world.h"

M_3::M_3(const GunType& type, const int& rate_of_fire, const int& base_damage,
         const int& falloff_distance, const double& base_precision, uint16_t ammo,
         const int& kill_bonification):
        Gun(type, rate_of_fire, base_damage, falloff_distance, base_precision, ammo,
            kill_bonification) {
    if (type != M3) {
        throw std::runtime_error("Un m3 debe tener tipo M3");
    }
}

void M_3::shoot(GameWorld& game, Player& shooter) {
    ammo -= 1;

    const Vector2D<int> origin(shooter.rect.position.x + shooter.rect.width / 2,
                               shooter.rect.position.y + shooter.rect.height / 2);

    double central_angle_deg = shooter.get_orientation();
    const double cone_angle_deg = 30.0;
    const int num_pellets = 8;

    for (int i = 0; i < num_pellets; ++i) {
        double t = (i + 0.5) / num_pellets;          // En (0,1)
        double offset = (t - 0.5) * cone_angle_deg;  // En (-cone/2, cone/2)
        double angle_deg = central_angle_deg + offset;

        Shot shot(origin, angle_deg);
        shot.shoot(game, shooter);

        if (Player* hit_player = dynamic_cast<Player*>(shot.hit)) {
            execute_shot(hit_player, shot.distance);
            if (!hit_player->is_alive())
                shooter.count_kill(kill_bonification);
        }
    }

    // game.execute_shot(...)
}
