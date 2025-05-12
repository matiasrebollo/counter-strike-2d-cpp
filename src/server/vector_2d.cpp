#include "server/vector_2d.h"

Vector2D Vector2D::operator+(const Vector2D& other) const { return {x + other.x, y + other.y}; }

Vector2D Vector2D::operator*(float scalar) const { return {x * scalar, y * scalar}; }

Vector2D Vector2D::operator-(const Vector2D& other) const { return {x - other.x, y - other.y}; }

float Vector2D::dot(const Vector2D& other) const { return x * other.x + y * other.y; }

float Vector2D::cross(const Vector2D& other) const { return x * other.y - y * other.x; }

float Vector2D::magnitude() const { return std::hypot(x, y); }
