#ifndef RECT_H
#define RECT_H

#include "common/vector_2d.h"

class Rect {
public:
    Vector2D position;
    int width, height;

    Rect(Vector2D position, int width, int height);

    bool intersects_with(const Rect& other_Rect) const;

    // Permito la copia para una Rect
    Rect(const Rect&) = default;
    Rect& operator=(const Rect&) = default;

    ~Rect();
};

#endif
