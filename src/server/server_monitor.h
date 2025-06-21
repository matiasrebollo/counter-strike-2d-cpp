#ifndef SERVER_MONITOR_H
#define SERVER_MONITOR_H

#include <memory>
#include <mutex>
#include <set>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "../common/game_snapshot.h"
#include "../common/queue.h"

#include "client_sender.h"
#include "cs2d_game.h"
#include "game_dead_exception.h"

#define MSG_GAME_JOINED(id) "Game of id " + (id) + " has been joined!"

/*
    This class is the owner of the shared resources of the server. It's methods are protected with
    a mutex to avoid RC.
*/
class ServerMonitor {
private:
    std::mutex mutex;
    std::set<std::string> players;
    std::unordered_map<std::string, std::shared_ptr<CS2DGame>> games;
    int game_id;

public:
    ServerMonitor();
    /*
        Register a new user in the server. Only if your username is a non empty string and
        nobody is registered with that name it will return true (usernames are unique).
    */
    bool create_username(const std::string& username);
    /*
        Register a new game in a specify map. If the map doesnt exist it will crash.
        If the game is created succesfully, it will add the game's creator and it will
        stablish the relationship game - sender thread.
    */
    std::shared_ptr<CS2DGame> create_new_game(const std::string& username,
                                              const std::string& map_filename,
                                              std::shared_ptr<ClientSender> sender);
    /*
        Join to an existent game. If the game doesn't exist, it returns a nullptr.
        If the game is dead (is still at the game's map but nobody is playing), it will throw an
        exception, later cached in the client handler, who will send through the socket an
       appropiate message. If you join the game succesfully, the game will save the sender thread.
    */
    std::shared_ptr<CS2DGame> join_game(const std::string& gameName, const std::string& username,
                                        std::shared_ptr<ClientSender> sender);
    /*
        If somebody leaves the server (have closed the client program or his game have ended),
       delete the username. Its also called if somebody register a username valid because somebody
       had left the server, but its still in a current game. In this particular case, you could
       change your name.
    */
    void delete_username(const std::string& username);
    /*
        Return a particular game its exist in the server.
    */
    CS2DGame& get_game(const std::string& gameName);
    /*
        Delete a particular game name.
    */
    void manage_end_game(const std::string& gameName);
    /*
        Join a game thread if it had ended and delete it from the map of games too.
    */
    void reap_games();
    /*
        Similar to reap but stop and join all the current games for kill the server.
    */
    void kill_games();
};

#endif
