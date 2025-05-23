#include "client/lobby/lobby.h"

#include <QMessageBox>
#include <iostream>

#include "client/client_protocol.h"
#include "common/commands.h"
#include "common/lobby_request.h"
#include "common/message.h"
#include "common/skins.h"

#include "ui_lobby.h"

Lobby::Lobby(QWidget* parent): QMainWindow(parent), ui(new Ui::Lobby) {
    ui->setupUi(this);
    ui->stack->setCurrentIndex(0);
    connect(ui->backButton, &QPushButton::clicked, this, &Lobby::go_to_lobby);
    connect(ui->backButton_2, &QPushButton::clicked, this, &Lobby::go_to_lobby);
    connect(ui->connectButton, &QPushButton::clicked, this, &Lobby::connect_to_sv);
}

Lobby::~Lobby() { delete ui; }

void Lobby::go_to_lobby() { ui->stack->setCurrentIndex(1); }


void Lobby::on_CreateGame_clicked() {
    CreateUsernameDTO request;
    request.username = ui->lineEdit->text().toStdString();
    protocol.value().send_lobby_request(request);

    ServerResponseLobby response = protocol.value().receive_command();
    if (response.commandType == CREATE_USERNAME && response.success) {
        ui->stack->setCurrentIndex(3);
        this->username = ui->lineEdit->text().toStdString();
    }
}

void Lobby::on_JoinGame_clicked() {
    CreateUsernameDTO request;
    request.username = ui->lineEdit->text().toStdString();
    protocol.value().send_lobby_request(request);

    ServerResponseLobby response = protocol.value().receive_command();
    if (response.commandType == CREATE_USERNAME && response.success) {
        ui->stack->setCurrentIndex(2);
        this->username = ui->lineEdit->text().toStdString();
    }
}

void Lobby::on_JoinGameButton_clicked() {
    std::string game_name = ui->game_code->text().toStdString();

    JoinGameDTO request;

    request.gamename = game_name;
    request.tt_skin = TerroristSkin::GUERRILLA;
    request.ct_skin = CounterTerroristSkin::GIGN;

    protocol.value().send_lobby_request(request);
    ServerResponseLobby response = protocol.value().receive_command();
    if (response.commandType == JOIN_GAME && response.success) {
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

        request.tt_skin = TerroristSkin::GUERRILLA;
        request.ct_skin = CounterTerroristSkin::GIGN;
        request.size_players = n_min_players;

        protocol.value().send_lobby_request(request);
        ServerResponseLobby response = protocol.value().receive_command();
        if (response.commandType == CREATE_GAME && response.success) {
            QString game_code = QString::fromStdString(response.game_name);
            QMessageBox::information(this, "Codigo de partida", game_code);
            close();
        }
    }
}

void Lobby::connect_to_sv() {
    try {
        QString hostname = ui->Server->text();
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

std::string Lobby::get_username() { return this->username; }
