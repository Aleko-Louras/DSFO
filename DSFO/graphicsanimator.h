/**
A proxy object that's useful for animating QGraphicsItems that don't
extend QObject and therefore don't have properties to animate.

Credit to Marek R from StackOverflow for this idea
Post Link: https://stackoverflow.com/questions/30317184/how-to-animate-the-outline-of-a-qgraphicsitem-in-real-time

University of Utah CS 3505 final project, group LAQE
Writen by Lucas Pearce, Ethan Block, Will Black, Quinn Pritchett, Aleko Louras
**/

#ifndef GRAPHICSANIMATOR_H
#define GRAPHICSANIMATOR_H

#include <QGraphicsObject>
#include <QPropertyAnimation>

class GraphicsAnimator : public QGraphicsObject {
    Q_OBJECT
    // If you need to define other properties, you'll need
    // to set up the necessary read and write methods.
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
private:
    QPropertyAnimation *targetAnimation;
public:
    QGraphicsItem *target;

    /// Constructs a GraphicsAnimator given a parentItem and animation
    GraphicsAnimator(QGraphicsItem *target, const QByteArray &propertyName);
    /// Read method for a QGraphicsItem's position
    QPointF pos() const;
public slots:
    /// Write method for a QGraphicsItem's position.
    /// @param newPos is the item's new position
    void setPos(const QPointF &newPos);
public:
    /// Overrides QGraphicsObject's paint method to avoid drawing a proxy object.
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    /// Overrides QGraphicsObject's boundingRect method to ensure the proxy object's
    /// bounding box is nonexistant and can't be clicked.
    QRectF boundingRect() const;
    /// Returns the animation set for 'this' GraphicsAnimator.
    QPropertyAnimation *animation();
};

#endif // GRAPHICSANIMATOR_H
