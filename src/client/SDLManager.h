#ifndef SDLMANAGER_H
#define SDLMANAGER_H

#include <optional>
#include <string>
#include <utility>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>

#include "../common/block_texture_parser.h"
#include "../common/game_info_dto.h"
#include "../common/game_snapshot.h"
#include "../common/settings.h"

#include "camera.h"
#include "local_info.h"
#include "shop.h"
#include "sounds.h"

class SDLManager {
private:
    SDL2pp::SDL sdl;
    SDL2pp::SDLTTF ttf;
    SDL2pp::SDLMixer mix;
    SDL2pp::Mixer mixer;
    SDL2pp::Window window;
    SDL2pp::Renderer renderer;
    TextureManager texture_manager;
    BlockTextureParser texture_parser;
    Camera camera;
    Shop shop;
    Sounds sounds;
    std::optional<GameMapDTO> map;

    void update_camera(int player_x, int player_y);
    std::pair<Position, GunSprites> get_gun_info(const WeaponType& equipped,
                                                 const GunType& primary_gun);
    void render_player(const PlayerInfo& p, const BlockTextureInfo& sprite_info);
    void render_player_weapon(const PlayerInfo& p);
    void render_fov(float orientation_deg);
    void render_hud_time(int time_left);
    void render_hud_life(int life);
    void render_hud_ammo(int ammo);
    void render_hud_money(int money);
    Crosshairs get_crosshair_color(int mouse_x, int mouse_y, const LocalInfo& local_info);

public:
    SDLManager();

    /* Se asigna el mapa una vez que es recibido cuando comienza la partida */
    void set_map(GameMapDTO game_map);


    /* Se asigna los valores de la shop una vez que es recibido cuando comienza la partida*/
    void set_shop(const ShopInfoDTO& shop_info);

    /* Se asigna la cantidad total de players para manejar la cantidad de canales de sonido */
    void set_total_players(int total_players);

    /* Renderiza la fase de waiting */
    void render_waiting_screen(int players_connected, int players_required,
                               const std::string& gamename, int iteration, int FPS);

    /* Devuelve las coords logicas del renderizador */
    SDL_Point get_logical_size() const;

    /* Limpia la pantalla */
    void clear_display();

    /* Renderiza en orden de profundidad el mapa, cada jugador con su arma, el FOV y el HUD */
    void render_in_z_order(const LocalInfo& local_info, int it);

    /* Devuelve què boton de la tienda fue clickeado si alguno fue clickeado si click es true
     * o interactua con el boton si es false.
     */
    std::optional<ShopButtonType> interact_button(int x, int y, int money, GunType primary,
                                                  bool click);

    /* Renderiza la tienda de la fase de compra */
    void render_shop(int player_money, GunType primary_gun, GunType secondary_gun);

    /* Renderiza la mira en la posicion del mouse */
    void render_crosshair(const LocalInfo& local_info);

    /* Muestra lo dibujado en pantalla */
    void show_screen();
};

#endif
