#include <QApplication>
#include <QFontDatabase>

#include "game_editor.h"
#define PATH_CS_FONT "../assets/cs_regular.ttf"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    int fontId = QFontDatabase::addApplicationFont(PATH_CS_FONT);
    if (fontId == -1) {
        qWarning("No se pudo cargar la fuente");
    } else {
        QString family = QFontDatabase::applicationFontFamilies(fontId).at(0);
        QFont font(family, 12);
        a.setFont(font);
    }
    Game_editor w;
    w.show();
    return a.exec();
}
