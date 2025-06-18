#ifndef VECTOR_2D_H
#define VECTOR_2D_H

#include <cmath>
#include <type_traits>

template <typename T>
class Vector2D {
public:
    T x, y;

    explicit Vector2D(T x = T(0), T y = T(0)): x(x), y(y) {}

    Vector2D operator+(const Vector2D& other) const { return Vector2D(x + other.x, y + other.y); }

    Vector2D operator-(const Vector2D& other) const { return Vector2D(x - other.x, y - other.y); }

    Vector2D operator*(T scalar) const { return Vector2D(x * scalar, y * scalar); }

    bool operator==(const Vector2D& other) const { return x == other.x && y == other.y; }

    bool operator<(const Vector2D& other) const {
        return (x < other.x) || (x == other.x && y < other.y);
    }

    T dot(const Vector2D& other) const { return x * other.x + y * other.y; }

    T cross(const Vector2D& other) const { return x * other.y - y * other.x; }

    double magnitude() const { return std::hypot(static_cast<double>(x), static_cast<double>(y)); }

    Vector2D<double> normalized() const {
        double mag = magnitude();
        if (mag == 0.0)
            return Vector2D<double>(0.0, 0.0);
        return Vector2D<double>(static_cast<double>(x) / mag, static_cast<double>(y) / mag);
    }
};

#endif
