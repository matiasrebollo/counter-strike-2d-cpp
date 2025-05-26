#ifndef LOBBY_H
#define LOBBY_H

#include <QMainWindow>
#include <optional>
#include <string>
#include <vector>

#include "../../common/skins.h"
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
    TerroristSkin& get_tt_skin();
    CounterTerroristSkin& get_ct_skin();
    std::string get_username();
    std::string get_gamecode();

private slots:
    void on_CreateGame_clicked();

    void on_JoinGame_clicked();

    void go_to_lobby();

    void on_JoinGameButton_clicked();

    void on_createButton_clicked();

    void connect_to_sv();

    void on_select_tt_skin_clicked();

    void on_select_ct_skin_clicked();

    void on_next_tt_skin_clicked();

    void on_prev_tt_skin_clicked();

    void on_prev_ct_skin_clicked();

    void on_next_ct_skin_clicked();

    void on_go_to_select_skin_btn_clicked();

private:
    Ui::Lobby* ui;
    CounterTerroristSkin selected_ct_skin;
    TerroristSkin selected_tt_skin;
    std::vector<std::string> partidas;
    std::string username;
    std::string gamecode;
    std::optional<ClientProtocol> protocol;

    enum TerroristSkin skins_tt[4] = {ARTIC_AVENGER, PHEONIX, L3337_KREW, GUERRILLA};
    enum CounterTerroristSkin skins_ct[4] = {SEAL_FORCE, UK_SAS, GSG_9, GIGN};
};
#endif  // LOBBY_H
