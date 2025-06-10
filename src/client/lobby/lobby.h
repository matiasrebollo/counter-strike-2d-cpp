#ifndef LOBBY_H
#define LOBBY_H

#include <QMainWindow>
#include <optional>
#include <string>
#include <vector>

#include "../../common/skins.h"
#include "client/client_protocol.h"
#include "common/settings.h"

#define TITLE_MSG_CREATE "Crear una partida"
#define TITLE_MSG_JOIN "Unirse a una partida"
#define MSG_NO_USERNAME "Ingrese un nombre de usuario para registrarse."
#define MSG_USERNAME_ALREADY_USED "Ya existe un usuario con ese nombre. Ingresa uno distinto."
#define MSG_GAME_ALREADY_STARTED "La partida ya ha comenzado o no existe."
#define MSG_GAME_NOT_CREATED "El servidor no ha podido crear su partida."
#define MSG_MAP_NOT_SELECTED "Debe seleccionar un mapa para crear una partida."

#define MSG_NO_PROTOCOL "Debes conectarte a un servidor disponible para poder jugar."
#define BASH_MSG_NO_USERNAME "Debes tener un nombre de usuario único para poder jugar."
#define MSG_NO_GAME "Tenes que unirte a un juego o crear uno para jugar!"

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
    ClientProtocol&& get_protocol();
    TerroristSkin& get_tt_skin();
    CounterTerroristSkin& get_ct_skin();
    std::string get_username();
    std::string get_gamecode();

private slots:
    void on_CreateGame_clicked();

    void on_JoinGame_clicked();

    void go_to_lobby();

    void on_JoinGameButton_clicked();

    void connect_to_sv();

    void on_select_tt_skin_clicked();

    void on_select_ct_skin_clicked();

    void on_next_tt_skin_clicked();

    void on_prev_tt_skin_clicked();

    void on_prev_ct_skin_clicked();

    void on_next_ct_skin_clicked();

    void on_go_to_select_skin_btn_clicked();

    void on_CreateGameButton_clicked();

private:
    Ui::Lobby* ui;
    CounterTerroristSkin selected_ct_skin;
    TerroristSkin selected_tt_skin;
    std::vector<std::string> partidas;
    std::string username;
    std::string gamecode;
    std::optional<ClientProtocol> protocol;

    void create_game();
    void format_string(std::string& s);

    enum TerroristSkin skins_tt[4] = {PHEONIX, L3337_KREW, ARTIC_AVENGER, GUERRILLA};
    enum CounterTerroristSkin skins_ct[4] = {SEAL_FORCE, GSG_9, UK_SAS, GIGN};
};
#endif  // LOBBY_H
