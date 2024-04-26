/**
Implementation of the GraphicsAnimator class, which facilitates the animation of QGraphicsItems

University of Utah CS 3505 final project, group LAQE
Writen by Lucas Pearce, Ethan Block, Will Black, Quinn Pritchett, Aleko Louras
**/

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
