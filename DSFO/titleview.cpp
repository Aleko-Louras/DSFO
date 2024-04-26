/**
cpp file for the title view, which is where the title is actually displayed in the graphical interface.
University of Utah CS 3505 final project, group LAQE
Writen by Lucas Pearce, Ethan Block, Will Black, Quinn Pritchett, Aleko Louras
**/

#include "titleview.h"
#include <QResizeEvent>
#include <QGraphicsPixmapItem>
#include <QTimer>

TitleView::TitleView(QWidget *parent) : QGraphicsView(parent), world(b2Vec2(0.0f, 0.0f))
{
    // ESSENTIAL SETTINGS!
    setMinimumSize(500, 400);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QRectF sceneBox(0, 0, width(), height());
    QImage planeImage = QImage(":/images/image.png").scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation).mirrored(true, false);

    titleScene = new QGraphicsScene(sceneBox, this);
    plane = titleScene->addPixmap(QPixmap::fromImage(planeImage));

    QGraphicsTextItem *title = titleScene->addText("Welcome to DSFO!", QFont("Arial Rounded MT Bold", 30));
    title->setPos(sceneBox.center().x() - title->boundingRect().width() / 2, sceneBox.center().y() - title->boundingRect().height() / 2);

    //Add a label to give information about the game
    QLabel *label = new QLabel("This is an educational game that teaches users about stacks and graphs. If you are confused, click the 'Menu' button and then click 'More Information' to learn more. Enjoy!");
    label->setWordWrap(true);
    label->setAlignment(Qt::AlignCenter);
    titleScene->addWidget(label);
    //Set the label at the bottom
    label->setGeometry(0, 300, 500, 100);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &TitleView::movePlane);
    timer->start(timeStep * 1000);

    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, -10.0f);
    b2Body* groundBody = world.CreateBody(&groundBodyDef);
    b2PolygonShape groundBox;
    groundBox.SetAsBox(50.0f, 3.0f);
    groundBody->CreateFixture(&groundBox, -5.0f);
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(0.0f, -1.5f);
    body = world.CreateBody(&bodyDef);
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(1.0f, 1.0f);
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    body->CreateFixture(&fixtureDef);

    // apply right impulse, but only if max velocity is not reached yet
    body->ApplyForce( b2Vec2(-1000.0f, 0), body->GetWorldCenter(), true);

    setScene(titleScene);
}

void TitleView::movePlane()
{
    world.Step(timeStep, velocityIterations, positionIterations);
    b2Vec2 position = body->GetPosition();

    // // Convert position from meters to pixels
    float xPosPixels = position.x * pixelsPerMeter;
    float yPosPixels = position.y * pixelsPerMeter;

    int widgetWidth = this->width();
    int planeWidth = plane->boundingRect().width();

    const int wrapWidth = widgetWidth + planeWidth; // Total width for wrapping

    float wrappedXPos = std::fmod(xPosPixels, wrapWidth);

    if (wrappedXPos < 0)
        wrappedXPos += wrapWidth;

    plane->setPos(wrappedXPos - planeWidth, -yPosPixels);

    if (yPosPixels >= maxYPos)
        // If at the maximum y position, move it down
        body->ApplyForce(b2Vec2(0.0f, -5.0f), body->GetWorldCenter(), true);
    else if (yPosPixels <= minYPos)
        // If at the minimum y position, move it up
        body->ApplyForce(b2Vec2(0.0f, 5.0f), body->GetWorldCenter(), true);
}

TitleView::~TitleView()
{
    delete titleScene;
    delete plane;
}

void TitleView::resizeEvent(QResizeEvent *event)
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
