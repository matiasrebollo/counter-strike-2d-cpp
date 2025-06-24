#include "server/player.h"

#include <algorithm>
#include <iostream>

#include "server/game_world.h"

Player::Player(const std::string& name, Vector2D<int>& position):
        Collidable(position, PLAYER_THICKNESS, PLAYER_THICKNESS),
        name(name),
        team(CT),
        moving_up(false),
        moving_down(false),
        moving_left(false),
        moving_right(false),
        making_action(false),
        orientation(0.0),
        life(Settings::getInstance().get_player_initial_life()),
        shot(std::nullopt),
        is_planting_bomb(false),
        is_defusing_bomb(false),
        on_site(false),
        bonifications(0),
        kills(0),
        deaths(0),
        loadout(),
        FPS_SERVER(Settings::getInstance().get_fps_server()),
        PLAYER_INITIAL_LIFE(Settings::getInstance().get_player_initial_life()),
        PLAYER_SPEED(Settings::getInstance().get_player_speed()) {}

std::string Player::get_username() const { return name; }
float Player::get_orientation() const { return orientation; }
bool Player::is_ct() const { return team == CT; }
bool Player::is_tt() const { return team == TT; }
bool Player::is_alive() const { return this->life > 0; }
bool Player::has_bomb() const { return loadout.has_bomb(); }
bool Player::is_on_site() const { return this->on_site; }
bool Player::defusing_bomb() const { return is_defusing_bomb; }
WeaponType Player::equipped() const { return loadout.get_equipped(); }
void Player::change_team(const Team& new_team) { team = new_team; }

void Player::update(GameWorld& game, const float& delta_t) {
    shot = std::nullopt;
    is_planting_bomb = false;
    int delta_it = static_cast<int>(std::round(delta_t * FPS_SERVER));

    Weapon* weapon = loadout.equipped_weapon();

    if (is_defusing_bomb)
        return;

    if (making_action && on_site && dynamic_cast<Bomb*>(weapon)) {
        is_planting_bomb = true;
        weapon->update(delta_t, *this, game);
        return;
    }

    int stepped = delta_it * PLAYER_SPEED;
    Vector2D<int> step(0, 0);
    if (moving_up) {
        step = step + Vector2D<int>(0, -stepped);
    }
    if (moving_down) {
        step = step + Vector2D<int>(0, stepped);
    }
    if (moving_left) {
        step = step + Vector2D<int>(-stepped, 0);
    }
    if (moving_right) {
        step = step + Vector2D<int>(stepped, 0);
    }
    if (step.x != 0 && step.y != 0) {
        step.x = static_cast<int>(step.x / std::sqrt(2));
        step.y = static_cast<int>(step.y / std::sqrt(2));
    }

    game.make_step_player(*this, step);
    on_site = game.on_site(*this);

    if (weapon)
        weapon->update(delta_t, *this, game);
}

void Player::move_up() { moving_up = true; }
void Player::move_down() { moving_down = true; }
void Player::move_left() { moving_left = true; }
void Player::move_right() { moving_right = true; }

bool Player::collides_with(const Collidable& other_collidable) const {
    if (!is_alive())
        return false;
    return rect.intersects_with(other_collidable.rect);
}

void Player::receive_bomb(std::shared_ptr<Bomb> bomb) { loadout.receive_bomb(bomb); }
void Player::leave_bomb() {
    if (dynamic_cast<Bomb*>(loadout.equipped_weapon())) {
        equip_secondary();
    }
    loadout.leave_bomb();
}

void Player::stop_moving_up() { moving_up = false; }
void Player::stop_moving_down() { moving_down = false; }
void Player::stop_moving_left() { moving_left = false; }
void Player::stop_moving_right() { moving_right = false; }
void Player::rotate(const double& new_orientation) { this->orientation = new_orientation; }
void Player::restart() {
    life = PLAYER_INITIAL_LIFE;
    moving_up = false;
    moving_down = false;
    moving_left = false;
    moving_right = false;
    making_action = false;
    on_site = false;
    is_planting_bomb = false;
    is_defusing_bomb = false;
    shot = std::nullopt;
    orientation = 0.0;
    stop_making_action();
    leave_bomb();
}

void Player::make_action() {
    if (Weapon* weapon = loadout.equipped_weapon())
        weapon->action();
    making_action = true;
}
void Player::stop_making_action() {
    if (Weapon* weapon = loadout.equipped_weapon())
        weapon->stop_action();
    making_action = false;
}
void Player::defuse_bomb() { is_defusing_bomb = true; }
void Player::stop_defusing_bomb() { is_defusing_bomb = false; }

void Player::shoot(const Shot& a_shot) { shot = a_shot.get_dto(); }

void Player::receive_damage(const int& damage, GameWorld& game) {
    if (life == 0)
        return;
    life = std::max(life - damage, 0);
    if (life == 0) {
        deaths += 1;
        game.drop_weapons(*this);
    }
}

void Player::count_kill(Player& victim, const int& money_bonification) {
    if ((this->is_ct() && victim.is_ct()) || (this->is_tt() && victim.is_tt())) {
        bonifications -= Settings::getInstance().get_team_kill_penalty();
        loadout.decrease_money_by(Settings::getInstance().get_team_kill_penalty());
    } else {
        kills += 1;
        bonifications += money_bonification;
        loadout.add_money(money_bonification);
    }
}

void Player::unequip_weapon() {
    if (Weapon* weapon = loadout.equipped_weapon())
        weapon->stop_action();
}

void Player::equip_primary() {
    unequip_weapon();
    loadout.equip_primary();
}
void Player::equip_secondary() {
    unequip_weapon();
    loadout.equip_secondary();
}
void Player::equip_knife() {
    unequip_weapon();
    loadout.equip_knife();
}
void Player::equip_bomb() {
    unequip_weapon();
    loadout.equip_bomb();
}

Loadout& Player::get_loadout() { return loadout; }

const PlayerDTO Player::get_dto() const {
    return PlayerDTO{name,
                     rect.position,
                     orientation,
                     life,
                     shot,
                     is_planting_bomb,
                     is_defusing_bomb,
                     on_site,
                     bonifications,
                     kills,
                     deaths,
                     loadout.get_dto()};
}


Player::~Player() {}
