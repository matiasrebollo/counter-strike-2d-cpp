#include "game_ui_state.h"

#include "game_ui.h"

WaitingForGameState::WaitingForGameState() {}

void WaitingForGameState::handle(GameUI& game_ui) { game_ui.handle_waiting_phase(); }

BuyPhaseState::BuyPhaseState(const GameMap map): map(map) {}

void BuyPhaseState::handle(GameUI& game_ui) { game_ui.handle_buy_phase(this->map); }

AttackPhaseState::AttackPhaseState(const GameMap map): map(map) {}

void AttackPhaseState::handle(GameUI& game_ui) { game_ui.handle_attack_phase(this->map); }

GameEndedState::GameEndedState() {}

void GameEndedState::handle(GameUI& game_ui) { game_ui.handle_game_ended_phase(); }
