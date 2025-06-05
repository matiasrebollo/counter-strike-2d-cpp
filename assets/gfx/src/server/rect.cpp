#include "server/rect.h"

Rect::Rect(Vector2D position, int width, int height):
        position(position), width(width), height(height) {}

bool Rect::intersects_with(const Rect& other_rect) const {
    return !(position.x + width <= other_rect.position.x ||
             position.x >= other_rect.position.x + other_rect.width ||
             position.y + height <= other_rect.position.y ||
             position.y >= other_rect.position.y + other_rect.height);
}

Rect::~Rect() {}
