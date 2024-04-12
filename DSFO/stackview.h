#ifndef STACKVIEW_H
#define STACKVIEW_H

#include <QObject>
#include <QWidget>
#include <QGraphicsRectItem>
#include <QGraphicsView>

class StackView : public QGraphicsView
{
    Q_OBJECT
    public:
        StackView(QWidget *parent = nullptr);
    private:
        QGraphicsScene *stackScene;

        QGraphicsRectItem *receivingTunnel;
        QGraphicsRectItem *receivingConveyor;
        QGraphicsRectItem *sendingTunnel;
        QGraphicsRectItem *sendingConveyor;
        QGraphicsRectItem *luggage;
        QGraphicsLineItem *divider;

        QGraphicsProxyWidget *node;

        void resizeEvent(QResizeEvent *event) override;
};

#endif // STACKVIEW_H
