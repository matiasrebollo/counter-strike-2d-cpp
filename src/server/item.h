#ifndef ITEM_H
#define ITEM_H

#include <cstdint>
#include <memory>

#include "common/weapon_type.h"
#include "server/gun.h"
#include "server/rect.h"

struct Item {
    Rect rect;
    std::unique_ptr<Gun> gun;  // si es nullptr, representa la bomba.
    uint64_t drop_id;
};

#endif
