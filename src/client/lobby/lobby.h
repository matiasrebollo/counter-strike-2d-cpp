#ifndef LOBBY_H
#define LOBBY_H

#include <QMainWindow>
#include <vector>
#include <string>
#include <optional>

#include "../../client/client_protocol.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Lobby;
}
QT_END_NAMESPACE

class Lobby : public QMainWindow
{
    Q_OBJECT

public:
    Lobby(QWidget *parent = nullptr);
    ~Lobby();

private slots:
    void on_CreateGame_clicked();

    void on_JoinGame_clicked();

    void go_to_lobby();

    void on_JoinGameButton_clicked();

    void on_createButton_clicked();

    void connect_to_sv();

private:
    Ui::Lobby *ui;
    std::vector<std::string> partidas;
    std::string username;
    std::optional<ClientProtocol> protocol;
};
#endif // LOBBY_H
