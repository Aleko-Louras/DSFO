#ifndef ENDSCREEN_H
#define ENDSCREEN_H

#include <QWidget>
#include <Box2D/Box2D.h>
#include <QTimer>
#include <QPainter>
#include <QTransform>

namespace Ui {
class EndScreen;
}

class EndScreen : public QWidget
{
    Q_OBJECT

public:
    explicit EndScreen(QWidget *parent = nullptr);
    ~EndScreen();

public slots:
    void handleTrigger();

private:
    Ui::EndScreen *ui;
    b2World world;
    b2Body* body;
    QTimer* timer;

};

#endif // ENDSCREEN_H
