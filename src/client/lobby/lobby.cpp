#include "client/lobby/lobby.h"

#include <QFont>
#include <QFontDatabase>
#include <QMessageBox>
#include <filesystem>

#define MAP_PATH "../maps"

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
    connect(ui->backButton2, &QPushButton::clicked, this, &Lobby::go_to_lobby);
    connect(ui->back_to_lobby3, &QPushButton::clicked, this, &Lobby::go_to_lobby);
    connect(ui->connectButton, &QPushButton::clicked, this, &Lobby::connect_to_sv);
}

Lobby::~Lobby() { delete ui; }

void Lobby::go_to_lobby() { ui->stack->setCurrentIndex(1); }


void Lobby::on_CreateGame_clicked() {
    if (this->username != "") {
        this->create_game();
        return;
    }
    CreateUsernameDTO request;
    request.username = ui->username->text().toStdString();
    if (request.username == "") {
        QMessageBox::information(this, TITLE_MSG_CREATE, MSG_NO_USERNAME);
        return;
    }

    protocol.value().send_lobby_request(request);

    ServerResponseLobby response = protocol.value().receive_command();
    if (response.success) {
        this->username = ui->username->text().toStdString();
    } else {
        QMessageBox::information(this, TITLE_MSG_CREATE, MSG_USERNAME_ALREADY_USED);
        return;
    }

    this->create_game();
}

void Lobby::create_game() {
    ui->stack->setCurrentIndex(2);
    ui->message->clear();
    ui->maps_list->clear();

    for (const auto& entry: std::filesystem::directory_iterator(MAP_PATH)) {
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

    protocol.value().send_lobby_request(second_request);
    ServerResponseLobby response = protocol.value().receive_command();
    if (response.success) {
        this->gamecode = response.game_name;
        close();
    } else {
        QMessageBox::information(this, TITLE_MSG_CREATE, MSG_GAME_NOT_CREATED);
    }
}

void Lobby::on_JoinGame_clicked() {
    if (this->username != "") {
        ui->stack->setCurrentIndex(3);
        return;
    }
    CreateUsernameDTO request;
    request.username = ui->username->text().toStdString();
    if (request.username == "") {
        QMessageBox::information(this, TITLE_MSG_JOIN, MSG_NO_USERNAME);
        return;
    }
    protocol.value().send_lobby_request(request);

    ServerResponseLobby response = protocol.value().receive_command();
    if (response.success) {
        ui->stack->setCurrentIndex(3);
        this->username = ui->username->text().toStdString();
    } else {
        QMessageBox::information(this, TITLE_MSG_JOIN, MSG_USERNAME_ALREADY_USED);
    }
}

void Lobby::on_JoinGameButton_clicked() {
    std::string game_name = ui->game_code->text().toStdString();

    JoinGameDTO request = {game_name};

    protocol.value().send_lobby_request(request);
    ServerResponseLobby response = protocol.value().receive_command();
    if (response.success) {
        this->gamecode = game_name;
        close();
    } else {
        QMessageBox::information(this, TITLE_MSG_JOIN, MSG_GAME_ALREADY_STARTED);
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
    if (!protocol.has_value()) {
        throw std::runtime_error(MSG_NO_PROTOCOL);
    }
    return protocol.value();
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
