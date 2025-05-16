#ifndef CS2D_GAME_H
#define CS2D_GAME_H

#include <list>
#include <map>
#include <memory>
#include <string>

#include "common/queue.h"
#include "common/thread.h"
#include "server/collidable.h"
#include "server/command.h"
#include "server/player.h"

class CS2DGame: public Thread {
private:
    std::map<std::string, std::shared_ptr<Player>> players;
    std::list<std::shared_ptr<Collidable>> collidables;
    // Queue<std::unique_ptr<Command>> command_queue; // problemas con guardar smart pointers

    void broadcast_map() const;
    void broadcast_snapshot() const;

public:
    CS2DGame();

    void new_player(std::string& username);

    void run() override;

    CS2DGame(const CS2DGame&) = delete;
    CS2DGame& operator=(const CS2DGame&) = delete;

    ~CS2DGame();
};

#endif
