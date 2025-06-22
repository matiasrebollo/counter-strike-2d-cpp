#include "server/shop.h"

#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

Shop::Shop() {
    std::vector<GunType> guns = {AWP, M3, AK47, GLOCK};
    Settings& settings = Settings::getInstance();

    for (const auto& gun: guns) {
        prices[gun] = settings.get_gun_price(gun);
        ammo_by_clip[gun] = settings.get_clip_size(gun);
    }
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

int Shop::clip_price() const { return Settings::getInstance().get_clip_price(); }

std::unique_ptr<Gun> Shop::buy_gun(const GunType& gun_type, Loadout& buyer_loadout) {
    int price = gun_price(gun_type);
    if (buyer_loadout.get_money() < price)
        return nullptr;

    buyer_loadout.decrease_money_by(price);
    std::unique_ptr<Gun> new_gun = Gun::new_gun(gun_type);
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

const ShopInfoDTO Shop::get_shop_info() const {
    int price = clip_price();
    return ShopInfoDTO{prices, ammo_by_clip, price};
}

Shop::~Shop() {}
