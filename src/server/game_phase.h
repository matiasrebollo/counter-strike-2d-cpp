#ifndef GAME_PHASE_H
#define GAME_PHASE_H

#include <memory>

#include "common/phase.h"
#include "server/command.h"

#define BUY_PHASE_DURATION 10
#define ATTACK_PHASE_DURATION 60
#define WAITING_PLAYERS_PHASE_DURATION 300
#define BETWEEN_ROUNDS_PHASE_DURATION 5

class CS2DGame;  // Forward declaration

class GamePhase {
protected:
    CS2DGame& game;
    const size_t duration;

public:
    explicit GamePhase(CS2DGame& game, const size_t& duration);
    virtual Phase type() = 0;
    void run();
    virtual bool should_continue() = 0;
    virtual void execute(std::unique_ptr<Command> cmd) = 0;
    virtual void end() = 0;
    virtual ~GamePhase() {}
};

class WaitingPlayersPhase: public GamePhase {
public:
    explicit WaitingPlayersPhase(CS2DGame& game);
    Phase type() override;
    bool should_continue() override;
    void execute(std::unique_ptr<Command> cmd) override;
    void end() override;
};

class BuyPhase: public GamePhase {
public:
    explicit BuyPhase(CS2DGame& game);
    Phase type() override;
    bool should_continue() override;
    void execute(std::unique_ptr<Command> cmd) override;
    void end() override;
};

class AttackPhase: public GamePhase {
public:
    explicit AttackPhase(CS2DGame& game);
    Phase type() override;
    bool should_continue() override;
    void execute(std::unique_ptr<Command> cmd) override;
    void end() override;
};

class BetweenRoundsPhase: public GamePhase {
public:
    explicit BetweenRoundsPhase(CS2DGame& game);
    Phase type() override;
    bool should_continue() override;
    void execute(std::unique_ptr<Command> cmd) override;
    void end() override;
};

// Ended ???

#endif
