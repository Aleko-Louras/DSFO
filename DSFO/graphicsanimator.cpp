#include "graphicsanimator.h"

GraphicsAnimator::GraphicsAnimator(QGraphicsItem *target, const QByteArray &propertyName) : QGraphicsObject(target), target(target)
{
    setFlags(QGraphicsItem::ItemHasNoContents);
    targetAnimation = new QPropertyAnimation(this, propertyName);
}

QPointF GraphicsAnimator::pos() const { return target->pos(); }
void GraphicsAnimator::setPos(const QPointF &newPos) { target->setPos(newPos); }
void GraphicsAnimator::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {}
QRectF GraphicsAnimator::boundingRect() const { return QRectF(); }
QPropertyAnimation* GraphicsAnimator::animation() { return targetAnimation; }
