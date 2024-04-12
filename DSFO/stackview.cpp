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
// animator->animation()->start();




    // QPropertyAnimation *luggageAnim = new QPropertyAnimation(animator, "scale");
    // luggageAnim->setStartValue(1);
    // luggageAnim->setEndValue(1.4);
    // luggageAnim->setDuration(5000);
    // luggageAnim->setEasingCurve(QEasingCurve::SineCurve);
    // luggageAnim->start();



    // QPropertyAnimation *luggageAnimation = new QPropertyAnimation(luggage, "y");


}

void StackView::resizeEvent(QResizeEvent *event) {
    qreal horizontalCenter = rect().center().x();

    fitInView(stackScene->sceneRect(), Qt::KeepAspectRatio);
    centerOn(horizontalCenter, rect().center().y());

    // divider->moveBy(100, 0);

    // divider->setLine(horizontalCenter, rect().top(), horizontalCenter, rect().bottom());


    // QWidget *parent = parentWidget();
    // if (!parent) return;

    // int newWidth = event->size().width();
    // int newHeight = event->size().height();

    // if (newWidth < newHeight)
    //     if (newHeight < parent->width() * 0.5)
    //         setGeometry(pos().x(), pos().y(), newHeight, newHeight);
    //     else setGeometry(pos().x(), pos().y(), newWidth, newWidth);
    // else
    //     if (newHeight < parent->height() - 150)
    //         setGeometry(pos().x(), pos().y(), newWidth, newWidth);
    //     else setGeometry(pos().x(), pos().y(), newHeight, newHeight);

    // setImage(image);
}

LuggageAnimator::LuggageAnimator(QAbstractGraphicsShapeItem * parent) : QGraphicsObject(parent), mParent(parent) {

    // QPainter painter;
    // painter.fillRect(0, 0, 100, 100, Qt::black);

    setFlags(QGraphicsItem::ItemHasNoContents);
    mAnimation = new QPropertyAnimation(this, "pos");
    qDebug() << mAnimation->propertyName();

    // QPropertyAnimation *anim = new QPropertyAnimation(this, "pos", this);
    // anim->setDuration(10000);
    // anim->setStartValue(QPoint(0, 0));
    // anim->setEndValue(QPoint(100, 250));
    // anim->start();
}

void LuggageAnimator::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {}

QRectF LuggageAnimator::boundingRect() const { return QRectF(); }

QPropertyAnimation* LuggageAnimator::animation() const { return mAnimation; }


