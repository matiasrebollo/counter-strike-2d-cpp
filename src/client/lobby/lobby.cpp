#include "client/lobby/lobby.h"

#include <QFileInfo>
#include <QFont>
#include <QFontDatabase>
#include <QMessageBox>
#include <filesystem>
#include <iostream>
#include <memory>
#include <utility>

#include "client/client_protocol.h"
#include "common/commands.h"
#include "common/communication_ended.h"
#include "common/lobby_request.h"
#include "common/message.h"
#include "common/skins.h"

#include "ui_lobby.h"

#ifdef TESTS
#include "common/mock_socket.h"
using Socket = MockSocket;
#else
#include "common/socket.h"
using Socket = RealSocket;
#endif

#define PATH_CS_FONT "../../../assets/cs_regular.ttf"
#define TITLE_NO_SV "Fallo de conexion"
#define MSG_NO_SV "No hay un servidor en la direccion solicidada"
#define TITLE_SV_CLOSED "Servidor Desconectado"
#define MSG_SV_CLOSED "Se ha perdido la conexion con el servidor"

Lobby::Lobby(QWidget* parent):
        QMainWindow(parent),
        ui(new Ui::Lobby),
        selected_ct_skin(SEAL_FORCE),
        selected_tt_skin(PHEONIX),
        username(""),
        gamecode(""),
        is_creator_(false),
        can_change_name(false) {
    ui->setupUi(this);
    ui->stack->setCurrentIndex(0);
    ui->skins_tt_stack->setCurrentIndex(0);
    ui->skins_ct_stack->setCurrentIndex(0);

    connect(ui->backButton, &QPushButton::clicked, this, &Lobby::go_to_lobby);
    connect(ui->backButton2, &QPushButton::clicked, this, &Lobby::go_to_lobby);
    connect(ui->back_to_lobby3, &QPushButton::clicked, this, &Lobby::go_to_lobby);
    connect(ui->connectButton, &QPushButton::clicked, this, &Lobby::connect_to_sv);

    this->sound_player = new QMediaPlayer(this);
    QString relativePath = "../src/client/lobby/cs_music.mp3";
    QString absolutePath = QFileInfo(relativePath).absoluteFilePath();
    sound_player->setMedia(QUrl::fromLocalFile(absolutePath));
    sound_player->setVolume(10);
}

Lobby::~Lobby() { delete ui; }

void Lobby::go_to_lobby() {
    this->sound_player->play();
    ui->stack->setCurrentIndex(1);
}


void Lobby::on_CreateGame_clicked() {
    if (this->username != "" && !this->can_change_name) {
        this->create_game();
        return;
    }
    CreateUsernameDTO request;
    request.username = ui->username->text().toStdString();
    if (request.username == "") {
        QMessageBox::information(this, TITLE_MSG_CREATE, MSG_NO_USERNAME);
        return;
    }

    this->tryLobbyRequest([this, request]() {
        protocol.value().send_lobby_request(request);

        ServerResponseLobby response = protocol.value().receive_server_response_lobby();
        if (response.status == ResponseStatus::SUCCESS) {
            this->username = ui->username->text().toStdString();
            this->can_change_name = false;
        } else {
            QMessageBox::information(this, TITLE_MSG_CREATE, MSG_USERNAME_ALREADY_USED);
            return;
        }

        this->create_game();
    });
}

void Lobby::create_game() {
    ui->stack->setCurrentIndex(2);
    ui->message->clear();
    ui->maps_list->clear();

    for (const auto& entry: std::filesystem::directory_iterator(PATH_FOLDER_MAPS)) {
        if (entry.is_regular_file()) {
            std::string name = entry.path().filename().string();
            this->format_string(name);
            QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(name));
            item->setTextAlignment(Qt::AlignCenter);
            ui->maps_list->addItem(item);
        }
    }
    if (ui->maps_list->count() == 0) {
        ui->message->setText("Tenes que crear algún mapa para jugar");
    }
}

void Lobby::format_string(std::string& s) { s.erase(s.length() - 5); }

void Lobby::on_CreateGameButton_clicked() {
    if (not(ui->maps_list->currentIndex().isValid())) {
        QMessageBox::information(this, TITLE_MSG_CREATE, MSG_MAP_NOT_SELECTED);
        return;
    }

    std::string map_name = ui->maps_list->currentItem()->text().toStdString();
    CreateGameDTO second_request = {map_name};

    this->tryLobbyRequest([this, second_request]() {
        protocol.value().send_lobby_request(second_request);
        ServerResponseLobby response = protocol.value().receive_server_response_lobby();
        if (response.status == ResponseStatus::SUCCESS) {
            this->gamecode = response.game_name;
            this->is_creator_ = true;
            this->sound_player->stop();
            close();
        } else if (response.status == ResponseStatus::GAME_NOT_CREATED) {
            QMessageBox::information(this, TITLE_MSG_CREATE, MSG_GAME_NOT_CREATED);
        }
    });
}

void Lobby::on_JoinGame_clicked() {
    if (this->username != "" && !this->can_change_name) {
        ui->stack->setCurrentIndex(3);
        return;
    }
    CreateUsernameDTO request;
    request.username = ui->username->text().toStdString();
    if (request.username == "") {
        QMessageBox::information(this, TITLE_MSG_JOIN, MSG_NO_USERNAME);
        return;
    }
    this->tryLobbyRequest([this, request]() {
        protocol.value().send_lobby_request(request);
        ServerResponseLobby response = protocol.value().receive_server_response_lobby();
        if (response.status == ResponseStatus::SUCCESS) {
            ui->stack->setCurrentIndex(3);
            this->username = ui->username->text().toStdString();
            this->can_change_name = false;
        } else if (response.status == ResponseStatus::USERNAME_IN_USE) {
            QMessageBox::information(this, TITLE_MSG_JOIN, MSG_USERNAME_ALREADY_USED);
        }
    });
}

void Lobby::on_JoinGameButton_clicked() {
    std::string game_name = ui->game_code->text().toStdString();

    JoinGameDTO request = {game_name};

    this->tryLobbyRequest([this, request, game_name]() {
        protocol.value().send_lobby_request(request);
        ServerResponseLobby response = protocol.value().receive_server_response_lobby();
        switch (response.status) {
            case ResponseStatus::GAME_NOT_EXIST:
                QMessageBox::information(this, TITLE_MSG_JOIN,
                                         QString::fromStdString(MSG_GAME_NOT_EXIST(game_name)));
                break;

            case ResponseStatus::SUCCESS:
                this->gamecode = game_name;
                this->sound_player->stop();
                close();
                break;

            case ResponseStatus::GAME_IS_DEAD:
                QMessageBox::information(this, TITLE_MSG_JOIN, MSG_GAME_IS_DEAD);
                break;

            case ResponseStatus::GAME_IS_FULL:
                QMessageBox::information(this, TITLE_MSG_JOIN,
                                         QString::fromStdString(MSG_GAME_IS_FULL(game_name)));
                break;

            case ResponseStatus::USERNAME_ALREADY_IN_GAME:
                this->can_change_name = true;
                QMessageBox::information(this, TITLE_MSG_JOIN, MSG_USERNAME_ALREADY_USED_IN_GAME);
                break;

            default:
                QMessageBox::information(this, TITLE_MSG_JOIN, MSG_UNEXPECTED_SERVER_RESPONSE);
                break;
        }
    });
}

void Lobby::connect_to_sv() {
    try {
        QString hostname = ui->Host->text();
        QString port = ui->Port->text();

        const std::string hostname_str = hostname.toStdString();
        const std::string port_str = port.toStdString();

        auto socket = std::make_unique<Socket>(hostname_str.c_str(), port_str.c_str());
        protocol.emplace(std::move(socket));
        go_to_lobby();
    } catch (...) {
        QMessageBox::critical(this, TITLE_NO_SV, MSG_NO_SV);
    }
}

ClientProtocol&& Lobby::get_protocol() {
    if (!protocol.has_value()) {
        throw std::runtime_error(MSG_NO_PROTOCOL);
    }
    return std::move(protocol.value());
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

std::string Lobby::get_username() {
    if (this->username == "") {
        throw std::runtime_error(BASH_MSG_NO_USERNAME);
    }
    return this->username;
}

std::string Lobby::get_gamecode() {
    if (this->gamecode == "") {
        throw std::runtime_error(MSG_NO_GAME);
    }
    return this->gamecode;
}

bool Lobby::is_creator() { return is_creator_; }

void Lobby::tryLobbyRequest(const std::function<void()>& func) {
    try {
        func();
    } catch (const CommunicationEnded& e) {
        QMessageBox::critical(this, TITLE_SV_CLOSED, MSG_SV_CLOSED);
        this->sound_player->stop();
        this->close();
    }
}
