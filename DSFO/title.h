#ifndef TITLE_H
#define TITLE_H

#include <QWidget>
#include <Box2D/Box2D.h>
#include <QTimer>

namespace Ui {
class Title;
}

class Title : public QWidget
{
    Q_OBJECT

public:
    explicit Title(QWidget *parent = nullptr);
    ~Title();
    void changeDirection();

public slots:
              // void setPosition(QRect newPosition);
    void handleTrigger();

private:
    Ui::Title *ui;
    b2World world;
    b2Body* body;
    QTimer* timer;
    QTimer* timer2;
    std::vector<b2Vec2> angles;
    int angleIndex;

};

#endif // TITLE_H
