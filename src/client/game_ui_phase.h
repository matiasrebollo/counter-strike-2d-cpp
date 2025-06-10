#ifndef GAME_UI_PHASE_H
#define GAME_UI_PHASE_H

class GameUI;  // Forward declaration

class GameUIPhase {
protected:
    GameUI& game_ui;

public:
    explicit GameUIPhase(GameUI& game_ui);
    /* Metodo principal del gameloop, que posee un clock para mantener un
     * game rate constante, renderiza la ultima snapshot, maneja eventos del
     * cliente y actualiza el estado del jugador y del juego.
     */
    void run();
    void change_phase();
    virtual void handle_game_events() = 0;
    virtual bool update_game_state() = 0;
    virtual void show_game(const int& it) = 0;
    virtual ~GameUIPhase() {}
};

class WaitingForGamePhase: public GameUIPhase {
public:
    explicit WaitingForGamePhase(GameUI& game_ui);
    void handle_game_events() override;
    bool update_game_state() override;
    void show_game(const int& it) override;
};

class UIBuyPhase: public GameUIPhase {
public:
    explicit UIBuyPhase(GameUI& game_ui);
    void handle_game_events() override;
    bool update_game_state() override;
    void show_game(const int& it) override;
};

class UIAttackPhase: public GameUIPhase {
public:
    explicit UIAttackPhase(GameUI& game_ui);
    void handle_game_events() override;
    bool update_game_state() override;
    void show_game(const int& it) override;
};

/*class GameEndedPhase: public GameUIPhase {
public:
    explicit GameEndedPhase(GameUI& game_ui);
    void handle_game_events() override;
    void update_game_state() override;
    bool keep_running() override;
    void show_game() override;
};*/

#endif
