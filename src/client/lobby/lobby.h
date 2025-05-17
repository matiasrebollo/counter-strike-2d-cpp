#ifndef LOBBY_H
#define LOBBY_H

#include <QMainWindow>
#include <optional>
#include <string>
#include <vector>

#include "client/client_protocol.h"

// cppcheck-suppress unknownMacro
QT_BEGIN_NAMESPACE
namespace Ui {
class Lobby;
}
QT_END_NAMESPACE

class Lobby: public QMainWindow {
    Q_OBJECT

public:
    explicit Lobby(QWidget* parent = nullptr);
    ~Lobby();
    ClientProtocol& get_protocol();

private slots:  // cppcheck-suppress unknownMacro
    void on_CreateGame_clicked();

    void on_JoinGame_clicked();

    void go_to_lobby();

    void on_JoinGameButton_clicked();

    void on_createButton_clicked();

    void connect_to_sv();

    void wait_to_start();

private:
    Ui::Lobby* ui;
    std::vector<std::string> partidas;
    std::string username;
    std::optional<ClientProtocol> protocol;
};
#endif  // LOBBY_H
