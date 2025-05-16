#include "lobby.h"
#include "./ui_lobby.h"
#include <iostream>

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
    ui->stack->setCurrentIndex(3);
}


void Lobby::on_JoinGame_clicked()
{
    ui->stack->setCurrentIndex(2);
    //partidas = protocolo.recvPartidas()
    partidas = {"PArtida1", "partida2", "paartida3"};
    ui->GamesList->clear();
    for (const auto &partida : partidas){
        ui->GamesList->addItem(QString::fromStdString(partida));
    }
}

void Lobby::on_JoinGameButton_clicked()
{
    //unirse a un juego
    int index = ui->GamesList->currentRow();
    std::string partida = partidas[index];
    std::cout << partida;
    /*if (protoclo.unirsePartida(abc.asd.ads)){
        close();
    }*/
}


void Lobby::on_createButton_clicked()
{
    QString game_name = ui->GameName->text();
    QString nro_jugadores_minimo = ui->NPlayers->text();
    /*if (protoclo.crearPartida(abc.asd.ads)){
        close();
    }*/
}

void Lobby::connect_to_sv(){
    try{
        QString servname = ui->Server->text();
        QString port = ui->Port->text();
        //instanciar protocolo(hostname, port)
        go_to_lobby();
    }catch(...){
        //error
    }
}

