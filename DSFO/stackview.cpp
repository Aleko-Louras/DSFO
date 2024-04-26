/**
StackView teaches a user the stack data structure by creating a scenario
in which luggage is loaded onto a plane with a FILO protocol.

University of Utah CS 3505 final project, group LAQE
Writen by Lucas Pearce, Ethan Block, Will Black, Quinn Pritchett, Aleko Louras
**/

#include "stackview.h"
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include <QTimer>

StackView::StackView(QWidget *parent) : QGraphicsView(parent)
{
    // These are helpful for relative positioning
    QRectF sceneBox(0, 0, 500, 400); //! This is linked to the aspect ratio
    qreal horizontalCenter = sceneBox.center().x();

    qreal tunnelWidth = 60;
    qreal tunnelHeight = 40;
    qreal conveyorWidth = 50;
    qreal conveyorHeight = 160;
    qreal luggageZoneWidth = 40;
    qreal luggageZoneHeight = 30;

    // All of the values below are defined using positions relative to the
    // sceneBox, which helps with reusability and, possibly, readability.
    // Absolute positions can also be used; for example:
    // rightTunnelY = sceneBox.bottom() - tunnelHeight = 400 - 50 = 350
    qreal leftTunnelX = (horizontalCenter - tunnelWidth) / 2;
    qreal leftTunnelY = sceneBox.top();
    qreal leftConveyorX = (horizontalCenter - conveyorWidth) / 2;
    qreal leftConveyorY = leftTunnelY + tunnelHeight;

    qreal rightTunnelX = horizontalCenter + leftTunnelX;
    qreal rightTunnelY = sceneBox.bottom() - tunnelHeight;
    qreal rightConveyorX = horizontalCenter + leftConveyorX;
    qreal rightConveyorY = rightTunnelY - conveyorHeight - 100;

    qreal addZoneX = (horizontalCenter - luggageZoneWidth) / 2;
    qreal addZoneY = tunnelHeight + conveyorHeight - luggageZoneHeight - (conveyorWidth - luggageZoneWidth) / 2;
    QLineF dividingLine(horizontalCenter, sceneBox.top(), horizontalCenter, sceneBox.bottom());

    receiveZoneRect = QRectF(horizontalCenter + addZoneX, rightConveyorY + (conveyorWidth - luggageZoneWidth) / 2,
                             luggageZoneWidth, luggageZoneHeight);

    //! Below is where the scene is actually created and items added to it.
    stackScene = new QGraphicsScene(sceneBox, this);
    stackScene->addLine(dividingLine, QPen(Qt::black, 3));
    stackScene->addRect(leftConveyorX, leftConveyorY, conveyorWidth, conveyorHeight, QPen(Qt::black, 3), Qt::gray);
    stackScene->addRect(rightConveyorX, rightConveyorY, conveyorWidth, conveyorHeight + 100, QPen(Qt::black, 3), Qt::gray);

    // Conveyor tunnels are placed above the conveyor and any luggage added later on.
    QGraphicsRectItem *receivingTunnel = stackScene->addRect(leftTunnelX, leftTunnelY, tunnelWidth, tunnelHeight, QPen(Qt::black, 3), Qt::gray);
    QGraphicsRectItem *sendingTunnel = stackScene->addRect(rightTunnelX, rightTunnelY, tunnelWidth, tunnelHeight, QPen(Qt::black, 3), Qt::gray);
    receivingTunnel->setZValue(1);
    sendingTunnel->setZValue(1);

    QFont addZoneFont("Helvetica [Cronyx]", 16);
    addZoneFont.setBold(true);

    // The "addZone" allows the user to place luggage on the conveyor.
    QPushButton *addLuggage = new QPushButton("+");
    connect(addLuggage, &QPushButton::clicked,this, &StackView::addLuggage);
    addLuggage->setFixedSize(luggageZoneWidth, luggageZoneHeight);
    addLuggage->setStyleSheet("QPushButton { background-color: white; color: black;}");
    addLuggage->move(addZoneX, addZoneY);
    addLuggage->setFont(addZoneFont);
    addZone = stackScene->addWidget(addLuggage);

    // An animation button allows the user to start the receiving conveyor.
    QPushButton *animationBtn = new QPushButton("Animate");
    connect(animationBtn, &QPushButton::clicked, this, &StackView::animate);
    animationBtn->setStyleSheet("QPushButton { background-color: white; color: black; }");
    animationBtn->setEnabled(false);
    animationButton = stackScene->addWidget(animationBtn);
    animationButton->setPos(horizontalCenter - animationButton->minimumWidth() / 2, sceneBox.center().y()  - animationButton->minimumHeight() / 2);

    QImage planeImg(":/images/birdsEyePlane.png");
    qDebug() << planeImg.width();
    qDebug() << planeImg.height();
    planeImg = planeImg.scaled(500, 400, Qt::KeepAspectRatio, Qt::SmoothTransformation).copy(QRect(120, 100, 300, 200));

    QGraphicsPixmapItem *plane = stackScene->addPixmap(QPixmap::fromImage(planeImg));
    plane->setPos(horizontalCenter, sceneBox.top());
    plane->setZValue(1);

    setScene(stackScene);
}

StackView::~StackView()
{
    delete stackScene;
    delete addZone;
}

void StackView::resizeEvent(QResizeEvent *event)
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
    fitInView(stackScene->sceneRect(), Qt::KeepAspectRatio);

    // Centers StackView inside its parent widget
    int x = (parent->width() - width()) / 2;
    int y = (parent->height() - height()) / 2;
    move(x,y);
}

void StackView::addLuggage()
{
    if (!addZone->isVisible())
        return;

    QGraphicsRectItem *bag = stackScene->addRect(addZone->rect(), QPen(Qt::black, 2), Qt::white);
    QGraphicsSimpleTextItem *number = new QGraphicsSimpleTextItem(QString::number(luggage.count() + 1), bag);
    GraphicsAnimator *luggageAnimator = new GraphicsAnimator(bag, "pos");

    addZone->setVisible(false);
    luggage.push_back(luggageAnimator);
    animationButton->setEnabled(luggage.count() > 2);

    QFont font("Helvetica [Cronyx]", 14);
    font.setBold(true);
    number->setFont(font);
    number->setPos(QPointF(bag->x() + (bag->boundingRect().width() - number->boundingRect().width()) / 2,
                           bag->y() + (bag->boundingRect().height() - number->boundingRect().height()) / 2));

    // Advances the luggage to the sending conveyor when its animation is finished for the
    // the receiving conveyor
    connect(luggageAnimator->animation(), &QPropertyAnimation::finished,
            this,
            [this, luggageAnimator] {
                luggageAnimator->setPos(QPointF(receiveZoneRect.x(), sceneRect().bottom() - addZone->rect().height()));
                luggageAnimator->animation()->setStartValue(luggageAnimator->pos());
                luggageAnimator->animation()->setEndValue(receiveZoneRect.topLeft());
                luggageAnimator->animation()->setDuration(2000);
            });

    // Animates luggage's progress along the receiving conveyor
    luggageAnimator->animation()->setStartValue(addZone->pos());
    luggageAnimator->animation()->setEndValue(QPointF(addZone->x(), sceneRect().top()));
    luggageAnimator->animation()->setDuration(2000);
    luggageAnimator->animation()->start();

    qreal delay = 2000 * (addZone->boundingRect().height() / addZone->y());

    QTimer::singleShot(delay, this, [this] {addZone->setVisible(true);});
}

void StackView::animate()
{
    animationButton->setEnabled(false);
    addZone->setEnabled(false);

    QPropertyAnimation *animation = luggage.last()->animation();
    if (animation->state() == QAbstractAnimation::Running) {
        // Prevents the animation from starting until all luggage is off
        // the receiving conveyor.
        QTimer::singleShot(animation->duration() - animation->currentTime() + 100, this, &StackView::tryNextAnimation);
        return;
    }

    tryNextAnimation();
}

void StackView::tryNextAnimation()
{
    if (luggage.empty()) {
        addZone->setEnabled(true);
        return;
    }

    GraphicsAnimator *animator = luggage.takeLast();
    QPropertyAnimation *animation = animator->animation();
    connect(animation, &QPropertyAnimation::finished, animator, &GraphicsAnimator::deleteLater);

    // Sets a delay so that one luggage can't overlap another
    qreal targetHeight = receiveZoneRect.height();
    qreal travelDistance = animation->startValue().toPointF().y() - animation->endValue().toPointF().y();
    qreal delay = animation->duration() * (targetHeight / travelDistance) * 1.5;

    animation->start();
    QTimer::singleShot(delay, this, &StackView::tryNextAnimation);
}
