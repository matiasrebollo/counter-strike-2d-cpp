#ifndef GAME_UI_STATE_H
#define GAME_UI_STATE_H

#include "common/game_map.h"

class GameUI;  // Forward declaration

class GameUIState {
public:
    virtual void handle(GameUI& game_ui) = 0;
    virtual ~GameUIState() {}
};

class WaitingForGameState: public GameUIState {
public:
    WaitingForGameState();
    void handle(GameUI& game_ui) override;
};

class BuyPhaseState: public GameUIState {
private:
    const GameMap map;

public:
    explicit BuyPhaseState(const GameMap map);

    void handle(GameUI& game_ui) override;
};

class AttackPhaseState: public GameUIState {
private:
    const GameMap map;

public:
    explicit AttackPhaseState(const GameMap map);

    void handle(GameUI& game_ui) override;
};

#endif
