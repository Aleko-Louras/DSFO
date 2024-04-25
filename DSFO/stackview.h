/**
header file for the stack view. This is where the stack interactive clicker and animation is displayed, the implementation is here.
University of Utah CS 3505 final project, group LAQE
Writen by Lucas Pearce, Ethan Block, Will Black, Quinn Pritchett, Aleko Louras
**/

#ifndef STACKVIEW_H
#define STACKVIEW_H

#include <QObject>
#include <QWidget>
#include <QGraphicsRectItem>
#include <QGraphicsView>
#include <QGraphicsWidget>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QPixMap>
#include <QGraphicsPixmapItem>
#include "graphicsanimator.h"
#include <QLabel>

class StackView : public QGraphicsView
{
    Q_OBJECT
    public:
        StackView(QWidget *parent = nullptr);
        ~StackView();
    private:
        // Ratio of width to height (currently 5:4)
        qreal aspectRatio = 1.25;

        QGraphicsScene *stackScene;
        // Items drawn in the QGraphicsScene
        QGraphicsRectItem *receivingTunnel;
        QGraphicsRectItem *receivingConveyor;
        QGraphicsRectItem *sendingTunnel;
        QGraphicsRectItem *sendingConveyor;
        QGraphicsLineItem *divider;
        QGraphicsItem *plane;
        // Luggage-related items
        // QGraphicsRectItem *luggage;
        // QParallelAnimationGroup *luggageAnimation = new QParallelAnimationGroup(this);
        // GraphicsAnimator *animator;
        QGraphicsProxyWidget *addZone;
        QGraphicsProxyWidget *animationButton;
        QGraphicsProxyWidget *testButton;

        QList<GraphicsAnimator*> luggage;

        QRectF receiveZoneRect;

        QLabel label;

        /// Resizes 'this' StackView alongside its parent widget.
        void resizeEvent(QResizeEvent *event) override;

    private slots:
        void tryNextAnimation();
        void addLuggage();
        void animate();
};

#endif // STACKVIEW_H
