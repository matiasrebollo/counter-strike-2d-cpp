#ifndef VECTOR_2D_H
#define VECTOR_2D_H

#include <cmath>

class Vector2D {
public:
    int x, y;

    Vector2D(int x, int y);
    Vector2D operator+(const Vector2D& other) const;
    Vector2D operator*(int scalar) const;
    Vector2D operator-(const Vector2D& other) const;
    int dot(const Vector2D& other) const;
    int cross(const Vector2D& other) const;
    double magnitude() const;

    ~Vector2D();
};

#endif
