#include <iostream>

#include "lobby.h"
#include "ui_lobby.h"

#include "../../common/message.h"
#include "../../common/command.h"
#include "../../common/skins.h"
#include "../../client/client_protocol.h"

Lobby::Lobby(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Lobby)
{
    ui->setupUi(this);
    ui->stack->setCurrentIndex(0);
    connect(ui->backButton,&QPushButton::clicked,this,&Lobby::go_to_lobby);
    connect(ui->backButton_2,&QPushButton::clicked,this,&Lobby::go_to_lobby);
    connect(ui->connectButton,&QPushButton::clicked,this,&Lobby::connect_to_sv);
}

Lobby::~Lobby()
{
    delete ui;
}

void Lobby::go_to_lobby()
{
    ui->stack->setCurrentIndex(1);
}



void Lobby::on_CreateGame_clicked()
{
    this->username = ui->username->text().toStdString()
    ui->stack->setCurrentIndex(3);
}


void Lobby::on_JoinGame_clicked()
{
    this->username = ui->username->text().toStdString()
    ui->stack->setCurrentIndex(2);
}

void Lobby::on_JoinGameButton_clicked()
{
    int index = ui->GamesList->currentRow();
    std::string game_name = partidas[index];

    MessageFromClient request;

    request.commandType = CommandType::JOIN_GAME;   // commandType
    request.s = game_name;                  // s
    request.tt_skin = TerroristSkin::GUERRILLA;   // tt_skin
    request.ct_skin = CounterTerroristSkin::GIGN; // ct_skin

    protocol.Send_command(request);
    ServerResponseLobby response = protocol.Receive_command();
    if (response.commandType == JOIN_GAME && response.success){
        close()
    }
}


void Lobby::on_createButton_clicked()
{
    bool ok;
    int n_min_players = ui->NPlayers->text().toInt(&ok);
    if (not ok){
        //error
    }else {
        MessageFromClient request;

        request.commandType = CommandType::CREATE_GAME;   // commandType
        request.tt_skin = TerroristSkin::GUERRILLA;   // tt_skin
        request.ct_skin = CounterTerroristSkin::GIGN; // ct_skin
        request.size_players = n_min_players;

        protocol.Send_command(request);
        ServerResponseLobby response = protocol.Receive_command();
        if (response.commandType == CREATE_GAME && response.success){
            close()
        }
    }
}

void Lobby::connect_to_sv(){
    try{
        QString hostname = ui->Hostname->text();
        QString port = ui->Port->text();
        
        ClientProtocol(hostname.toStdString(), port.toStdString())
        go_to_lobby();
    }catch(...){
        //error
    }
}

