#include "server/shop.h"

#include <memory>
#include <stdexcept>
#include <utility>

Shop::Shop() {
    prices[AWP] = AWP_PRICE;
    prices[M3] = M3_PRICE;
    prices[AK47] = AK47_PRICE;

    ammo_by_clip[AWP] = SIZE_AWP_CLIP;
    ammo_by_clip[M3] = SIZE_M3_CLIP;
    ammo_by_clip[AK47] = SIZE_AK47_CLIP;
    ammo_by_clip[GLOCK] = SIZE_GLOCK_CLIP;
}


int Shop::gun_price(const GunType& gun_type) {
    auto it = prices.find(gun_type);
    if (it != prices.end()) {
        return it->second;
    } else {
        throw std::runtime_error("No deberías conmprar un arma que no está a la venta");
    }
}

int Shop::clip_ammo(const GunType& gun_type) {
    auto it = ammo_by_clip.find(gun_type);
    if (it != ammo_by_clip.end()) {
        return it->second;
    } else {
        throw std::runtime_error("No deberías conmprar un arma que no está a la venta");
    }
}

int Shop::clip_price() { return CLIP_PRICE; }

std::unique_ptr<Gun> Shop::buy_gun(const GunType& gun_type, Loadout& buyer_loadout) {
    int price = gun_price(gun_type);
    if (buyer_loadout.get_money() < price)
        return nullptr;

    buyer_loadout.decrease_money_by(price);
    std::unique_ptr<Gun> new_gun = std::make_unique<Gun>(gun_type);
    return buyer_loadout.new_primary_gun(std::move(new_gun));
}
void Shop::buy_clip(const bool& for_primary, Loadout& buyer_loadout) {
    int price = clip_price();
    if (buyer_loadout.get_money() < price)
        return;

    if (for_primary && buyer_loadout.primary_gun_type() == NONE) {
        return;
    }

    buyer_loadout.decrease_money_by(price);
    if (for_primary) {
        buyer_loadout.add_ammo_to_primary(clip_ammo(buyer_loadout.primary_gun_type()));
    } else {
        buyer_loadout.add_ammo_to_secondary(clip_ammo(buyer_loadout.secondary_gun_type()));
    }
}

const ShopInfoDTO Shop::get_shop_info() const { return ShopInfoDTO{prices, ammo_by_clip}; }

Shop::~Shop() {}
