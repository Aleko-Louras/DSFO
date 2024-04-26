/**
StackView teaches a user the stack data structure by creating a scenario
in which luggage is loaded onto a plane with a FILO protocol.

University of Utah CS 3505 final project, group LAQE
Writen by Lucas Pearce, Ethan Block, Will Black, Quinn Pritchett, Aleko Louras
**/

#ifndef STACKVIEW_H
#define STACKVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include "graphicsanimator.h"

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
        QGraphicsProxyWidget *addZone;
        QGraphicsProxyWidget *animationButton;

        QList<GraphicsAnimator*> luggage;

        QRectF receiveZoneRect;

        /// Resizes 'this' StackView alongside its parent widget.
        void resizeEvent(QResizeEvent *event) override;

    private slots:
        /// \brief tryNextAnimation will recursively animate the loading of luggage
        /// onto the plane until the luggage list is empty.
        void tryNextAnimation();
        /// \brief addLuggage will send a bag down the receiving conveyor that can
        /// later be loaded onto the plane, temporarily disabling the addZone before
        /// it does so.
        void addLuggage();
        /// \brief animate will begin animation on the receiving conveyor by calling
        /// tryNextAnimation.
        void animate();
};

#endif // STACKVIEW_H
