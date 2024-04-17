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

StackView::StackView(QWidget *parent) : QGraphicsView(parent)
{
    // These are helpful for relative positioning and resizing
    aspectRatio = 1.25;
    QRectF sceneBox(0, 0, 500, 400);
    qreal horizontalCenter = sceneBox.center().x();

    // Defining the dimensions first also helps with relative positioning
    qreal tunnelWidth = 100;
    qreal tunnelHeight = 50;
    qreal conveyorWidth = 75;
    qreal conveyorHeight = 200;
    qreal luggageAdderWidth = 60;
    qreal luggageAdderHeight = 50;

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
    qreal rightConveyorY = rightTunnelY - conveyorHeight;

    qreal luggageAdderX = (horizontalCenter - luggageAdderWidth) / 2;
    qreal luggageAdderY = tunnelHeight + conveyorHeight - luggageAdderHeight - (conveyorWidth - luggageAdderWidth) / 2;
    QLineF dividingLine(horizontalCenter, sceneBox.top(), horizontalCenter, sceneBox.bottom());

    // Below is where the scene is actually created and
    // items added to it.
    stackScene = new QGraphicsScene(sceneBox, this);
    divider = stackScene->addLine(dividingLine, QPen(Qt::black, 3));

    receivingConveyor = stackScene->addRect(leftConveyorX, leftConveyorY, conveyorWidth, conveyorHeight, QPen(Qt::black, 3), Qt::gray);
    sendingConveyor = stackScene->addRect(rightConveyorX, rightConveyorY, conveyorWidth, conveyorHeight, QPen(Qt::black, 3), Qt::gray);

    // The luggage is declared here to ensure that it's shown above
    // the conveyors and below the tunnels.
    // A list of luggage items will be added soon
    luggageAdder = stackScene->addWidget(new QPushButton());
    luggageAdder->resize(luggageAdderWidth, luggageAdderHeight);
    luggageAdder->setPos(luggageAdderX, luggageAdderY);
    luggage = stackScene->addRect(0, 0, 60, 50, QPen(Qt::black, 2), Qt::white);

    receivingTunnel = stackScene->addRect(leftTunnelX, leftTunnelY, tunnelWidth, tunnelHeight, QPen(Qt::black, 3), Qt::gray);
    sendingTunnel = stackScene->addRect(rightTunnelX, rightTunnelY, tunnelWidth, tunnelHeight, QPen(Qt::black, 3), Qt::gray);

    // This is useful for animating multiple objects at the same time.
    // It's not yet necessary, so I only include it now for demo purposes.
    QParallelAnimationGroup *animation = new QParallelAnimationGroup(this);

    animator = new GraphicsAnimator(luggage, "pos");
    animator->animation()->setStartValue(QPointF(luggageAdderX, luggageAdderY));
    // animator->animation()->setKeyValueAt(0, QPointF(50, 200));
    // animator->animation()->setKeyValueAt(0.25, QPointF(200, 200));
    // animator->animation()->setKeyValueAt(0.5, QPointF(200, 50));
    // animator->animation()->setKeyValueAt(0.75, QPointF(50, 50));
    animator->animation()->setEndValue(QPointF(luggageAdderX, sceneBox.top() - 20));
    animator->animation()->setDuration(2000);
    // animator->animation()->setEasingCurve(QEasingCurve::SineCurve);

    // animator->animation()->setLoopCount(-1);
    // animator->animation()->start();
    animation->addAnimation(animator->animation());
    animation->setLoopCount(-1);
    animation->start();

    setScene(stackScene);

    // sendingConveyor->setFlag(QGraphicsItem::ItemIsSelectable);
    // luggage->setFlag(QGraphicsItem::ItemIsMovable);
    // centerOn(horizontalCenter, rect().center().y());
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

    // Methods like this could be useful if we wanted to move
    // things around after resizing
    // divider->moveBy(100, 0);
}


