#ifndef ITEM_H
#define ITEM_H

#include <cstdint>
#include <memory>
#include <utility>

class GameWorld;

#include "common/item_dto.h"
#include "common/weapon_type.h"
#include "server/gun.h"
#include "server/rect.h"

class Item {
public:
    Rect rect;

protected:
    uint64_t drop_id;

public:
    Item(const Rect& rect, uint64_t drop_id);

    virtual ~Item() = default;

    uint64_t get_drop_id() const;
    void set_drop_id(const uint64_t& new_drop_id);

    virtual void try_pick_up(GameWorld& game, Player& player) = 0;

    virtual ItemDTO get_dto() const = 0;
};

class DroppedGun: public Item {
private:
    std::unique_ptr<Gun> gun;

public:
    DroppedGun(const Rect& rect, std::unique_ptr<Gun> gun, uint64_t drop_id);

    GunType get_gun_type() const;

    std::unique_ptr<Gun> take_gun();

    void try_pick_up(GameWorld& game, Player& player) override;

    ItemDTO get_dto() const override;
};

class DroppedBomb: public Item {
public:
    DroppedBomb(const Rect& rect, uint64_t drop_id);

    void try_pick_up(GameWorld& game, Player& player) override;

    ItemDTO get_dto() const override;
};

#endif
