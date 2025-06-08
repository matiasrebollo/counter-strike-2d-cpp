#ifndef GAME_UI_PHASE_H
#define GAME_UI_PHASE_H

class GameUI;  // Forward declaration

class GameUIPhase {
protected:
    GameUI& game_ui;

public:
    explicit GameUIPhase(GameUI& game_ui);
    void run();
    void change_phase();
    virtual void handle_game_events() = 0;
    virtual void update_game_state() = 0;
    virtual bool keep_running() = 0;
    virtual void show_game(const int& it) = 0;
    virtual ~GameUIPhase() {}
};

class WaitingForGamePhase: public GameUIPhase {
public:
    explicit WaitingForGamePhase(GameUI& game_ui);
    void handle_game_events() override;
    void update_game_state() override;
    bool keep_running() override;
    void show_game(const int& it) override;
};

class UIBuyPhase: public GameUIPhase {
public:
    explicit UIBuyPhase(GameUI& game_ui);
    void handle_game_events() override;
    void update_game_state() override;
    bool keep_running() override;
    void show_game(const int& it) override;
};

class UIAttackPhase: public GameUIPhase {
public:
    explicit UIAttackPhase(GameUI& game_ui);
    void handle_game_events() override;
    void update_game_state() override;
    bool keep_running() override;
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
