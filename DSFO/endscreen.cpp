#include "endscreen.h"
#include <QResizeEvent>
#include <QGraphicsPixmapItem>

EndScreen::EndScreen(QWidget *parent) : QGraphicsView(parent), world(b2Vec2(0.0f, 0.0f))
{
    // ESSENTIAL SETTINGS!
    setMinimumSize(500, 400);//would love to move it to the right to center it but havent found a way to make it work.
    //setAlignment(Qt::AlignCenter);

    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QRectF sceneBox(0, 0, width(), height());
    QImage planeImage = QImage(":/images/firstPlane.png").scaled(90, 90, Qt::KeepAspectRatio, Qt::SmoothTransformation).convertToFormat(QImage::Format_Mono);

    titleScene = new QGraphicsScene(sceneBox, this);
    plane = titleScene->addPixmap(QPixmap::fromImage(planeImage));
    titleScene->setSceneRect(QRectF(-125, 0, width(), height()));
    QString text = "We are ready for take off!\n\nYour score was: " + QString::number(userScore);
    title = titleScene->addText(text, QFont("Arial Rounded MT Bold", 30));
    title->setPos(sceneBox.center().x() - title->boundingRect().width() / 2, sceneBox.center().y() - title->boundingRect().height() / 2);
    titleScene->setBackgroundBrush(Qt::darkCyan);

    setScene(titleScene);
}

void EndScreen::movePlane(){
    world.Step(timeStep, velocityIterations, positionIterations);
    b2Vec2 position = body->GetPosition();
    plane->setPos(position.x, position.y);

}

EndScreen::~EndScreen()
{
    delete titleScene;
    delete plane;
    delete title;
    delete timer;
}
void EndScreen::triggerAnimation(int userScore){



    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &EndScreen::movePlane);
    timer->start(timeStep * 1000);

    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, 30.0f);
    b2Body* groundBody = world.CreateBody(&groundBodyDef);
    b2PolygonShape groundBox;
    groundBox.SetAsBox(50.0f, -30.0f);
    groundBody->CreateFixture(&groundBox, -5.0f);
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(560.0f, 600.0f);
    body = world.CreateBody(&bodyDef);
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(1.0f, 1.0f);
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    body->CreateFixture(&fixtureDef);
    // apply right impulse, but only if max velocity is not reached yet
    body->ApplyForce( b2Vec2(0, -12000.0f), body->GetWorldCenter(), true);
    title->setPlainText("We are ready for take off!\n\nYour score was: " + QString::number(userScore));
}

void EndScreen::resizeEvent(QResizeEvent *event)
{
    QWidget *parent = parentWidget();
    if (!parent) return;

    int newWidth = event->size().width();
    int newHeight = event->size().height();
    // Ensures that the width and height don't violate the aspect ratio.
    if (newWidth < newHeight * aspectRatio)
        resize(newWidth, newWidth / aspectRatio);
    else resize(newHeight * aspectRatio, newHeight);

    // Fairly straightforward: scales the QGraphicsScene
    fitInView(titleScene->sceneRect(), Qt::KeepAspectRatio);

    // Centers StackView inside its parent widget
    int x = (parent->width() - width()) / 2;
    int y = (parent->height() - height()) / 2;
    move(x,y);
}
