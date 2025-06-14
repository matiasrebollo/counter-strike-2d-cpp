#include "clickable_label.h"

// This code is essentialy explained in the QT's official documentation
// https://doc.qt.io/qt-6/dnd.html

ClickableLabel::ClickableLabel(const QString& text, QWidget* parent): QLabel(text, parent) {
    setAlignment(Qt::AlignCenter);
    setStyleSheet("border: 1px solid gray;");
    setAcceptDrops(true);
}

void ClickableLabel::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        emit clicked();
    }
}

void ClickableLabel::mouseMoveEvent(QMouseEvent* event) {
    if (!(event->buttons() & Qt::LeftButton))
        return;
    if ((event->pos() - dragStartPosition).manhattanLength() < QApplication::startDragDistance())
        return;
    QDrag* drag = new QDrag(this);
    QMimeData* mimeData = new QMimeData;
    mimeData->setData("application/x-block", QByteArray("block"));
    drag->setMimeData(mimeData);
    drag->exec(Qt::CopyAction | Qt::MoveAction);
}

void ClickableLabel::dragEnterEvent(QDragEnterEvent* event) {
    if (event->mimeData()->hasFormat("application/x-block")) {
        event->acceptProposedAction();
    }
}

void ClickableLabel::dropEvent(QDropEvent* event) {
    emit this->dropped(this->x(), this->y());
    event->acceptProposedAction();
}
