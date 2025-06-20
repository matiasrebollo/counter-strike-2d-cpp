#include "server/item.h"

#include "server/game_world.h"

Item::Item(const Rect& rect, uint64_t drop_id): rect(rect), drop_id(drop_id) {}

uint64_t Item::get_drop_id() const { return drop_id; }
void Item::set_drop_id(const uint64_t& new_drop_id) { drop_id = new_drop_id; }

DroppedGun::DroppedGun(const Rect& rect, std::unique_ptr<Gun> gun, uint64_t drop_id):
        Item(rect, drop_id), gun(std::move(gun)) {}
std::unique_ptr<Gun> DroppedGun::take_gun() { return std::move(gun); }
GunType DroppedGun::get_gun_type() const { return gun->get_type(); }
void DroppedGun::try_pick_up(GameWorld& game, Player& player) {
    game.pick_up_gun_for(player, *this);
}

DroppedBomb::DroppedBomb(const Rect& rect, uint64_t drop_id): Item(rect, drop_id) {}
void DroppedBomb::try_pick_up(GameWorld& game, Player& player) {
    game.try_pick_up_bomb_for(player, *this);
}
