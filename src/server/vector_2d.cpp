#include "server/vector_2d.h"

Vector2D::Vector2D(int x, int y): x(x), y(y) {}

Vector2D Vector2D::operator+(const Vector2D& other) const { return {x + other.x, y + other.y}; }

Vector2D Vector2D::operator*(int scalar) const { return {x * scalar, y * scalar}; }

Vector2D Vector2D::operator-(const Vector2D& other) const { return {x - other.x, y - other.y}; }

int Vector2D::dot(const Vector2D& other) const { return x * other.x + y * other.y; }

int Vector2D::cross(const Vector2D& other) const { return x * other.y - y * other.x; }

double Vector2D::magnitude() const { return std::hypot(x, y); }

Vector2D::~Vector2D() {}
