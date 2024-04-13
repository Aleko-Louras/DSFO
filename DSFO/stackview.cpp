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

StackView::StackView(QWidget *parent) : QGraphicsView(parent) {

    QRectF sceneBox(0, 0, 400, 400);
    stackScene = new QGraphicsScene(sceneBox, this);

    qreal horizontalCenter = sceneBox.center().x();
    qreal tunnelWidth = 100;
    qreal tunnelHeight = 50;
    qreal conveyorWidth = 75;
    qreal conveyorHeight = 200;

    qreal leftTunnelX = (horizontalCenter - tunnelWidth) / 2;
    qreal leftTunnelY = sceneBox.top();
    qreal leftConveyorX = (horizontalCenter - conveyorWidth) / 2;
    qreal leftConveyorY = sceneBox.top() + tunnelHeight;
    qreal rightTunnelX = horizontalCenter + (horizontalCenter - tunnelWidth) / 2;
    qreal rightTunnelY = sceneBox.bottom() - tunnelHeight;
    qreal rightConveyorX = horizontalCenter + (horizontalCenter - conveyorWidth) / 2;
    qreal rightConveyorY = sceneBox.bottom() - tunnelHeight - conveyorHeight;

    QLineF dividingLine(horizontalCenter, sceneBox.top(), horizontalCenter, sceneBox.bottom());

    receivingTunnel = stackScene->addRect(leftTunnelX, leftTunnelY, tunnelWidth, tunnelHeight, QPen(Qt::black, 3), Qt::gray);

    receivingConveyor = stackScene->addRect(leftConveyorX, leftConveyorY, conveyorWidth, conveyorHeight, QPen(Qt::black, 3), Qt::gray);

    sendingTunnel = stackScene->addRect(rightTunnelX, rightTunnelY, tunnelWidth, tunnelHeight, QPen(Qt::black, 3), Qt::gray);

    sendingConveyor = stackScene->addRect(rightConveyorX, rightConveyorY, conveyorWidth, conveyorHeight, QPen(Qt::black, 3), Qt::gray);
    // sendingConveyor->setFlag(QGraphicsItem::ItemIsSelectable);


    divider = stackScene->addLine(dividingLine, QPen(Qt::black, 3));

    luggage = stackScene->addRect(0, 0, 50, 50, QPen(Qt::black, 5), Qt::black);

    animator = new LuggageAnimator(luggage);

    QParallelAnimationGroup *animation = new QParallelAnimationGroup(this);
    // luggage->setFlag(QGraphicsItem::ItemIsMovable);

    // animator->animation()->setStartValue(QPointF(200, 50));
    animator->animation()->setKeyValueAt(0, QPointF(50, 200));
    animator->animation()->setKeyValueAt(0.25, QPointF(200, 200));
    animator->animation()->setKeyValueAt(0.5, QPointF(200, 50));
    animator->animation()->setKeyValueAt(0.75, QPointF(50, 50));
    animator->animation()->setEndValue(QPointF(50, 200));
    animator->animation()->setDuration(500);
    // animator->animation()->setEasingCurve(QEasingCurve::SineCurve);

    animation->addAnimation(animator->animation());

    animation->setLoopCount(-1);
    animation->start();

    // animator->setFlag(QGraphicsItem::ItemIsMovable);
    // QGraphicsItemGroup *leftConveyor;
    // leftConveyor->addToGroup({receivingTunnel, receivingConveyor});

    // luggage =

    setScene(stackScene);

    centerOn(horizontalCenter, rect().center().y());
}

void StackView::resizeEvent(QResizeEvent *event) {

    // divider->moveBy(100, 0);

    QWidget *parent = parentWidget();
    if (!parent) return;

    int newWidth = event->size().width();
    int newHeight = event->size().height();

    if (newWidth < newHeight)
        resize(newWidth, newWidth);
    else resize(newHeight, newHeight);

    fitInView(stackScene->sceneRect(), Qt::KeepAspectRatio);

    int x = (parent->width() - width()) / 2;
    int y = (parent->height() - height()) / 2;

    // qDebug() << QPoint(x, y);
    // qDebug() << parent->width();
    // qDebug() << width();
    // qDebug() << parent->height();
    // qDebug() << newHeight;

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


