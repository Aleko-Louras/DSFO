#include "title.h"
#include "ui_title.h"
#define DEGTORAD 0.0174532925199432957f

Title::Title(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Title), world(b2Vec2(0.0f, 0.0f))
{
    ui->setupUi(this);
    timer = new QTimer(this);
    timer2 = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Title::handleTrigger);
    timer->start(25);
    //connect(this, trigge, this, handleTrigger());
    // b2Vec2 gravity(0.0f, -10.0f);
    // b2World world(gravity);
    b2BodyDef myBodyDef;
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, -10.0f);
    b2Body* groundBody = world.CreateBody(&groundBodyDef);
    b2PolygonShape groundBox;
    groundBox.SetAsBox(50.0f, 3.0f);
    groundBody->CreateFixture(&groundBox, 0.0f);
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(0.0f, 4.0f);
    body = world.CreateBody(&bodyDef);
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(1.0f, 1.0f);
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.9f;
    body->CreateFixture(&fixtureDef);
    body->SetLinearVelocity(b2Vec2(0, -1.0f));
    //body->SetAngularVelocity(-90 * DEGTORAD);
    connect(timer2, &QTimer::timeout, this, [this]() {body->SetLinearVelocity(b2Vec2(1, 0.0f));});
    timer2->start(5000);

    // QRect newPosition(0, 0, 429, 311);
    // for(int i = 0; i < 5; i++){
    //     QTimer::singleShot(100, [newPosition, this]() {ui->label->setGeometry(newPosition); qDebug() << "single shot!";});
    //     QRect position = ui->label->geometry();
    // }



    // qDebug() << "Title is being hit";


}

// void Title::setPosition(QRect newPosition) {
//     newPosition.setY(newPosition.y() + 1);
//     ui->label->setGeometry(newPosition);
// }
void Title::handleTrigger(){
    float timeStep = 1.0f / 60.0f;
    int32 velocityIterations = 6;
    int32 positionIterations = 2;
    // for (int32 i = 0; i < 60; ++i)
    // {
    //qDebug() << "Got called";
    world.Step(timeStep, velocityIterations, positionIterations);
    b2Vec2 position = body->GetPosition();

    float angle = body->GetAngle();
    //qDebug() << position.y;
    int x = ui->label->geometry().x();
    ui->label->setGeometry(x, position.y - (position.y*30), 511, 231);
    //}
}
Title::~Title()
{
    delete ui;
}
