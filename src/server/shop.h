#ifndef SHOP_H
#define SHOP_H

#include <memory>
#include <unordered_map>

#include "common/game_info_dto.h"
#include "common/settings.h"
#include "server/gun.h"
#include "server/loadout.h"

class Shop {
private:
    std::unordered_map<GunType, int> prices;
    std::unordered_map<GunType, int> ammo_by_clip;

    int gun_price(const GunType& gun_type);
    int clip_price();
    int clip_ammo(const GunType& gun_type);

public:
    Shop();

    std::unique_ptr<Gun> buy_gun(const GunType& gun_type, Loadout& buyer_loadout);
    void buy_clip(const bool& for_primary, Loadout& buyer_loadout);

    const ShopInfoDTO get_shop_info() const;

    Shop(const Shop&) = delete;
    Shop& operator=(const Shop&) = delete;

    ~Shop();
};

#endif
