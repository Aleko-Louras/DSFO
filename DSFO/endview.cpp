#include "endview.h"


EndView::EndView(QWidget *parent) : QGraphicsView(parent), world(b2Vec2(0.0f, 0.0f))
{
    // QGraphicsScene structure settings including layout, initializing the size and location of the scene,
    // and creating the pixmap image of a plane png along with the space for the title and the text itself.
    setMinimumSize(500, 400);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QRectF sceneBox(0, 0, width(), height());
    QImage planeImage = QImage(":/images/firstPlane.png").scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    titleScene = new QGraphicsScene(sceneBox, this);
    plane = titleScene->addPixmap(QPixmap::fromImage(planeImage));
    titleScene->setSceneRect(QRectF(0, 0, width(), height()));
    QString text = "We are ready for take off!\n\nYour score was: " + QString::number(userScore);
    title = titleScene->addText(text, QFont("Arial Rounded MT Bold", 20));
    title->setPos(sceneBox.center().x() - title->boundingRect().width() / 2, sceneBox.center().y() - title->boundingRect().height() / 2);

    //set the scene
    setScene(titleScene);
}

void EndView::movePlane(){

    //from the world, we get the location of the plane body in the engine world,
    // and set the position of the plane png to be body position.
    world.Step(timeStep, velocityIterations, positionIterations);
    b2Vec2 position = body->GetPosition();
    plane->setPos(position.x, position.y);
    titleScene->update();
}

EndView::~EndView()
{
    delete titleScene;
    delete plane;
    delete title;
    delete timer;
}

void EndView::triggerAnimation(int userScore)
{
    //create the timer that will update the planes location every timeStep
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &EndView::movePlane);
    timer->start(timeStep * 1000);
    //Setup for the ground
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, 30.0f);
    b2Body* groundBody = world.CreateBody(&groundBodyDef);
    b2PolygonShape groundBox;
    groundBox.SetAsBox(50.0f, -40.0f);
    groundBody->CreateFixture(&groundBox, -5.0f);
    //setup for the planes body in the physics world
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(200.0f, 600.0f);
    body = world.CreateBody(&bodyDef);
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(1.0f, 1.0f);
    //important body settings for the plane body
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    body->CreateFixture(&fixtureDef);
    // apply an upwards force on the planes body, with the planes png getting set to the location of the body
    body->ApplyForce( b2Vec2(0, -17000.0f), body->GetWorldCenter(), true);
    //this method is called when we know we reached the end, so display the correct user score
    title->setPlainText("We are ready for take off!\n\nYour score was: " + QString::number(userScore));
}

void EndView::resizeEvent(QResizeEvent *event)
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
