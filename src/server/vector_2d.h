#ifndef VECTOR_2D_H
#define VECTOR_2D_H

struct Vector2D {
    float x, y;

    Vector2D operator+(const Vector2D& other) const { return {x + other.x, y + other.y}; }

    Vector2D operator*(float scalar) const { return {x * scalar, y * scalar}; }
};

#endif
