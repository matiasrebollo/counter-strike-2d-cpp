#include "server/shot.h"

#include <algorithm>
#include <limits>
#include <vector>

Shot::Shot(Vector2D origin, Vector2D direction): origin(origin), direction(direction) {}

const Collidable* Shot::shoot(const std::list<std::shared_ptr<Collidable>>& obstacles) const {
    const Collidable* hit = nullptr;
    double closest = std::numeric_limits<double>::max();

    for (const auto& collidable: obstacles) {
        double dist = impacts(*collidable);
        if (dist != 0.0) {
            if (dist < closest) {
                closest = dist;
                hit = collidable.get();
            }
        }
    }

    return hit;
}

double Shot::impacts(const Collidable& collidable) const {
    Hitbox h = collidable.get_hitbox();
    Vector2D v1 = h.position;
    Vector2D v2 = {h.position.x + h.width, h.position.y};
    Vector2D v3 = {h.position.x + h.width, h.position.y + h.height};
    Vector2D v4 = {h.position.x, h.position.y + h.height};

    std::vector<double> distances = {intersects_segment(v1, v2), intersects_segment(v2, v3),
                                     intersects_segment(v3, v4), intersects_segment(v4, v1)};

    auto it = std::min_element(distances.begin(), distances.end(), [](double a, double b) {
        if (a == 0.0)
            return false;
        if (b == 0.0)
            return true;
        return a < b;
    });

    return (it != distances.end() && *it > 0.0) ? *it : 0.0;
}


// R(t) = origin + direction * t, con t ≥ 0 - Semirrecta por la que recorrerá el disparo.
// S(u) = seg_start + seg_dir * u, con 0 ≤ u ≤ 1 - Segmento, se quiere ver si la recta lo corta.
// Buscamos u y t para los que se cumpla: origin + direction * t  ==  seg_start + seg_dir * u
// => direction * t - seg_dir * u = seg_start - origin
// => direction * t + (-seg_dir) * u = r (siendo r = seg_start - origin)
// => ... (wolfram) =>  t = (r x (seg_dir)) / ((direction))x(seg_dir)), u = (r x direction) /
// ((shoot_direction))x(seg_dir))

double Shot::intersects_segment(const Vector2D& seg_start, const Vector2D& seg_end) const {

    Vector2D seg_dir = seg_end - seg_start;
    Vector2D r = seg_start - this->origin;

    double c = static_cast<double>(this->direction.cross(seg_dir));

    if (c == 0)
        return 0.0;  // son paralelos, no hay intersección

    double t = static_cast<double>(r.cross(seg_dir)) / c;
    double u = static_cast<double>(r.cross(this->direction)) / c;

    // La semirrecta solo vale para t >= 0, y el segmento para u ∈ [0,1]. Se intersecan si t y u
    // cumplen con esto.
    if (t >= 0 && u >= 0 && u <= 1) {
        return t * this->direction.magnitude();
    }

    return 0.0;
}

Shot::~Shot() {}
