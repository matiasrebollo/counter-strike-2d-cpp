#include "client/lobby/lobby.h"

#include <QFont>
#include <QFontDatabase>
#include <QMessageBox>
#include <iostream>

#include "client/client_protocol.h"
#include "common/commands.h"
#include "common/lobby_request.h"
#include "common/message.h"
#include "common/skins.h"

#include "ui_lobby.h"

#define PATH_CS_FONT "../../../assets/cs_regular.ttf"

Lobby::Lobby(QWidget* parent):
        QMainWindow(parent),
        ui(new Ui::Lobby),
        selected_ct_skin(SEAL_FORCE),
        selected_tt_skin(PHEONIX) {
    ui->setupUi(this);
    ui->stack->setCurrentIndex(0);
    ui->skins_tt_stack->setCurrentIndex(0);
    ui->skins_ct_stack->setCurrentIndex(0);

    connect(ui->backButton, &QPushButton::clicked, this, &Lobby::go_to_lobby);
    connect(ui->backButton_2, &QPushButton::clicked, this, &Lobby::go_to_lobby);
    connect(ui->back_to_lobby3, &QPushButton::clicked, this, &Lobby::go_to_lobby);
    connect(ui->connectButton, &QPushButton::clicked, this, &Lobby::connect_to_sv);
}

Lobby::~Lobby() { delete ui; }

void Lobby::go_to_lobby() { ui->stack->setCurrentIndex(1); }


void Lobby::on_CreateGame_clicked() {
    CreateUsernameDTO request;
    request.username = ui->username->text().toStdString();
    protocol.value().send_lobby_request(request);

    ServerResponseLobby response = protocol.value().receive_command();
    if (response.commandType == CREATE_USERNAME && response.success) {
        ui->stack->setCurrentIndex(3);
        this->username = ui->username->text().toStdString();
    }
}

void Lobby::on_JoinGame_clicked() {
    CreateUsernameDTO request;
    request.username = ui->username->text().toStdString();
    protocol.value().send_lobby_request(request);

    ServerResponseLobby response = protocol.value().receive_command();
    if (response.commandType == CREATE_USERNAME && response.success) {
        ui->stack->setCurrentIndex(2);
        this->username = ui->username->text().toStdString();
    }
}

void Lobby::on_JoinGameButton_clicked() {
    std::string game_name = ui->game_code->text().toStdString();

    JoinGameDTO request;
    request.gamename = game_name;
    request.tt_skin = selected_tt_skin;
    request.ct_skin = selected_ct_skin;

    protocol.value().send_lobby_request(request);
    ServerResponseLobby response = protocol.value().receive_command();
    if (response.commandType == JOIN_GAME && response.success) {
        this->gamecode = game_name;
        close();
    }
}


void Lobby::on_createButton_clicked() {
    bool ok;
    int n_min_players = ui->NPlayers->text().toInt(&ok);
    if (not ok) {
        // error
    } else {
        CreateGameDTO request;
        request.tt_skin = selected_tt_skin;
        request.ct_skin = selected_ct_skin;
        request.size_players = n_min_players;

        protocol.value().send_lobby_request(request);
        ServerResponseLobby response = protocol.value().receive_command();
        if (response.commandType == CREATE_GAME && response.success) {
            this->gamecode = response.game_name;
            QString game_code = QString::fromStdString(response.game_name);
            QMessageBox::information(this, "Codigo de partida", game_code);
            close();
        }
    }
}

void Lobby::connect_to_sv() {
    try {
        QString hostname = ui->Host->text();
        QString port = ui->Port->text();

        protocol.emplace(hostname.toStdString(), port.toStdString());
        go_to_lobby();
    } catch (...) {
        // error
    }
}

ClientProtocol& Lobby::get_protocol() {
    if (protocol.has_value()) {
        return protocol.value();
    }
    throw std::runtime_error("Protocolo no inicializado");
}

void Lobby::on_select_tt_skin_clicked() {
    this->selected_tt_skin = skins_tt[ui->skins_tt_stack->currentIndex()];
}


void Lobby::on_select_ct_skin_clicked() {
    this->selected_ct_skin = skins_ct[ui->skins_ct_stack->currentIndex()];
}

void Lobby::on_next_tt_skin_clicked() {
    int index = ui->skins_tt_stack->currentIndex();
    index = (index + 1) % ui->skins_tt_stack->count();
    ui->skins_tt_stack->setCurrentIndex(index);
}

void Lobby::on_prev_tt_skin_clicked() {
    int index = ui->skins_tt_stack->currentIndex();
    index = (index - 1 + ui->skins_tt_stack->count()) % ui->skins_tt_stack->count();
    ui->skins_tt_stack->setCurrentIndex(index);
}

void Lobby::on_prev_ct_skin_clicked() {
    int index = ui->skins_ct_stack->currentIndex();
    index = (index - 1 + ui->skins_ct_stack->count()) % ui->skins_ct_stack->count();
    ui->skins_ct_stack->setCurrentIndex(index);
}

void Lobby::on_next_ct_skin_clicked() {
    int index = ui->skins_ct_stack->currentIndex();
    index = (index + 1) % ui->skins_ct_stack->count();
    ui->skins_ct_stack->setCurrentIndex(index);
}

void Lobby::on_go_to_select_skin_btn_clicked() { ui->stack->setCurrentIndex(4); }

TerroristSkin& Lobby::get_tt_skin() { return this->selected_tt_skin; }

CounterTerroristSkin& Lobby::get_ct_skin() { return this->selected_ct_skin; }

std::string Lobby::get_username() { return this->username; }

std::string Lobby::get_gamecode() { return this->gamecode; }
