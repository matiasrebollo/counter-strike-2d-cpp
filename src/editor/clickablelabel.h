#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

#include <QLabel>
#include <QMouseEvent>

class ClickableLabel: public QLabel {
    Q_OBJECT

public:
    explicit ClickableLabel(const QString& text = "", QWidget* parent = nullptr):
            QLabel(text, parent) {
        setAlignment(Qt::AlignCenter);
        setStyleSheet("border: 1px solid gray;");
    }

signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent*) override { emit clicked(); }
};

#endif  // CLICKABLELABEL_H
