#ifndef GAME_UI_PHASE_H
#define GAME_UI_PHASE_H

#include "../common/settings.h"

class GameUI;  // Forward declaration

class GameUIPhase {
protected:
    GameUI& game_ui;
    int it;
    const int FPS_CLIENT;

public:
    GameUIPhase(GameUI& game_ui, int starting_it);
    /* Metodo principal del gameloop, que posee un clock para mantener un
     * game rate constante, renderiza la ultima snapshot, maneja eventos del
     * cliente y actualiza el estado del jugador y del juego.
     */
    void run();
    void change_phase(int last_it);
    virtual void handle_game_events() = 0;
    virtual bool update_game_state() = 0;
    virtual void show_game(const int& it) = 0;
    virtual ~GameUIPhase() {}
};

class WaitingForGamePhase: public GameUIPhase {
public:
    WaitingForGamePhase(GameUI& game_ui, int starting_it);
    void handle_game_events() override;
    bool update_game_state() override;
    void show_game(const int& it) override;
};

class UIBuyPhase: public GameUIPhase {
public:
    UIBuyPhase(GameUI& game_ui, int starting_it);
    void handle_game_events() override;
    bool update_game_state() override;
    void show_game(const int& it) override;
};

class UIAttackPhase: public GameUIPhase {
public:
    UIAttackPhase(GameUI& game_ui, int starting_it);
    void handle_game_events() override;
    bool update_game_state() override;
    void show_game(const int& it) override;
};

class RoundEndedPhase: public GameUIPhase {
public:
    RoundEndedPhase(GameUI& game_ui, int starting_it);
    void handle_game_events() override;
    bool update_game_state() override;
    void show_game(const int& it) override;
};

#endif
