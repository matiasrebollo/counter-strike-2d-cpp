#include "server/player.h"

#include <algorithm>
#include <iostream>
#include <limits>

#include "server/hitbox.h"

Player::Player(Vector2D pos): hitbox(pos, 0.5f, 0.5f), direction(0, 1) {}

void Player::step(Vector2D dir, const std::vector<Collidable>& obstacles) {
    Hitbox new_hitbox = hitbox;
    new_hitbox.position = new_hitbox.position + dir * 0.25f;
    std::cout << "Jugador moviendose a: (" << new_hitbox.position.x << "," << new_hitbox.position.y
              << ")\n";

    if (std::any_of(obstacles.begin(), obstacles.end(), [&new_hitbox](const Collidable& obj) {
            return new_hitbox.intersects(obj.get_hitbox());
        })) {
        std::cout << "¡Colisión! No se puede mover ahí.\n";
        return;
    }

    hitbox.position = new_hitbox.position;
}

void Player::rotate(Vector2D dir) {
    if (dir.x == 0 && dir.y == 0)
        return;
    direction = dir;
}

void Player::shoot(const std::vector<Collidable>& obstacles) const {
    const Vector2D origin = hitbox.position;

    float closest_distance = std::numeric_limits<float>::max();
    const Collidable* first_hit = nullptr;

    for (const auto& obj: obstacles) {
        Hitbox target = obj.get_hitbox();

        Vector2D to_obj = target.position - origin;
        float dist = to_obj.magnitude();

        if (shoot_intersects_hitbox(origin, direction, target)) {
            if (dist < closest_distance) {
                closest_distance = dist;
                first_hit = &obj;
            }
        }
    }

    if (first_hit) {
        Hitbox h = first_hit->get_hitbox();
        std::cout << "¡Impacto! Disparo acertó a objeto en (" << h.position.x << ", "
                  << h.position.y << ")\n";
    } else {
        std::cout << "Disparo fallido. No impactó ningún objeto.\n";
    }
}

bool Player::shoot_intersects_hitbox(const Vector2D& shoot_origin, const Vector2D& shoot_dir,
                                     const Hitbox& hitbox) const {

    Vector2D v1 = hitbox.position;
    Vector2D v2 = {hitbox.position.x + hitbox.width, hitbox.position.y};
    Vector2D v3 = {hitbox.position.x + hitbox.width, hitbox.position.y + hitbox.height};
    Vector2D v4 = {hitbox.position.x, hitbox.position.y + hitbox.height};

    return shoot_intersects_segment(shoot_origin, shoot_dir, v1, v2) ||
           shoot_intersects_segment(shoot_origin, shoot_dir, v2, v3) ||
           shoot_intersects_segment(shoot_origin, shoot_dir, v3, v4) ||
           shoot_intersects_segment(shoot_origin, shoot_dir, v4, v1);
}

// R(t) = shoot_origin + shoot_dir * t, con t ≥ 0 - Recta por la que recorrerá el disparo.
// S(u) = seg_start + seg_dir * u, con 0 ≤ u ≤ 1 - Segmento, se quiere ver si la recta lo corta.
// Buscamos u y t para los que se cumpla: shoot_origin + shoot_dir * t  ==  seg_start + seg_dir * u
// => shoot_dir * t - seg_dir * u = seg_start - shoot_origin
// => shoot_dir * t + (-seg_dir) * u = r (siendo r = seg_start - shoot_origin)
// => ... (wolfram) =>  t = (r x (seg_dir)) / ((shoot_dir))x(seg_dir)), u = (shoot_dir x r) /
// ((shoot_dir))x(seg_dir))

bool Player::shoot_intersects_segment(const Vector2D& shoot_origin, const Vector2D& shoot_dir,
                                      const Vector2D& seg_start, const Vector2D& seg_end) const {

    Vector2D seg_dir = seg_end - seg_start;
    Vector2D r = shoot_origin - seg_start;

    float cross1 = shoot_dir.cross(seg_dir);
    float cross2 = r.cross(seg_dir);
    float cross3 = shoot_dir.cross(r);

    if (cross1 == 0)
        return false;  // tiro es paralelo a segmento con el que se busca intersecar (no se cruzan)

    float t = cross2 / cross1;
    float u = cross3 / cross1;

    return t >= 0 && t <= 1 && u >= 0;
}

void Player::print_position() const {
    std::cout << "Jugador en (" << hitbox.position.x << ", " << hitbox.position.y << ")\n";
}

Player::~Player() {}
