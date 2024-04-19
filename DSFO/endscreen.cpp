#include "endscreen.h"
#include "ui_endscreen.h"

EndScreen::EndScreen(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::EndScreen),  world(b2Vec2(0.0f, 0.0f))
{
    ui->setupUi(this);
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &EndScreen::handleTrigger);
    timer->start(10);

    QPixmap pixmap(":/images/airplane-front-view-design-illustration-free.png");
    int w = ui->label->width();
    int h = ui->label->height();
    // set a scaled pixmap to a w x h window keeping its aspect ratio
    ui->label->setPixmap(pixmap.scaled(w,h,Qt::KeepAspectRatio));
    b2BodyDef myBodyDef;
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, -10.0f);
    b2Body* groundBody = world.CreateBody(&groundBodyDef);
    b2PolygonShape groundBox;
    groundBox.SetAsBox(50.0f, 3.0f);
    groundBody->CreateFixture(&groundBox, -5.0f);
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(0.0f, 0.0f);
    body = world.CreateBody(&bodyDef);
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(1.0f, 1.0f);
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    //fixtureDef.restitution = 0.9f;
    body->CreateFixture(&fixtureDef);
    //body->ApplyForce( b2Vec2(1.0f,0), body->GetWorldCenter(), true);
}

void EndScreen::handleTrigger(){

        float timeStep = 1.0f / 60.0f;
        int32 velocityIterations = 6;
        int32 positionIterations = 2;

        world.Step(timeStep, velocityIterations, positionIterations);
        b2Vec2 position = body->GetPosition();

        // Convert position from meters to pixels
        float pixelsPerMeter = 30;
        float xPosPixels = position.x * pixelsPerMeter;
        float yPosPixels = position.y * pixelsPerMeter;

        int widgetWidth = this->width();
        int labelWidth = ui->label->width();

        const int wrapWidth = widgetWidth + labelWidth; // Total width for wrapping

        float wrappedXPos = std::fmod(xPosPixels, wrapWidth);

        if (wrappedXPos < 0)
            wrappedXPos += wrapWidth;

        ui->label->move(wrappedXPos - labelWidth, yPosPixels - (yPosPixels * 30));
}
EndScreen::~EndScreen()
{
    delete ui;
}
