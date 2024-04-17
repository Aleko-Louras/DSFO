#ifndef STACKVIEW_H
#define STACKVIEW_H

#include <QObject>
#include <QWidget>
#include <QGraphicsRectItem>
#include <QGraphicsView>
#include <QGraphicsWidget>
#include <QPropertyAnimation>
#include "graphicsanimator.h"

class StackView : public QGraphicsView
{
    Q_OBJECT
    public:
        StackView(QWidget *parent = nullptr);
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
        // Luggage-related items
        QGraphicsRectItem *luggage;
        GraphicsAnimator *animator;
        QGraphicsProxyWidget *luggageAdder;

        /// Resizes 'this' StackView alongside its parent widget.
        void resizeEvent(QResizeEvent *event) override;
};

#endif // STACKVIEW_H
