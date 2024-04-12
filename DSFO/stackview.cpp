#include "stackview.h"
#include <QObject>
#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QDebug>
#include <QLayout>
#include <QResizeEvent>

StackView::StackView(QWidget *parent) : QGraphicsView(parent) {

    QRectF sceneBox(0, 0, 400, 400);
    stackScene = new QGraphicsScene(sceneBox, this);

    qreal horizontalCenter = sceneBox.center().x();

    QSizeF conveyorDim = QSizeF(75, 200);
    QSizeF tunnelDim = QSizeF(100, 50);

    QPointF receivingTunnelPos((horizontalCenter - tunnelDim.width()) / 2, sceneBox.top());
    QPointF sendingTunnelPos(horizontalCenter + (horizontalCenter - tunnelDim.width()) / 2, sceneBox.bottom() - tunnelDim.height());

    QPointF receivingConveyorPos((horizontalCenter - conveyorDim.width()) / 2, sceneBox.top() + tunnelDim.height());
    QPointF sendingConveyorPos(horizontalCenter + (horizontalCenter - conveyorDim.width()) / 2, sceneBox.bottom() - tunnelDim.height() - conveyorDim.height());

    QLineF dividingLine(horizontalCenter, sceneBox.top(), horizontalCenter, sceneBox.bottom());

    receivingTunnel = stackScene->addRect(QRectF(receivingTunnelPos, tunnelDim), QPen(Qt::black, 3), Qt::gray);
    receivingTunnel->setFlag(QGraphicsItem::ItemIsMovable);

    receivingConveyor = stackScene->addRect(QRectF(receivingConveyorPos, conveyorDim), QPen(Qt::black, 3), Qt::gray);
    receivingConveyor->setFlag(QGraphicsItem::ItemIsMovable);

    sendingTunnel = stackScene->addRect(QRectF(sendingTunnelPos, tunnelDim), QPen(Qt::black, 3), Qt::gray);
    sendingTunnel->setFlag(QGraphicsItem::ItemIsMovable);

    sendingConveyor = stackScene->addRect(QRectF(sendingConveyorPos, conveyorDim), QPen(Qt::black, 3), Qt::gray);
    sendingConveyor->setFlag(QGraphicsItem::ItemIsMovable);

    divider = stackScene->addLine(dividingLine, QPen(Qt::black, 3));
    divider->setFlag(QGraphicsItem::ItemIsMovable);

    setScene(stackScene);

    qDebug() << receivingTunnel->scenePos();
    qDebug() << receivingTunnel->pos();
}

void StackView::resizeEvent(QResizeEvent *event) {
    // qDebug() << event->oldSize();
    qDebug() << event->size();
    qDebug() << rect();
    // qDebug() << sceneRect();

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
