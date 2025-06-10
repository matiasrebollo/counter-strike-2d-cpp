#ifndef LOADOUT_H
#define LOADOUT_H

#include <cstdint>
#include <memory>
#include <optional>

#include "common/loadout_dto.h"
#include "server/gun.h"
#include "server/knife.h"

#define INITIAL_MONEY 20000
#define CLIP_PRICE 50

/*
 * Loadout
 * Objeto encargado del manejo del equipamiento y compras para un jugador
 *
 * Se encarga de manejar el equipamiento del jugador,
 * efectuando las compras de armas y munición en caso de ser estas posibles.
 *
 * */
class Loadout {
private:
    uint16_t money;
    Knife knife;
    std::unique_ptr<Gun> primary_gun;
    Gun secondary_gun;
    WeaponType equipped;

    /*
     * Devuelve la cantidad de balas de un cargador para un tipo de arma.
     *
     * */
    static uint16_t ammo_per_clip_for(const GunType& gun_type);

    /*
     * Devuelve el precio de un tipo de arma.
     *
     * */
    static uint16_t price_for(const GunType& gun_type);

    /*
     * Disminuye el dinero en amount_of_money.
     *
     * */
    void decrease_money_by(const uint16_t& amount_of_money);
    /*
     * Cambia el arma primaria a gun, y la cantidad de balas del arma primaria a 30.
     *
     * Devuelve el arma primaria que poseía anteriormente.
     *
     * */
    std::unique_ptr<Gun> new_primary_gun(std::unique_ptr<Gun> gun);
    /*
     * Devuelve true si el jugador tiene el suficiente dinero para comprar un arma,
     * false en caso contrario.
     *
     * */
    bool can_buy_gun(const GunType& gun_type) const;

public:
    /*
     * Constructor de Loadout
     *
     * El equipamiento de un jugador inicialmente es:
     *
     * Dinero: 500.
     * Cuchillo.
     * Arma primaria: Ninguna.
     * Arma secundaria: Glock.
     *
     * */
    Loadout();

    /*
     * Compra un arma primaria para un jugador.
     *
     * Si la compra puede ser efectuada, disminuye el dinero en el precio de dicho arma, cambia el
     * arma primaria sin importar la munición de la y devuelve el arma primaria que poseía
     * anteriormente.
     *
     * Devuelve std::nullopt si no se tenía arma equipada o no se pudo efectuar la compra.
     *
     * */
    std::unique_ptr<Gun> buy_primary_gun(const GunType& gun_type);

    /*
     * Compra munición para algún arma del equipamiento.
     *
     * Si for_primary es true, compra balas para el arma primaria, caso contrario para el arma
     * secundaria.
     *
     * Si la compra puede ser efectuada, disminuye el dinero en la cantidad de balas compradas
     * por el precio de cada cargador (depende del tipo de arma para la que se compra),
     * aumenta la cantidad de balas del arma que corresponda y devuelve true.
     *
     * Devuelve false si el jugador no tiene el suficiente dinero para comprar las balas pedidas
     * o si se intenta comprar para el arma primaria y no se tiene una equipada.
     *
     * */
    bool buy_ammo(const bool& for_primary);

    void equip_primary();
    void equip_secondary();
    void equip_knife();
    Gun* equipped_gun();
    const LoadoutDTO get_dto() const;

    Loadout(const Loadout&) = delete;
    Loadout& operator=(const Loadout&) = delete;

    ~Loadout();
};

#endif
