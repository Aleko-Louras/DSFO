#include "stackview.h"
#include <QObject>
#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QDebug>
#include <QLayout>
#include <QResizeEvent>
#include <QSequentialAnimationGroup>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include <iostream>

StackView::StackView(QWidget *parent) : QGraphicsView(parent) {
    aspectRatio = 1.25;

    QRectF sceneBox(0, 0, 500, 400);
    stackScene = new QGraphicsScene(sceneBox, this);

    qreal horizontalCenter = sceneBox.center().x();
    qreal tunnelWidth = 100;
    qreal tunnelHeight = 50;
    qreal conveyorWidth = 75;
    qreal conveyorHeight = 200;
    qreal luggageAdderWidth = 60;
    qreal luggageAdderHeight = 50;

    qreal leftTunnelX = (horizontalCenter - tunnelWidth) / 2;
    qreal leftTunnelY = sceneBox.top();
    qreal leftConveyorX = (horizontalCenter - conveyorWidth) / 2;
    qreal leftConveyorY = leftTunnelY + tunnelHeight;

    qreal rightTunnelX = horizontalCenter + leftTunnelX;
    qreal rightTunnelY = sceneBox.bottom() - tunnelHeight;
    qreal rightConveyorX = horizontalCenter + leftConveyorX;
    qreal rightConveyorY = rightTunnelY - conveyorHeight;

    qreal luggageAdderX = (horizontalCenter - luggageAdderWidth) / 2;
    qreal luggageAdderY = tunnelHeight + conveyorHeight - luggageAdderHeight - (conveyorWidth - luggageAdderWidth) / 2;

    QLineF dividingLine(horizontalCenter, sceneBox.top(), horizontalCenter, sceneBox.bottom());
    divider = stackScene->addLine(dividingLine, QPen(Qt::black, 3));

    receivingConveyor = stackScene->addRect(leftConveyorX, leftConveyorY, conveyorWidth, conveyorHeight, QPen(Qt::black, 3), Qt::gray);
    sendingConveyor = stackScene->addRect(rightConveyorX, rightConveyorY, conveyorWidth, conveyorHeight, QPen(Qt::black, 3), Qt::gray);

    luggageAdder = stackScene->addWidget(new QPushButton());
    luggageAdder->resize(luggageAdderWidth, luggageAdderHeight);
    luggageAdder->setPos(luggageAdderX, luggageAdderY);
    luggage = stackScene->addRect(0, 0, 60, 50, QPen(Qt::black, 2), Qt::white);
    animator = new LuggageAnimator(luggage);

    receivingTunnel = stackScene->addRect(leftTunnelX, leftTunnelY, tunnelWidth, tunnelHeight, QPen(Qt::black, 3), Qt::gray);
    sendingTunnel = stackScene->addRect(rightTunnelX, rightTunnelY, tunnelWidth, tunnelHeight, QPen(Qt::black, 3), Qt::gray);

    QParallelAnimationGroup *animation = new QParallelAnimationGroup(this);

    animator->animation()->setStartValue(QPointF(luggageAdderX, luggageAdderY));
    // animator->animation()->setKeyValueAt(0, QPointF(50, 200));
    // animator->animation()->setKeyValueAt(0.25, QPointF(200, 200));
    // animator->animation()->setKeyValueAt(0.5, QPointF(200, 50));
    // animator->animation()->setKeyValueAt(0.75, QPointF(50, 50));
    animator->animation()->setEndValue(QPointF(luggageAdderX, sceneBox.top() - 20));
    animator->animation()->setDuration(2000);
    // animator->animation()->setEasingCurve(QEasingCurve::SineCurve);

    animation->addAnimation(animator->animation());
    animation->setLoopCount(-1);
    animation->start();

    setScene(stackScene);

    // sendingConveyor->setFlag(QGraphicsItem::ItemIsSelectable);
    // luggage->setFlag(QGraphicsItem::ItemIsMovable);
    // centerOn(horizontalCenter, rect().center().y());
}

void StackView::resizeEvent(QResizeEvent *event) {

    // divider->moveBy(100, 0);

    QWidget *parent = parentWidget();
    if (!parent) return;

    int newWidth = event->size().width();
    int newHeight = event->size().height();

    if (newWidth < newHeight * aspectRatio)
        resize(newWidth, newWidth / aspectRatio);
    else resize(newHeight * aspectRatio, newHeight);


    fitInView(stackScene->sceneRect(), Qt::KeepAspectRatio);

    int x = (parent->width() - width()) / 2;
    int y = (parent->height() - height()) / 2;

    move(x,y);
}

LuggageAnimator::LuggageAnimator(QAbstractGraphicsShapeItem * parent) : QGraphicsObject(parent), mParent(parent) {

    setFlags(QGraphicsItem::ItemHasNoContents);
    mAnimation = new QPropertyAnimation(this, "pos");
}

QPointF LuggageAnimator::pos() const { return mParent->pos(); }

void LuggageAnimator::setPos(const QPointF &newPos) { mParent->setPos(newPos); }

void LuggageAnimator::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {}

QRectF LuggageAnimator::boundingRect() const { return QRectF(); }

QPropertyAnimation* LuggageAnimator::animation() const { return mAnimation; }


