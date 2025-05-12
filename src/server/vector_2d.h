#ifndef VECTOR_2D_H
#define VECTOR_2D_H

#include <cmath>

struct Vector2D {
    float x, y;

    Vector2D operator+(const Vector2D& other) const;
    Vector2D operator*(float scalar) const;
    Vector2D operator-(const Vector2D& other) const;
    float dot(const Vector2D& other) const;
    float cross(const Vector2D& other) const;
    float magnitude() const;

    ~Vector2D() = default;
};

#endif
