#ifndef STACKVIEW_H
#define STACKVIEW_H

#include <QObject>
#include <QWidget>
#include <QGraphicsRectItem>
#include <QGraphicsView>
#include <QGraphicsWidget>
#include <QPropertyAnimation>

// Credit to Marek R from StackOverflow for this idea
// Post Link: https://stackoverflow.com/questions/30317184/how-to-animate-the-outline-of-a-qgraphicsitem-in-real-time
class LuggageAnimator : public QGraphicsObject {
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
    private:
        QAbstractGraphicsShapeItem *mParent;
        QPropertyAnimation *mAnimation;
    public:
        LuggageAnimator(QAbstractGraphicsShapeItem * parent);

        QPointF pos() const;
    public slots:
        void setPos(const QPointF &newPos);
    public:
        void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
        QRectF boundingRect() const;
        QPropertyAnimation *animation() const;
};

class StackView : public QGraphicsView
{
    Q_OBJECT
    public:
        StackView(QWidget *parent = nullptr);
    private:
        qreal aspectRatio;

        QGraphicsScene *stackScene;

        QGraphicsRectItem *receivingTunnel;
        QGraphicsRectItem *receivingConveyor;
        QGraphicsRectItem *sendingTunnel;
        QGraphicsRectItem *sendingConveyor;

        QGraphicsLineItem *divider;

        QGraphicsRectItem *luggage;
        LuggageAnimator *animator;

        QGraphicsProxyWidget *luggageAdder;

        QGraphicsProxyWidget *node;

        void resizeEvent(QResizeEvent *event) override;
};

#endif // STACKVIEW_H
