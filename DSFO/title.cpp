#include "title.h"
#include "ui_title.h"
#define DEGTORAD 0.0174532925199432957f

Title::Title(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Title), world(b2Vec2(0.0f, 0.0f))
{
    ui->setupUi(this);
    QPixmap pixmap(":/image.png");
    int w = ui->label->width();
    int h = ui->label->height();
// set a scaled pixmap to a w x h window keeping its aspect ratio
    ui->label->setPixmap(pixmap.scaled(w,h,Qt::KeepAspectRatio));
    //rotate pixmap

    std::vector<QPixmap> images = {pixmap.transformed(QTransform().rotate(-90)), pixmap.transformed(QTransform().rotate(-180)),pixmap.transformed(QTransform().rotate(90)), pixmap};
    timer = new QTimer(this);
    timer2 = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Title::handleTrigger);
    timer->start(1);
    //connect(this, trigge, this, handleTrigger());
    // b2Vec2 gravity(0.0f, -10.0f);
    // b2World world(gravity);
    b2BodyDef myBodyDef;
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, -10.0f);
    b2Body* groundBody = world.CreateBody(&groundBodyDef);
    b2PolygonShape groundBox;
    groundBox.SetAsBox(50.0f, 3.0f);
    groundBody->CreateFixture(&groundBox, -5.0f);
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
    //fixtureDef.restitution = 0.9f;
    body->CreateFixture(&fixtureDef);
    //body->SetLinearVelocity(b2Vec2(0, -1.0f));
    //body->SetAngularVelocity(-90 * DEGTORAD);
    //left             /down          /right        up
    std::vector<b2Vec2> angles = {b2Vec2(-0.80f, 0), b2Vec2(0, -.80f), b2Vec2(0.80f, 0), b2Vec2(0, 0.80f)};
    angleIndex = 0;

    // apply left impulse, but only if max velocity is not reached yet

    // apply right impulse, but only if max velocity is not reached yet
    //body->ApplyLinearImpulse( b2Vec2(0,-5), body->GetWorldCenter(), true);
    //body->ApplyLinearImpulse( b2Vec2(0,5), body->GetWorldCenter(), true);
    //body->ApplyForce(b2Vec2(-0.80f, 0), b2Vec2(pos.x, pos.y), true);
    connect(timer2, &QTimer::timeout, this, &Title::setPosition);
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

    //float angle = body->GetAngle();
    //qDebug() << position.y;
    //int x = ui->label->geometry().x();
    ui->label->setGeometry(position.x * 30, position.y - (position.y*30), 511, 231);
    //}
}
void Title::setPosition(){
    b2Vec2 position = body->GetPosition();
    changeDirection(position);
}
void Title::changeDirection(b2Vec2 position){
    qDebug() << "hello";
    body->SetLinearVelocity(b2Vec2(0,0));
    int w = ui->label->width();
    int h = ui->label->height();
    ui->label->setPixmap(images[angleIndex].scaled(w,h,Qt::KeepAspectRatio));
    body->ApplyLinearImpulse(angles[angleIndex], position, true);
    angleIndex++;
}
Title::~Title()
{
    delete ui;
}
