#ifndef CLICKABLE_LABEL_H
#define CLICKABLE_LABEL_H

#include <QApplication>
#include <QDrag>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QLabel>
#include <QMimeData>
#include <QMouseEvent>
#include <QPoint>

class ClickableLabel: public QLabel {
    Q_OBJECT

public:
    explicit ClickableLabel(const QString& text = "", QWidget* parent = nullptr);

signals:
    void left_clicked();
    void right_clicked();
    void dropped(int x, int y);
    void double_click_left();
    void double_click_right();

protected:
    void mousePressEvent(QMouseEvent*) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dropEvent(QDropEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;

private:
    QPoint dragStartPosition;
};

#endif
