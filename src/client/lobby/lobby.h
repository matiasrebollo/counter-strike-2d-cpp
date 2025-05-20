#ifndef LOBBY_H
#define LOBBY_H

#include <QMainWindow>
#include <optional>
#include <string>
#include <vector>

#include <QtCore/QObject>

#include "client/client_protocol.h"

class LobbyUI;

class Lobby: public QMainWindow {
    Q_OBJECT

public:
    explicit Lobby(QWidget* parent = nullptr);
    ~Lobby();
    ClientProtocol& get_protocol();

    // cppcheck-suppress unknownMacro
private Q_SLOTS:
    void on_CreateGame_clicked();
    void on_JoinGame_clicked();
    void go_to_lobby();
    void on_JoinGameButton_clicked();
    void on_createButton_clicked();
    void connect_to_sv();

private:
    LobbyUI* ui;
    std::vector<std::string> partidas;
    std::string username;
    std::optional<ClientProtocol> protocol;
};

#endif  // LOBBY_H
