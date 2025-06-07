#include <QApplication>

#include "game_editor.h"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    Game_editor w;
    w.show();
    return a.exec();
}
