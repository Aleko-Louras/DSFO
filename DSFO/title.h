#ifndef TITLE_H
#define TITLE_H

#include <QWidget>
#include <Box2D/Box2D.h>
#include <QTimer>
#include <QPainter>
#include <QTransform>

namespace Ui {
class Title;
}

class Title : public QWidget
{
    Q_OBJECT

public:
    explicit Title(QWidget *parent = nullptr);
    ~Title();
    void changeDirection(b2Vec2 position);

public slots:
//               // void setPosition(QRect newPosition);
     void handleTrigger();
//     void setPosition();
     void movePlaneUpDown();

private:
    Ui::Title *ui;
    b2World world;
    b2Body* body;
    QTimer* timer;
    QTimer* timer2;
    QTimer* timerUpDown;
    std::vector<b2Vec2> angles;
    std::vector<QPixmap> images;
    int angleIndex;

};

#endif // TITLE_H
