#ifndef CS2D_GAME_H
#define CS2D_GAME_H

#include <list>
#include <map>
#include <memory>
#include <string>

#include "common/game_snapshot.h"
#include "common/message.h"
#include "common/queue.h"
#include "common/thread.h"
#include "server/client_sender.h"
#include "server/collidable.h"
#include "server/command.h"
#include "server/player.h"
#include "server/shot.h"

class CS2DGame: public Thread {
private:
    std::map<std::string, std::shared_ptr<Player>> players;
    std::map<std::string, std::shared_ptr<Queue<Snapshot>>> player_queues;
    std::list<std::shared_ptr<Collidable>> collidables;
    Queue<MessageFromClient> command_queue;

    void broadcast_map() const;
    void broadcast_snapshot();

    // devuelve la distancia del objeto con el que impactó o 0 si no impactó.
    double impacts(const Shot& shot, const Collidable& collidable) const;
    double intersects_segment(const Shot& shot, const Vector2D& seg_start,
                              const Vector2D& seg_end) const;

public:
    const std::string id;
    explicit CS2DGame(const std::string& id);

    void push(const MessageFromClient& command);
    void new_player(const std::string& username, ClientSender& sender);
    void move_player(const std::string& username, const Vector2D& direction);
    bool is_player_in_valid_position(const Player& player) const;
    void rotate_player(const std::string& username, const Vector2D& direction);
    void shoot(const std::string& username);
    const Collidable* first_impact(const Shot& shot) const;

    void run() override;

    CS2DGame(const CS2DGame&) = delete;
    CS2DGame& operator=(const CS2DGame&) = delete;

    ~CS2DGame() override;
};

#endif
