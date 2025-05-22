#ifndef LOADOUT_MANAGER_H
#define LOADOUT_MANAGER_H

#include <cstdint>
#include <memory>
#include <optional>

#include "server/glock.h"
#include "server/gun.h"

#define INITIAL_MONEY 500

/*
 * LoadoutManager
 * Objeto encargado del manejo del equipamiento y compras para un jugador
 *
 * Se encarga de manejar el equipamiento del jugador,
 * efectuando las compras de armas y munición en caso de ser estas posibles.
 *
 * */
class LoadoutManager {
private:
    uint16_t money;
    Knife knife;
    std::unique_ptr<Gun> primary_weapon;
    Glock secondary_weapon;

    /*
     * Devuelve el precio de una bala de un tipo de arma.
     *
     * */
    static uint16_t ammo_price_for(const WeaponType& weapon_type);

    /*
     * Devuelve el precio de un tipo de arma.
     *
     * */
    static uint16_t price_for(const WeaponType& weapon_type);

    /*
     * Disminuye el dinero en amount_of_money.
     *
     * */
    void decrease_money_by(const uint16_t& amount_of_money);
    /*
     * Cambia el arma primaria a weapon, y la cantidad de balas del arma primaria a 30.
     *
     * Devuelve el arma primaria que poseía anteriormente.
     *
     * */
    const std::unique_ptr<Gun> new_primary_weapon(std::unique_ptr<Gun> weapon);

    /*
     * Devuelve true si el jugador tiene el suficiente dinero para comprar un arma,
     * false en caso contrario.
     *
     * */
    bool can_buy_weapon(const WeaponType& weapon_type) const;

    /*
     * Devuelve true si el jugador tiene el suficiente dinero para comprar las balas pedidas
     * para el arma que corresponda, false en caso contrario.
     *
     * */
    bool can_buy_ammo(const uint16_t& ammo_count, bool for_primary) const;

public:
    /*
     * Constructor de LoadoutManager
     *
     * El equipamiento de un jugador inicialmente es:
     *
     * Dinero: 500.
     * Cuchillo.
     * Arma primaria: Ninguna.
     * Arma secundaria: Glock.
     *
     * */
    LoadoutManager();

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
    const std::unique_ptr<Gun> LoadoutManager::buy_primary_weapon(const WeaponType& weapon_type);

    /*
     * Compra munición para algún arma del equipamiento.
     *
     * Si for_primary es true, compra balas para el arma primaria, caso contrario para el arma
     * secundaria.
     *
     * Si la compra puede ser efectuada, disminuye el dinero en la cantidad de balas compradas
     * por el precio de cada bala (depende del tipo de arma para la que se compra),
     * aumenta la cantidad de balas del arma que corresponda y devuelve true.
     *
     * Devuelve false si el jugador no tiene el suficiente dinero para comprar las balas pedidas
     * o si se intenta comprar para el arma primaria y no se tiene una equipada.
     *
     * */
    bool buy_ammo(const uint16_t& ammo_count, bool for_primary);

    LoadoutManager(const LoadoutManager&) = delete;
    LoadoutManager& operator=(const LoadoutManager&) = delete;

    ~LoadoutManager();
};

#endif
